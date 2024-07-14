#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<sys/stat.h>
#include"UserManage.h"
#include"cJSON.h"

//�ж�����
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

//�ж������Ƿ�Ϸ�
bool isValidDate(int year, int month, int day) {
    if (year < 0 || month < 1 || month > 12 || day < 1) {
        return false;
    }
    int daysInMonth[] = {31, 28 + isLeapYear(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return day <= daysInMonth[month - 1];
}

//������ڸ�ʽ
bool checkDateFormat(const char *date) {
     // ��鳤��
    if (strlen(date) != 10) {
        return false;
    }
    // ���ָ���
    if (date[4] != '.' || date[7] != '.') {
        return false;
    }
    // ����������Ƿ�Ϊ����
    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue; // �����ָ���
        if (!isdigit(date[i])) {
            return false;
        }
    }
    return true;
}

//��֤����
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

//����û�
void addUser(cJSON *users)
{
    User user;
    printf("�������û�����:");
    gets(user.name);//�������

    printf("�������û��Ա�(�� or Ů):");
    char gender[20];
    gets(gender);
    while (strcmp(gender,"��")!=0 && strcmp(gender,"Ů")!=0 && gender[0] != '\0'){//��Ϊ��
        printf("�����������������:");
        gets(gender);
    }
    if(strcmp(gender,"��")==0){
        user.gender = 1;
    }else
    {
        user.gender = 0;
    }//���ݡ��С���Ů������Ա�

    printf("�������û���������(yyyy.mm.dd):");
    gets(user.birthday);
    while(!validateDate(user.birthday) && user.birthday[0] != '\0'){
        printf("���ڸ�ʽ��������������:");
        gets(user.birthday);
    }//�������

    printf("�������û��ֻ���:");
    gets(user.phone);
    while(strlen(user.phone) != 11 && user.phone[0] != '\0'){
        printf("�ֻ��Ŵ�������������:");
        gets(user.phone);
    }//��ӵ绰����


    printf("�������û��쿨����(yyyy.mm.dd):");
    gets(user.application_time);
    while(!validateDate(user.application_time) && user.application_time[0] != '\0'){
        printf("���ڸ�ʽ��������������:");
        gets(user.application_time);
    }//��Ӱ쿨ʱ��

    printf("�������û���Ч����(yyyy.mm.dd):");
    gets(user.effective_time);
    while(!validateDate(user.effective_time)){
        printf("���ڸ�ʽ��������������:");
        gets(user.effective_time);
    }//�����Чʱ��

    printf("�������û���ֵ���:");
    char money[20];
    gets(money);
    user.money = atof(money);//��ӽ��

    //���id����ȡ��idΪ���һ��id+1
    int size=cJSON_GetArraySize(users);
    cJSON *temp = NULL;
    temp = cJSON_GetArrayItem(users,size-1);
    user.id = cJSON_GetObjectItem(temp,"id")->valueint+1;
    printf("���û���idΪ�� %d\n",user.id);


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
    //���浽�ļ�
    // FILE *fp = fopen("user.json","w");
    // char *out = cJSON_Print(users);
    // fputs(out,fp);
    // fclose(fp);
    // free(out);
    // cJSON_Delete(user_json);
    printf("�û��쿨�ɹ�!\n");
}

void DeleteUser(cJSON *users)
{
    //��������ѡ���û�,��ɾ��
    char name[20];
    printf("����������Ҫɾ���û�������:");
    gets(name);
    int size = cJSON_GetArraySize(users);
    cJSON *user = NULL;
    printf("�����Ƿ��������Ŀͻ�:\n");

    int flag =0;//�ж��Ƿ��ҵ��û�
    for (int i = 0; i < size; i++)
    {
        user = cJSON_GetArrayItem(users,i);
        if(strcmp(cJSON_GetObjectItem(user,"name")->valuestring,name) == 0){
            char *out = cJSON_Print(user);
            printf("%s\n",out);
            free(out);
            flag=1;
        }
    }//�ҵ�����name�û������

    if(flag==0){
        printf("�޷��ҵ�����Ҫ��Ŀͻ�!!\n");
        return;
    }

    //����idɾ���û�
    int id;
    char Id[20];
    printf("��������Ҫɾ������ȷ�û�id:");
    gets(Id);
    id = atoi(Id);
    for (int i = 0; i < size; i++)
    {
        user = cJSON_GetArrayItem(users,i);
        if(cJSON_GetObjectItem(user,"id")->valueint == id){
            cJSON_DeleteItemFromArray(users,i);
            printf("�û�ע����Ƭ�ɹ�!\n");
            break;
        }
    }

    // FILE *fp = fopen("user.json","w");
    // char *out = cJSON_Print(users);
    // fputs(out,fp);
    // fclose(fp);
    // free(out);
}

//��ʼ���������û�
void loadUser(cJSON **users)
{
    // ���ļ�
    FILE *file = NULL;
    file = fopen("user.json", "r");
    if (file == NULL) {
        *users = cJSON_CreateArray();
        return;
    }

    // ����ļ���С
    struct stat statbuf;
    stat("user.json", &statbuf);
    int fileSize = statbuf.st_size;


    // ��������ļ���С���ڴ�
    char *jsonStr = (char *)malloc(sizeof(char) * fileSize + 1);
    memset(jsonStr, 0, fileSize + 1);


    // ��ȡ�ļ��е�json�ַ���
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


//�˳�ʱ�����û����ļ�
void exitUser(cJSON *users)
{
    FILE *fp = fopen("user.json","w");
    char *out = cJSON_Print(users);
    fputs(out,fp);
    fclose(fp);
    free(out);
    cJSON_Delete(users);
    printf("�˳��������û��б�ɹ�!\n");
}

//�޸��û���Ϣ
void modifyUser(cJSON *users)
{
    //��������ѡ���û�,���޸�
    char name[20];
    printf("����������Ҫ�޸��û�������:");
    // gets(name);
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Remove newline character

    int size = cJSON_GetArraySize(users);
    cJSON *user = NULL;
    printf("�����Ƿ��������Ŀͻ�:\n");


    int flag =0;//�ж��Ƿ��ҵ��û�
    for (int i = 0; i < size; i++)
    {
        user = cJSON_GetArrayItem(users,i);
        if(strcmp(cJSON_GetObjectItem(user,"name")->valuestring,name) == 0){
            char *out = cJSON_Print(user);
            printf("%s\n",out);
            free(out);
            flag=1;
        }
    }//�ҵ�����name�û������
    if(flag==0){
        printf("�޷��ҵ�����Ҫ��Ŀͻ�!!\n");
        return;
    }

    //����id�޸��û�
    int id;
    char idstr[20];
    printf("��������Ҫ�޸ĵ���ȷ�û�id:");
    fgets(idstr,20,stdin);
    id = atoi(idstr);
    for (int i = 0; i < size; i++)
    {
        user = cJSON_GetArrayItem(users,i);
        if(cJSON_GetObjectItem(user,"id")->valueint == id){
            break;
        }
    }
    printf("1.name\n2.gender\n3.birthday\n4.phone\n5.effective_time\n6.application_time\n7.money\n");//ѡ��Ҫ�޸ĵ�����
    printf("����������Ҫ�޸ĵ����Զ�Ӧ������:");//����Ҫ�޸ĵ���������
    char attribute[20];
    gets(attribute);
    int attr = atoi(attribute);
    printf("����������Ҫ�޸ĺ����Ϣ:");//�����޸ĺ����Ϣ
    char message[20];
    gets(message);
    switch (attr)
    {
    case 1:
        cJSON_ReplaceItemInObject(user,"name",cJSON_CreateString(message));
        break;
    case 2:
        if (strcmp(message,"��")){
            cJSON_ReplaceItemInObject(user,"gender",cJSON_CreateNumber(1));
        }else if (strcmp(message,"Ů")){
            cJSON_ReplaceItemInObject(user,"gender",cJSON_CreateNumber(0));
        }
        break;
    case 3:
        while(!validateDate(message) && message[0] != '\0'){
            printf("���ڸ�ʽ��������������:");
            gets(message);
        }
        cJSON_ReplaceItemInObject(user,"birthday",cJSON_CreateString(message));
        break;
    case 4:
        while(strlen(message) != 11 && message[0] != '\0'){
            printf("�ֻ��Ŵ�������������:");
            gets(message);
        }
        cJSON_ReplaceItemInObject(user,"phone",cJSON_CreateString(message));
        break;
    case 5:
        while(!validateDate(message) && message[0] != '\0'){
            printf("���ڸ�ʽ��������������:");
            gets(message);
        }
        cJSON_ReplaceItemInObject(user,"effective_time",cJSON_CreateString(message));
        break;
    case 6:
        while(!validateDate(message) && message[0] != '\0'){
            printf("���ڸ�ʽ��������������:");
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
    printf("�û���Ϣ�޸ĳɹ�!\n");
}

void showUser(cJSON *users){
    //��������ѡ���û�
    char name[20];
    printf("����������Ҫ��ѯ���û�����:");
    gets(name);
    int size = cJSON_GetArraySize(users);
    cJSON *user = NULL;
    printf("�����Ƿ����������û�:\n");


    int flag =0;//�ж��Ƿ��ҵ��û�
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
        printf("�޷��ҵ�����Ҫ��Ŀͻ�!!\n");
        return;
    }

    //����id��ʾ�û�
    int id;
    char idstr[20];
    printf("��������Ҫ��ѯ����ȷ�û�id:");
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
