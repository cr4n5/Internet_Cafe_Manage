#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<sys/stat.h>
#include"SessionManage.h"
#include"UserManage.h"
#include"cJSON.h"
#include<time.h>

//�ж�HH.mm��ʽ��ʱ���Ƿ�Ϸ�
bool isDayTime(const char *timeStr) {
    int hour, minute;
    char *dup = strdup(timeStr); // �����ַ����Ա����޸�ԭʼ�ַ���
    char *token = strtok(dup, "."); // �ָ�Сʱ����

    if (token == NULL) {
        free(dup);
        return false; // �ַ�����ʽ����ȷ
    }
    hour = atoi(token); // ת��СʱΪ����

    token = strtok(NULL, "."); // �ָ���Ӳ���
    if (token == NULL) {
        free(dup);
        return false; // �ַ�����ʽ����ȷ
    }
    minute = atoi(token); // ת������Ϊ����

    free(dup); // �ͷŸ��Ƶ��ַ���

    // У��Сʱ�ͷ��ӵķ�Χ
    if (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59) {
        return true;
    } else {
        return false;
    }
}

// ���machine_id�Ƿ���available_machine_id������
bool isMachineIdAvailable(int machine_id, int available_machine_id[], int size) {
    for (int i = 0; i < size; i++) {
        if (available_machine_id[i] == machine_id) {
            return false; // �ҵ���machine_id�������У�����false
        }
    }
    return true; // ������ϣ�û���ҵ���˵�����������У�����true
}

//�ж������Ƿ���Ч
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

//����
void StartSession(cJSON *sessions,cJSON *users)
{
    Session session;
    printf("����������:");
    gets(session.name);
    int size = cJSON_GetArraySize(users);
    cJSON *user = NULL;
    printf("�����Ƿ��������Ŀͻ�:\n");

     //����name��ʾ�����û�
    int flag = 0;//�ж��û��Ƿ��Ѿ��쿨
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
    int allid=5000;//δ�쿨�û���ʼid
    int available_machine_id[size];//æµ����λ��
    int j=0;
    for (int i = 0; i < size; i++)
    {
        cJSON *session = cJSON_GetArrayItem(sessions,i);
        bool is_online = cJSON_GetObjectItem(session,"is_online")->valueint;
        if(cJSON_GetObjectItem(session,"is_User")->valueint == 0 && is_online){
            allid = cJSON_GetObjectItem(session,"id")->valueint;
        }//�ҵ�δ�쿨�û��Ŀ��õ�id

        if(is_online){
            available_machine_id[j] = cJSON_GetObjectItem(session,"machine_id")->valueint;
            j++;
        }//�ҵ�æµ����λ��
    }
    size = j;
    if(flag == 0){//δ�쿨�û�
        printf("�������ͻ�������!\n");
        session.id = allid -1;
        session.is_User = 0;
        printf("������ʱ�ͻ������û�idΪ��%d\n",session.id);
    }else{//�Ѱ쿨�û�,ѡ��id
        char Id[20];
        printf("��������ȷ���û�id:");
        gets(Id);
        session.id = atoi(Id);
        session.is_User = 1;
    }
    
    char machine_id[20];
    //ѡ�����,������Ƿ����
    do
    {
        printf("��������������λ�ţ�ȷ������δ��ʹ�ã�:");
        gets(machine_id);
    } while (!isMachineIdAvailable(atoi(machine_id), available_machine_id, size));
    session.machine_id = atoi(machine_id);

    //��ȡid��Ӧ���û�����Чʱ��
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
    //���뿪ʼ���ں�ʱ��
    char date[20];
    printf("������������ʼ������(yyyy.mm.dd):");
    gets(date);
    while(!validateDate(date)){
        printf("��ʽ��������������:");
        gets(date);
    }
    if (session.is_User == 1 && !iseffective_date(date,effective_time)){//��֤�û���Чʱ���Ƿ����
        printf("���û���Ч�ڹ���!\n");
        printf("���ӳ���Ч����:");
        char new_effective_time[20];
        gets(new_effective_time);
        while(!validateDate(new_effective_time) || !iseffective_date(date,new_effective_time)){
            printf("��ʽ����򲻷�����Ч��Ҫ������������:");
            gets(new_effective_time);
        }
        cJSON_ReplaceItemInObject(user,"effective_time",cJSON_CreateString(new_effective_time));
    }
    strcpy(session.date,date);

    do{
        printf("�����뿪ʼʱ��(HH.mm):");
        gets(date);
    }while(!isDayTime(date));
    strcpy(session.start_time,date);
    
    //����������ʾ
    session.is_online = 1;

    //д��Ự
    cJSON *session_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(session_json, "id", session.id);
    cJSON_AddStringToObject(session_json, "name", session.name);
    cJSON_AddNumberToObject(session_json, "machine_id", session.machine_id);
    cJSON_AddStringToObject(session_json, "date", session.date);
    cJSON_AddStringToObject(session_json, "start_time", session.start_time);
    cJSON_AddNumberToObject(session_json, "is_online", session.is_online);
    cJSON_AddNumberToObject(session_json, "is_User", session.is_User);
    cJSON_AddItemToArray(sessions, session_json);

    printf("�û������Ǽǳɹ�!\n");
}

// ������������ʱ��֮���Сʱ��
int calculate_hours(const char *start_date, const char *start_time, const char *end_date, const char *end_time) {
    struct tm start_tm = {0}, end_tm = {0};
    int start_year, start_month, start_day, start_hour, start_minute;
    int end_year, end_month, end_day, end_hour, end_minute;

    // ������ʼ���ں�ʱ��
    sscanf(start_date, "%d.%d.%d", &start_year, &start_month, &start_day);
    sscanf(start_time, "%d.%d", &start_hour, &start_minute);

    // �����������ں�ʱ��
    sscanf(end_date, "%d.%d.%d", &end_year, &end_month, &end_day);
    sscanf(end_time, "%d.%d", &end_hour, &end_minute);

    // ���struct tm�ṹ��
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

    // ��struct tmת��Ϊtime_t
    time_t start_time_t = mktime(&start_tm);
    time_t end_time_t = mktime(&end_tm);

    // ������첢ת��ΪСʱ
    double diff = difftime(end_time_t, start_time_t);
    int hours = diff / 3600;
    if ((int)diff % 3600 > 0) {
        hours += 1; // δ��һСʱ��һСʱ����
    }

    return hours;
}

//������
void CalculateMoney(cJSON *session,cJSON *users,int id){
    //����ʱ��
    int hours = calculate_hours(cJSON_GetObjectItem(session,"date")->valuestring,cJSON_GetObjectItem(session,"start_time")->valuestring,cJSON_GetObjectItem(session,"end_date")->valuestring,cJSON_GetObjectItem(session,"end_time")->valuestring);
    printf("���û�����ʱ��Ϊ�� %d\n",hours);
    if (cJSON_GetObjectItem(session,"is_User")->valueint==0){//δ�쿨�û�
        double money = 0;
        if(hours<8){
            money = 2*hours;
        }else{
            money = 2*hours*0.9;
        }
        printf("���û�����Ӧ֧���� %lf\n",money);
        cJSON_AddNumberToObject(session,"money",money);
    }else{//�Ѱ쿨�û�
        double money = hours*2*0.8;
        printf("���û�����Ӧ֧�� %lf\n",money);
        cJSON_AddNumberToObject(session,"money",money);
        int size = cJSON_GetArraySize(users);
        cJSON *user = NULL;
        for (int i = 0; i < size; i++)
        {
            user = cJSON_GetArrayItem(users,i);
            if(cJSON_GetObjectItem(user,"id")->valueint == id){
                double ori_money = cJSON_GetObjectItem(user,"money")->valuedouble;
                if (ori_money < money){//����
                    printf("�û���������!\n");
                    printf("���ֵ�������ֵ�� :");
                    char recharge[20];
                    gets(recharge);
                    double add_money = atof(recharge);
                    while(ori_money+add_money < money){//��ֵ�����Ȼ����
                        printf("�����Ȼ���㣡�����������ֵ��:");
                        gets(recharge);
                        add_money = atof(recharge);
                    }
                    cJSON_ReplaceItemInObject(user,"money",cJSON_CreateNumber(ori_money+add_money-money));
                }else{
                    cJSON_ReplaceItemInObject(user,"money",cJSON_CreateNumber(ori_money-money));//�ۿ�
                }
            }
        }
    
    }
}

//��������
void EndSession(cJSON *sessions,cJSON *users)
{
    char Id[20];
    printf("�������û�id:");
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
    }//�ҵ�id��Ӧ��session
    if (flag==0){
        printf("��id�����ڻ���û���δ����\n");
        return;
    }else{//������������д����ʱ�䣬������
        char end_date[20];
        do{
            printf("������������ڣ�ȷ����ʽ��ȷ��(yyyy.mm.dd):");
            gets(end_date);
        }while(!validateDate(end_date));
        cJSON_AddStringToObject(session,"end_date",end_date);
        char end_time[20];
        do{
            printf("���������ʱ�䣬ȷ����ʽ��ȷ��(HH.mm)):");
            gets(end_time);
        }while(!isDayTime(end_time));
        cJSON_AddStringToObject(session,"end_time",end_time);
        CalculateMoney(session,users,id);
        printf("�ͻ���������ɹ�!\n");
    }
}

//�˳���д���ļ�
void exitSession(cJSON *sessions)
{
    FILE *fp = fopen("sessions.json","w");
    char *out = cJSON_Print(sessions);
    fputs(out,fp);
    fclose(fp);
    free(out);
    cJSON_Delete(sessions);
    printf("�˳�������Ự�б�ɹ�!\n");
}

//��ʼ��������session
void loadSession(cJSON **sessions)
{
    // ���ļ�
    FILE *file = NULL;
    file = fopen("sessions.json", "r");
    if (file == NULL) {
        *sessions = cJSON_CreateArray();
        return;
    }

    // ����ļ���С
    struct stat statbuf;
    stat("sessions.json", &statbuf);
    int fileSize = statbuf.st_size;


    // ��������ļ���С���ڴ�
    char *jsonStr = (char *)malloc(sizeof(char) * fileSize + 1);
    memset(jsonStr, 0, fileSize + 1);


    // ��ȡ�ļ��е�json�ַ���
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