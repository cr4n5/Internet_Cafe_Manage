#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<sys/stat.h>
#include"UserManage.h"
#include"cJSON.h"

//判断闰年
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

//判断日期是否合法
bool isValidDate(int year, int month, int day) {
    if (year < 0 || month < 1 || month > 12 || day < 1) {
        return false;
    }
    int daysInMonth[] = {31, 28 + isLeapYear(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return day <= daysInMonth[month - 1];
}

//检查日期格式
bool checkDateFormat(const char *date) {
     // 检查长度
    if (strlen(date) != 10) {
        return false;
    }
    // 检查分隔符
    if (date[4] != '.' || date[7] != '.') {
        return false;
    }
    // 检查年月日是否为数字
    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue; // 跳过分隔符
        if (!isdigit(date[i])) {
            return false;
        }
    }
    return true;
}

//验证日期
bool validateDate(const char *date) {
    if (!checkDateFormat(date)) {
        return false;
    }

    int year, month, day;
    sscanf(date, "%d.%d.%d", &year, &month, &day);

    if (!isValidDate(year, month, day)) {
        return false;
    }
    return true;
}

//添加用户
void addUser(cJSON *users)
{
    User user;
    printf("请输入用户姓名:");
    gets(user.name);//添加姓名

    printf("请输入用户性别(男 or 女):");
    char gender[20];
    gets(gender);
    while (strcmp(gender,"男")!=0 && strcmp(gender,"女")!=0 && gender[0] != '\0'){//可为空
        printf("输入错误！请重新输入:");
        gets(gender);
    }
    if(strcmp(gender,"男")==0){
        user.gender = 1;
    }else
    {
        user.gender = 0;
    }//根据“男”或“女”添加性别

    printf("请输入用户生日日期(yyyy.mm.dd):");
    gets(user.birthday);
    while(!validateDate(user.birthday) && user.birthday[0] != '\0'){
        printf("日期格式错误！请重新输入:");
        gets(user.birthday);
    }//添加生日

    printf("请输入用户手机号:");
    gets(user.phone);
    while(strlen(user.phone) != 11 && user.phone[0] != '\0'){
        printf("手机号错误！请重新输入:");
        gets(user.phone);
    }//添加电话号码


    printf("请输入用户办卡日期(yyyy.mm.dd):");
    gets(user.application_time);
    while(!validateDate(user.application_time) && user.application_time[0] != '\0'){
        printf("日期格式错误！请重新输入:");
        gets(user.application_time);
    }//添加办卡时间

    printf("请输入用户有效日期(yyyy.mm.dd):");
    gets(user.effective_time);
    while(!validateDate(user.effective_time)){
        printf("日期格式错误！请重新输入:");
        gets(user.effective_time);
    }//添加有效时间

    printf("请输入用户充值金额:");
    char money[20];
    gets(money);
    user.money = atof(money);//添加金额

    //添加id，获取的id为最后一个id+1
    int size=cJSON_GetArraySize(users);
    cJSON *temp = NULL;
    temp = cJSON_GetArrayItem(users,size-1);
    user.id = cJSON_GetObjectItem(temp,"id")->valueint+1;
    printf("该用户的id为： %d\n",user.id);


    cJSON *user_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(user_json,"id",user.id);
    cJSON_AddStringToObject(user_json,"name",user.name);
    cJSON_AddNumberToObject(user_json,"gender",user.gender);
    cJSON_AddStringToObject(user_json,"birthday",user.birthday);
    cJSON_AddStringToObject(user_json,"phone",user.phone);
    cJSON_AddStringToObject(user_json,"effective_time",user.effective_time);
    cJSON_AddStringToObject(user_json,"application_time",user.application_time);
    cJSON_AddNumberToObject(user_json,"money",user.money);

    cJSON_AddItemToArray(users,user_json);
    //保存到文件
    // FILE *fp = fopen("user.json","w");
    // char *out = cJSON_Print(users);
    // fputs(out,fp);
    // fclose(fp);
    // free(out);
    // cJSON_Delete(user_json);
    printf("用户办卡成功!\n");
}

void DeleteUser(cJSON *users)
{
    //根据名字选择用户,并删除
    char name[20];
    printf("请输入你想要删除用户的姓名:");
    gets(name);
    int size = cJSON_GetArraySize(users);
    cJSON *user = NULL;
    printf("以下是符合姓名的客户:\n");

    int flag =0;//判断是否找到用户
    for (int i = 0; i < size; i++)
    {
        user = cJSON_GetArrayItem(users,i);
        if(strcmp(cJSON_GetObjectItem(user,"name")->valuestring,name) == 0){
            char *out = cJSON_Print(user);
            printf("%s\n",out);
            free(out);
            flag=1;
        }
    }//找到符合name用户并输出

    if(flag==0){
        printf("无法找到符合要求的客户!!\n");
        return;
    }

    //根据id删除用户
    int id;
    char Id[20];
    printf("请输入想要删除的正确用户id:");
    gets(Id);
    id = atoi(Id);
    for (int i = 0; i < size; i++)
    {
        user = cJSON_GetArrayItem(users,i);
        if(cJSON_GetObjectItem(user,"id")->valueint == id){
            cJSON_DeleteItemFromArray(users,i);
            printf("用户注销卡片成功!\n");
            break;
        }
    }

    // FILE *fp = fopen("user.json","w");
    // char *out = cJSON_Print(users);
    // fputs(out,fp);
    // fclose(fp);
    // free(out);
}

//初始化，加载用户
void loadUser(cJSON **users)
{
    // 打开文件
    FILE *file = NULL;
    file = fopen("user.json", "r");
    if (file == NULL) {
        *users = cJSON_CreateArray();
        return;
    }

    // 获得文件大小
    struct stat statbuf;
    stat("user.json", &statbuf);
    int fileSize = statbuf.st_size;


    // 分配符合文件大小的内存
    char *jsonStr = (char *)malloc(sizeof(char) * fileSize + 1);
    memset(jsonStr, 0, fileSize + 1);


    // 读取文件中的json字符串
    int size = fread(jsonStr, sizeof(char), fileSize, file);
    if (size == 0) {
        *users = cJSON_CreateArray();
        fclose(file);
        return;
    }

    // printf("%s\n", jsonStr);
    
    fclose(file);
    *users = cJSON_Parse(jsonStr);
    free(jsonStr);
}


//退出时保存用户到文件
void exitUser(cJSON *users)
{
    FILE *fp = fopen("user.json","w");
    char *out = cJSON_Print(users);
    fputs(out,fp);
    fclose(fp);
    free(out);
    cJSON_Delete(users);
    printf("退出并保存用户列表成功!\n");
}

//修改用户信息
void modifyUser(cJSON *users)
{
    //根据名字选择用户,并修改
    char name[20];
    printf("请输入你想要修改用户的姓名:");
    // gets(name);
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Remove newline character

    int size = cJSON_GetArraySize(users);
    cJSON *user = NULL;
    printf("以下是符合姓名的客户:\n");


    int flag =0;//判断是否找到用户
    for (int i = 0; i < size; i++)
    {
        user = cJSON_GetArrayItem(users,i);
        if(strcmp(cJSON_GetObjectItem(user,"name")->valuestring,name) == 0){
            char *out = cJSON_Print(user);
            printf("%s\n",out);
            free(out);
            flag=1;
        }
    }//找到符合name用户并输出
    if(flag==0){
        printf("无法找到符合要求的客户!!\n");
        return;
    }

    //根据id修改用户
    int id;
    char idstr[20];
    printf("请输入想要修改的正确用户id:");
    fgets(idstr,20,stdin);
    id = atoi(idstr);
    for (int i = 0; i < size; i++)
    {
        user = cJSON_GetArrayItem(users,i);
        if(cJSON_GetObjectItem(user,"id")->valueint == id){
            break;
        }
    }
    printf("1.name\n2.gender\n3.birthday\n4.phone\n5.effective_time\n6.application_time\n7.money\n");//选择要修改的属性
    printf("请输入你想要修改的属性对应的数字:");//输入要修改的属性数字
    char attribute[20];
    gets(attribute);
    int attr = atoi(attribute);
    printf("请输入你想要修改后的信息:");//输入修改后的信息
    char message[20];
    gets(message);
    switch (attr)
    {
    case 1:
        cJSON_ReplaceItemInObject(user,"name",cJSON_CreateString(message));
        break;
    case 2:
        if (strcmp(message,"男")){
            cJSON_ReplaceItemInObject(user,"gender",cJSON_CreateNumber(1));
        }else if (strcmp(message,"女")){
            cJSON_ReplaceItemInObject(user,"gender",cJSON_CreateNumber(0));
        }
        break;
    case 3:
        while(!validateDate(message) && message[0] != '\0'){
            printf("日期格式错误！请重新输入:");
            gets(message);
        }
        cJSON_ReplaceItemInObject(user,"birthday",cJSON_CreateString(message));
        break;
    case 4:
        while(strlen(message) != 11 && message[0] != '\0'){
            printf("手机号错误！请重新输入:");
            gets(message);
        }
        cJSON_ReplaceItemInObject(user,"phone",cJSON_CreateString(message));
        break;
    case 5:
        while(!validateDate(message) && message[0] != '\0'){
            printf("日期格式错误！请重新输入:");
            gets(message);
        }
        cJSON_ReplaceItemInObject(user,"effective_time",cJSON_CreateString(message));
        break;
    case 6:
        while(!validateDate(message) && message[0] != '\0'){
            printf("日期格式错误！请重新输入:");
            gets(message);
        }
        cJSON_ReplaceItemInObject(user,"application_time",cJSON_CreateString(message));
        break;
    case 7:
        cJSON_ReplaceItemInObject(user,"money",cJSON_CreateNumber(atof(message)));
        break;
    default:
        break;
    }

    // FILE *fp = fopen("user.json","w");
    // char *out = cJSON_Print(users);
    // fputs(out,fp);
    // fclose(fp);
    // free(out);
    printf("用户信息修改成功!\n");
}

void showUser(cJSON *users){
    //根据名字选择用户
    char name[20];
    printf("请输入你想要查询的用户姓名:");
    gets(name);
    int size = cJSON_GetArraySize(users);
    cJSON *user = NULL;
    printf("以下是符合姓名的用户:\n");


    int flag =0;//判断是否找到用户
    for (int i = 0; i < size; i++)
    {
        user = cJSON_GetArrayItem(users,i);
        if(strcmp(cJSON_GetObjectItem(user,"name")->valuestring,name) == 0){
            char *out = cJSON_Print(user);
            printf("%s\n",out);
            free(out);
            flag=1;
        }
    }
    if(flag==0){
        printf("无法找到符合要求的客户!!\n");
        return;
    }

    //根据id显示用户
    int id;
    char idstr[20];
    printf("请输入想要查询的正确用户id:");
    fgets(idstr,20,stdin);
    id = atoi(idstr);
    for (int i = 0; i < size; i++)
    {
        user = cJSON_GetArrayItem(users,i);
        if(cJSON_GetObjectItem(user,"id")->valueint == id){
            char *out =cJSON_Print(user);
            printf("%s\n",out);
            free(out);
            break;
        }
    }
}




// int main()
// {
//     cJSON *users = cJSON_CreateArray();
//     loadUser(&users);
//     // addUser(users);
//     // modifyUser(users);
//     DeleteUser(users);
//     showUser(users);
//     exitUser(users);
// }
