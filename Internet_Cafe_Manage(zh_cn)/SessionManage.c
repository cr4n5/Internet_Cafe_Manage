#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<sys/stat.h>
#include"SessionManage.h"
#include"UserManage.h"
#include"cJSON.h"
#include<time.h>

//判断HH.mm格式的时间是否合法
bool isDayTime(const char *timeStr) {
    int hour, minute;
    char *dup = strdup(timeStr); // 复制字符串以避免修改原始字符串
    char *token = strtok(dup, "."); // 分割小时部分

    if (token == NULL) {
        free(dup);
        return false; // 字符串格式不正确
    }
    hour = atoi(token); // 转换小时为整数

    token = strtok(NULL, "."); // 分割分钟部分
    if (token == NULL) {
        free(dup);
        return false; // 字符串格式不正确
    }
    minute = atoi(token); // 转换分钟为整数

    free(dup); // 释放复制的字符串

    // 校验小时和分钟的范围
    if (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59) {
        return true;
    } else {
        return false;
    }
}

// 检查machine_id是否在available_machine_id数组中
bool isMachineIdAvailable(int machine_id, int available_machine_id[], int size) {
    for (int i = 0; i < size; i++) {
        if (available_machine_id[i] == machine_id) {
            return false; // 找到了machine_id在数组中，返回false
        }
    }
    return true; // 遍历完毕，没有找到，说明不在数组中，返回true
}

//判断日期是否有效
bool iseffective_date(char *date,char *effective_time){
    int year,month,day;
    sscanf(date,"%d.%d.%d",&year,&month,&day);
    int effective_year,effective_month,effective_day;
    sscanf(effective_time,"%d.%d.%d",&effective_year,&effective_month,&effective_day);
    if(year<effective_year){
        return true;
    }else if(year==effective_year){
        if(month<effective_month){
            return true;
        }else if(month==effective_month){
            if(day<=effective_day){
                return true;
            }
        }
    }
    return false;
}

//上网
void StartSession(cJSON *sessions,cJSON *users)
{
    Session session;
    printf("请输入姓名:");
    gets(session.name);
    int size = cJSON_GetArraySize(users);
    cJSON *user = NULL;
    printf("以下是符合姓名的客户:\n");

     //根据name显示所有用户
    int flag = 0;//判断用户是否已经办卡
    for (int i = 0; i < size; i++)
    {
        user = cJSON_GetArrayItem(users,i);
        if(strcmp(cJSON_GetObjectItem(user,"name")->valuestring,session.name) == 0){
            char *out = cJSON_Print(user);
            printf("%s\n",out);
            free(out);
            flag = 1;
        }
    }

    size= cJSON_GetArraySize(sessions);
    int allid=5000;//未办卡用户起始id
    int available_machine_id[size];//忙碌机器位置
    int j=0;
    for (int i = 0; i < size; i++)
    {
        cJSON *session = cJSON_GetArrayItem(sessions,i);
        bool is_online = cJSON_GetObjectItem(session,"is_online")->valueint;
        if(cJSON_GetObjectItem(session,"is_User")->valueint == 0 && is_online){
            allid = cJSON_GetObjectItem(session,"id")->valueint;
        }//找到未办卡用户的可用的id

        if(is_online){
            available_machine_id[j] = cJSON_GetObjectItem(session,"machine_id")->valueint;
            j++;
        }//找到忙碌机器位置
    }
    size = j;
    if(flag == 0){//未办卡用户
        printf("该姓名客户不存在!\n");
        session.id = allid -1;
        session.is_User = 0;
        printf("分配临时客户，此用户id为：%d\n",session.id);
    }else{//已办卡用户,选择id
        char Id[20];
        printf("请输入正确的用户id:");
        gets(Id);
        session.id = atoi(Id);
        session.is_User = 1;
    }
    
    char machine_id[20];
    //选择机器,并检查是否可用
    do
    {
        printf("请输入上网机器位号，确保机器未被使用！:");
        gets(machine_id);
    } while (!isMachineIdAvailable(atoi(machine_id), available_machine_id, size));
    session.machine_id = atoi(machine_id);

    //获取id对应的用户的有效时间
    char effective_time[20];
    size = cJSON_GetArraySize(users);
    if (session.is_User ==1){
        for (int i = 0; i < size; i++)
        {
            user = cJSON_GetArrayItem(users,i);
            if(cJSON_GetObjectItem(user,"id")->valueint == session.id){
                strcpy(effective_time,cJSON_GetObjectItem(user,"effective_time")->valuestring);
                break;
            }
        }
    }
    //输入开始日期和时间
    char date[20];
    printf("请输入上网开始的日期(yyyy.mm.dd):");
    gets(date);
    while(!validateDate(date)){
        printf("格式错误！请重新输入:");
        gets(date);
    }
    if (session.is_User == 1 && !iseffective_date(date,effective_time)){//验证用户有效时间是否过期
        printf("该用户有效期过期!\n");
        printf("请延长有效期至:");
        char new_effective_time[20];
        gets(new_effective_time);
        while(!validateDate(new_effective_time) || !iseffective_date(date,new_effective_time)){
            printf("格式错误或不符合有效期要求！请重新输入:");
            gets(new_effective_time);
        }
        cJSON_ReplaceItemInObject(user,"effective_time",cJSON_CreateString(new_effective_time));
    }
    strcpy(session.date,date);

    do{
        printf("请输入开始时间(HH.mm):");
        gets(date);
    }while(!isDayTime(date));
    strcpy(session.start_time,date);
    
    //正在上网表示
    session.is_online = 1;

    //写入会话
    cJSON *session_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(session_json, "id", session.id);
    cJSON_AddStringToObject(session_json, "name", session.name);
    cJSON_AddNumberToObject(session_json, "machine_id", session.machine_id);
    cJSON_AddStringToObject(session_json, "date", session.date);
    cJSON_AddStringToObject(session_json, "start_time", session.start_time);
    cJSON_AddNumberToObject(session_json, "is_online", session.is_online);
    cJSON_AddNumberToObject(session_json, "is_User", session.is_User);
    cJSON_AddItemToArray(sessions, session_json);

    printf("用户上网登记成功!\n");
}

// 计算两个日期时间之间的小时数
int calculate_hours(const char *start_date, const char *start_time, const char *end_date, const char *end_time) {
    struct tm start_tm = {0}, end_tm = {0};
    int start_year, start_month, start_day, start_hour, start_minute;
    int end_year, end_month, end_day, end_hour, end_minute;

    // 解析开始日期和时间
    sscanf(start_date, "%d.%d.%d", &start_year, &start_month, &start_day);
    sscanf(start_time, "%d.%d", &start_hour, &start_minute);

    // 解析结束日期和时间
    sscanf(end_date, "%d.%d.%d", &end_year, &end_month, &end_day);
    sscanf(end_time, "%d.%d", &end_hour, &end_minute);

    // 填充struct tm结构体
    start_tm.tm_year = start_year - 1900;
    start_tm.tm_mon = start_month - 1;
    start_tm.tm_mday = start_day;
    start_tm.tm_hour = start_hour;
    start_tm.tm_min = start_minute;

    end_tm.tm_year = end_year - 1900;
    end_tm.tm_mon = end_month - 1;
    end_tm.tm_mday = end_day;
    end_tm.tm_hour = end_hour;
    end_tm.tm_min = end_minute;

    // 将struct tm转换为time_t
    time_t start_time_t = mktime(&start_tm);
    time_t end_time_t = mktime(&end_tm);

    // 计算差异并转换为小时
    double diff = difftime(end_time_t, start_time_t);
    int hours = diff / 3600;
    if ((int)diff % 3600 > 0) {
        hours += 1; // 未满一小时按一小时计算
    }

    return hours;
}

//计算金额
void CalculateMoney(cJSON *session,cJSON *users,int id){
    //计算时间
    int hours = calculate_hours(cJSON_GetObjectItem(session,"date")->valuestring,cJSON_GetObjectItem(session,"start_time")->valuestring,cJSON_GetObjectItem(session,"end_date")->valuestring,cJSON_GetObjectItem(session,"end_time")->valuestring);
    printf("该用户上网时间为： %d\n",hours);
    if (cJSON_GetObjectItem(session,"is_User")->valueint==0){//未办卡用户
        double money = 0;
        if(hours<8){
            money = 2*hours;
        }else{
            money = 2*hours*0.9;
        }
        printf("该用户上网应支付： %lf\n",money);
        cJSON_AddNumberToObject(session,"money",money);
    }else{//已办卡用户
        double money = hours*2*0.8;
        printf("该用户上网应支付 %lf\n",money);
        cJSON_AddNumberToObject(session,"money",money);
        int size = cJSON_GetArraySize(users);
        cJSON *user = NULL;
        for (int i = 0; i < size; i++)
        {
            user = cJSON_GetArrayItem(users,i);
            if(cJSON_GetObjectItem(user,"id")->valueint == id){
                double ori_money = cJSON_GetObjectItem(user,"money")->valuedouble;
                if (ori_money < money){//余额不足
                    printf("用户卡内余额不足!\n");
                    printf("请充值金额，输入充值额 :");
                    char recharge[20];
                    gets(recharge);
                    double add_money = atof(recharge);
                    while(ori_money+add_money < money){//充值金额任然不足
                        printf("金额仍然不足！请重新输入充值额:");
                        gets(recharge);
                        add_money = atof(recharge);
                    }
                    cJSON_ReplaceItemInObject(user,"money",cJSON_CreateNumber(ori_money+add_money-money));
                }else{
                    cJSON_ReplaceItemInObject(user,"money",cJSON_CreateNumber(ori_money-money));//扣款
                }
            }
        }
    
    }
}

//结束上网
void EndSession(cJSON *sessions,cJSON *users)
{
    char Id[20];
    printf("请输入用户id:");
    gets(Id);
    int id = atoi(Id);
    int size = cJSON_GetArraySize(sessions);
    cJSON *session = NULL;
    int flag=0;
    for (int i = 0; i < size; i++)
    {
        session = cJSON_GetArrayItem(sessions,i);
        if(cJSON_GetObjectItem(session,"id")->valueint == id && cJSON_GetObjectItem(session,"is_online")->valueint == 1){
            cJSON_ReplaceItemInObject(session,"is_online",cJSON_CreateNumber(0));
            flag=1;
            break;
        }
    }//找到id对应的session
    if (flag==0){
        printf("此id不存在或此用户暂未上网\n");
        return;
    }else{//结束上网，填写结束时间，计算金额
        char end_date[20];
        do{
            printf("请输入结束日期，确保格式正确！(yyyy.mm.dd):");
            gets(end_date);
        }while(!validateDate(end_date));
        cJSON_AddStringToObject(session,"end_date",end_date);
        char end_time[20];
        do{
            printf("请输入结束时间，确保格式正确！(HH.mm)):");
            gets(end_time);
        }while(!isDayTime(end_time));
        cJSON_AddStringToObject(session,"end_time",end_time);
        CalculateMoney(session,users,id);
        printf("客户下网结算成功!\n");
    }
}

//退出，写入文件
void exitSession(cJSON *sessions)
{
    FILE *fp = fopen("sessions.json","w");
    char *out = cJSON_Print(sessions);
    fputs(out,fp);
    fclose(fp);
    free(out);
    cJSON_Delete(sessions);
    printf("退出并保存会话列表成功!\n");
}

//初始化，加载session
void loadSession(cJSON **sessions)
{
    // 打开文件
    FILE *file = NULL;
    file = fopen("sessions.json", "r");
    if (file == NULL) {
        *sessions = cJSON_CreateArray();
        return;
    }

    // 获得文件大小
    struct stat statbuf;
    stat("sessions.json", &statbuf);
    int fileSize = statbuf.st_size;


    // 分配符合文件大小的内存
    char *jsonStr = (char *)malloc(sizeof(char) * fileSize + 1);
    memset(jsonStr, 0, fileSize + 1);


    // 读取文件中的json字符串
    int size = fread(jsonStr, sizeof(char), fileSize, file);
    if (size == 0) {
        *sessions = cJSON_CreateArray();
        fclose(file);
        return;
    }

    // printf("%s\n", jsonStr);
    
    fclose(file);
    *sessions = cJSON_Parse(jsonStr);
    free(jsonStr);
}

// int main(){
//     cJSON *users = NULL;
//     loadUser(&users);
//     cJSON *sessions = NULL;
//     loadSession(&sessions);
    
//     StartSession(sessions,users);
//     // EndSession(sessions,users);
//     exitSession(sessions);
//     exitUser(users);
//     return 0;
// }