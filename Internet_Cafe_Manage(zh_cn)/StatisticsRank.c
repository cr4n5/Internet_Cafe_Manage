#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<sys/stat.h>
#include"SessionManage.h"
#include"UserManage.h"
#include"cJSON.h"
#include<time.h>

typedef struct JsonUser {
    int id;//δ�쿨�û�idΪ-1
    char name[20]; 
    double money;
    struct JsonUser *next;
} JsonUser;

typedef struct Node {//��User����
    int is_User;
    char name[20];
    double money;
    struct Node *next;
} Node;

//����û���JsonUser����
void AddRankUser(JsonUser **head,char *name,double money,int id,int is_User){
    JsonUser *current=*head;//��ǰ�ڵ�

    if (is_User==0){//���ÿһ��δ�쿨���û�
        JsonUser *newUser=(JsonUser *)malloc(sizeof(JsonUser));
        newUser->id=-1;
        strcpy(newUser->name,name);
        newUser->money=money;
        newUser->next=NULL;
        if(*head==NULL){//ͷ�ڵ�Ϊ��
            *head=newUser;
        }else{
            while(current->next!=NULL){//�ҵ�β�ڵ�
                current=current->next;
            }
            current->next=newUser;
        }
    }else{
        int flag=0;//��־�Ƿ��Ѿ����ڴ��û�
        while(current!=NULL){//��������
            if(current->id==id){
                current->money+=money;
                flag=1;
                break;
            }
            current=current->next;
        }
        if(flag==0){//�������ڴ˰쿨�û�
            JsonUser *newUser=(JsonUser *)malloc(sizeof(JsonUser));
            newUser->id=id;
            strcpy(newUser->name,name);
            newUser->money=money;
            newUser->next=NULL;
            if(*head==NULL){//ͷ�ڵ�Ϊ��
                *head=newUser;
            }else{
                current=*head;
                while(current->next!=NULL){//�ҵ�β�ڵ�
                    current=current->next;
                }
                current->next=newUser;
            }
        }
    }
}
    

//����JsonUser��������Node������
void RankUser(Node **head,JsonUser **user){
    JsonUser *current=*user;
    while(current!=NULL){//�����û�����
        //��ʼ��ÿһ��newNode
        Node *newNode=(Node *)malloc(sizeof(Node));
        if(current->id==-1){//���ְ쿨�û���ǰ쿨�û���1Ϊ�쿨�û�
            newNode->is_User=0;
        }else{
            newNode->is_User=1;
        }
        strcpy(newNode->name,current->name);
        newNode->money=current->money;
        newNode->next=NULL;

        if(*head==NULL){//ͷ�ڵ�Ϊ��
            *head=newNode;
        }else{
            Node *pre=NULL;//ǰһ���ڵ�
            Node *p=*head;//��ǰ�ڵ�
            while(p!=NULL && p->money>current->money){//�ҵ�����λ��
                pre=p;
                p=p->next;
            }
            if(pre==NULL){//����ͷ�ڵ�
                newNode->next=*head;
                *head=newNode;
            }else{
                pre->next=newNode;
                newNode->next=p;
            }
        }
        current=current->next;
    }
}

//��ӡǰʮ�Ŀͻ�
void printTopTenUsers(Node *head){
    int count=0;
    while(head!=NULL && count<10){
        printf("%d. %s %lf\n",++count,head->name,head->money);
        head=head->next;
    }
}

//��ӡǰ��İ쿨�û�
void printTopFiveCardUsers(Node *head){
    int CardCount=0;
    printf("����ǰ�����쿨�û���\n");
    while (head!=NULL && CardCount<5)
    {
        if(head->is_User==1){
            printf("%d. %s %lf\n",++CardCount,head->name,head->money);
        }
        head=head->next;
    }
}

//��ӡǰ�����ʱ�û�
void printTopFiveNoCardUsers(Node *head){
    int NoCardCount=0;
    printf("����ǰ������ʱ�û���\n");
    while (head!=NULL && NoCardCount<5)
    {
        if(head->is_User==0){
            printf("%d. %s %lf\n",++NoCardCount,head->name,head->money);
        }
        head=head->next;
    }
}

//��ӡ�����ϻ�����������
void printPropleMoney(JsonUser *user){
    int people=0;
    double money=0;
    while(user!=NULL){
        people++;
        money+=user->money;
        user=user->next;
    }
    printf("�����ϻ�������%d\n",people);
    printf("�������룺%lf\n",money);
}

//��ӡ�����ϻ�����������
void printPropleMoneyMonth(JsonUser *user){
    int people=0;//������
    int Cardpeople=0;//�쿨�û�����
    int NoCardpeople=0;//��ʱ�û�����
    double money=0;//������
    double CardMoney=0;//�쿨�û�����
    double NoCardMoney=0;//��ʱ�û�����
    while(user!=NULL){
        if(user->id==-1){
            NoCardpeople++;
            NoCardMoney+=user->money;
        }else{
            Cardpeople++;
            CardMoney+=user->money;
        }
        people++;
        money+=user->money;
        user=user->next;
    }
    printf("�����ϻ�������%d\n",people);
    printf("�������룺%lf\n",money);
    printf("���°쿨�û��ϻ�������%d\n",Cardpeople);
    printf("���°쿨�û����룺%lf\n",CardMoney);
    printf("������ʱ�û��ϻ�������%d\n",NoCardpeople);
    printf("������ʱ�û����룺%lf\n",NoCardMoney);
}

//�ж�yyyy.mm.dd�Ƿ���ͬ
bool SameDay(const char *date1,const char *date2){
    int year1,month1,day1;
    int year2,month2,day2;
    sscanf(date1,"%d.%d.%d",&year1,&month1,&day1);
    sscanf(date2,"%d.%d.%d",&year2,&month2,&day2);
    if (year1 == year2 && month1 == month2 && day1 == day2){
        return true;
    }
    return false;
}

//�ж�yyyy.mm�Ƿ���ͬ
bool SameMonth(const char *date1,const char *date2){
    int year1,month1,day1;
    int year2,month2;
    sscanf(date1,"%d.%d.%d",&year1,&month1, &day1);
    sscanf(date2,"%d.%d",&year2,&month2);
    if (year1 == year2 && month1 == month2){
        return true;
    }
    return false;
}

//�ж�yyyy.mm��ʽ�Ƿ���ȷ
bool isMonthTime(const char *date){
    //��鳤���Ƿ���ȷ
    if (strlen(date) != 7){
        return false;
    }
    //���ָ����Ƿ���ȷ
    if (date[4] != '.'){
        return false;
    }

    //����Ƿ�Ϊ����
    for (int i = 0; i < 7; i++){
        if (i == 4){
            continue;
        }
        if (date[i] < '0' || date[i] > '9'){
            return false;
        }
    }
    //����·��Ƿ���ȷ
    int year,month;
    sscanf(date,"%d.%d",&year,&month);
    if (month < 1 || month > 12){
        return false;
    }
    return true;
}

//�ͷ���������
void freeNode(Node *head) {
    Node *current = head; // ��ǰ�ڵ�����Ϊͷ�ڵ�
    Node *nextNode = NULL;

    while (current != NULL) {
        nextNode = current->next; // ������һ���ڵ��ָ��
        free(current); // �ͷŵ�ǰ�ڵ�
        current = nextNode; // �ƶ�����һ���ڵ�
    }
}

//�ͷ��û�����
void freeJsonUser(JsonUser *head) {
    JsonUser *current = head; // ��ǰ�ڵ�����Ϊͷ�ڵ�
    JsonUser *nextNode = NULL;

    while (current != NULL) {
        nextNode = current->next; // ������һ���ڵ��ָ��
        free(current); // �ͷŵ�ǰ�ڵ�
        current = nextNode; // �ƶ�����һ���ڵ�
    }
}

//ͳ��������
void DayStatistics(cJSON *sessions){
    char date[20];
    printf("������ͳ�Ƶ�����(yyyy.mm.dd):");
    gets(date);
    while(!validateDate(date)){
        printf("��ʽ��������������:");
        gets(date);
    }
    JsonUser *user = NULL;//�û�����
    int size=cJSON_GetArraySize(sessions);
    for (int i = 0; i < size; i++){//����sessions
        cJSON *session = cJSON_GetArrayItem(sessions,i);
        if (SameDay(cJSON_GetObjectItem(session,"date")->valuestring,date) && cJSON_GetObjectItem(session,"is_online")->valueint == 0){//��Ҫѡ�����ڲ����»�
            char *name = cJSON_GetObjectItem(session,"name")->valuestring;
            double money = cJSON_GetObjectItem(session,"money")->valuedouble;
            AddRankUser(&user,name,money,cJSON_GetObjectItem(session,"id")->valueint,cJSON_GetObjectItem(session,"is_User")->valueint);//��ӵ��û�����
        }
    }

    Node *head = NULL;//��������
    RankUser(&head,&user);//����
    printPropleMoney(user);//��ӡ��������������
    printf("����ǰʮ���û���\n");
    printTopTenUsers(head);//��ӡǰʮ�Ŀͻ�
    //�ͷ��ڴ�
    freeNode(head);
    freeJsonUser(user);
}

void MonthStatistics(cJSON *sessions){
    char date[20];
    printf("������ͳ�Ƶ��·�(yyyy.mm):");
    gets(date);
    while(!isMonthTime(date)){
        printf("��ʽ��������������:");
        gets(date);
    }
    JsonUser *user = NULL;//�û�����
    int size=cJSON_GetArraySize(sessions);
    for (int i = 0; i < size; i++){//����sessions
        cJSON *session = cJSON_GetArrayItem(sessions,i);
        if (SameMonth(cJSON_GetObjectItem(session,"date")->valuestring,date) && cJSON_GetObjectItem(session,"is_online")->valueint == 0){//��Ҫѡ�����ڲ����»�
            char *name = cJSON_GetObjectItem(session,"name")->valuestring;
            double money = cJSON_GetObjectItem(session,"money")->valuedouble;
            AddRankUser(&user,name,money,cJSON_GetObjectItem(session,"id")->valueint,cJSON_GetObjectItem(session,"is_User")->valueint);//��ӵ��û�����
        }
    }

    Node *head = NULL;//��������
    RankUser(&head,&user);//����
    printPropleMoneyMonth(user);//��ӡ�������������룬�쿨�û����������룬��ʱ�û�����������
    printTopFiveCardUsers(head);//��ӡǰ��İ쿨�û�
    printTopFiveNoCardUsers(head);//��ӡǰ�����ʱ�û�
    //�ͷ��ڴ�
    freeNode(head);
    freeJsonUser(user);

}

// int main(){
//     cJSON *users = NULL;
//     loadUser(&users);
//     cJSON *sessions = NULL;
//     loadSession(&sessions);
    
//     // DayStatistics(sessions);//2001.10.11
//     MonthStatistics(sessions);//2001.10

//     exitSession(sessions);
//     exitUser(users);
//     return 0;
// }