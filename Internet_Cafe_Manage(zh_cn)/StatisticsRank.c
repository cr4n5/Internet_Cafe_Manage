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
    int id;//未办卡用户id为-1
    char name[20]; 
    double money;
    struct JsonUser *next;
} JsonUser;

typedef struct Node {//对User排序
    int is_User;
    char name[20];
    double money;
    struct Node *next;
} Node;

//添加用户到JsonUser链表
void AddRankUser(JsonUser **head,char *name,double money,int id,int is_User){
    JsonUser *current=*head;//当前节点

    if (is_User==0){//添加每一个未办卡的用户
        JsonUser *newUser=(JsonUser *)malloc(sizeof(JsonUser));
        newUser->id=-1;
        strcpy(newUser->name,name);
        newUser->money=money;
        newUser->next=NULL;
        if(*head==NULL){//头节点为空
            *head=newUser;
        }else{
            while(current->next!=NULL){//找到尾节点
                current=current->next;
            }
            current->next=newUser;
        }
    }else{
        int flag=0;//标志是否已经存在此用户
        while(current!=NULL){//遍历链表
            if(current->id==id){
                current->money+=money;
                flag=1;
                break;
            }
            current=current->next;
        }
        if(flag==0){//若不存在此办卡用户
            JsonUser *newUser=(JsonUser *)malloc(sizeof(JsonUser));
            newUser->id=id;
            strcpy(newUser->name,name);
            newUser->money=money;
            newUser->next=NULL;
            if(*head==NULL){//头节点为空
                *head=newUser;
            }else{
                current=*head;
                while(current->next!=NULL){//找到尾节点
                    current=current->next;
                }
                current->next=newUser;
            }
        }
    }
}
    

//排序JsonUser，保存在Node链表中
void RankUser(Node **head,JsonUser **user){
    JsonUser *current=*user;
    while(current!=NULL){//遍历用户链表
        //初始化每一个newNode
        Node *newNode=(Node *)malloc(sizeof(Node));
        if(current->id==-1){//区分办卡用户与非办卡用户，1为办卡用户
            newNode->is_User=0;
        }else{
            newNode->is_User=1;
        }
        strcpy(newNode->name,current->name);
        newNode->money=current->money;
        newNode->next=NULL;

        if(*head==NULL){//头节点为空
            *head=newNode;
        }else{
            Node *pre=NULL;//前一个节点
            Node *p=*head;//当前节点
            while(p!=NULL && p->money>current->money){//找到插入位置
                pre=p;
                p=p->next;
            }
            if(pre==NULL){//插入头节点
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

//打印前十的客户
void printTopTenUsers(Node *head){
    int count=0;
    while(head!=NULL && count<10){
        printf("%d. %s %lf\n",++count,head->name,head->money);
        head=head->next;
    }
}

//打印前五的办卡用户
void printTopFiveCardUsers(Node *head){
    int CardCount=0;
    printf("消费前五名办卡用户：\n");
    while (head!=NULL && CardCount<5)
    {
        if(head->is_User==1){
            printf("%d. %s %lf\n",++CardCount,head->name,head->money);
        }
        head=head->next;
    }
}

//打印前五的临时用户
void printTopFiveNoCardUsers(Node *head){
    int NoCardCount=0;
    printf("消费前五名临时用户：\n");
    while (head!=NULL && NoCardCount<5)
    {
        if(head->is_User==0){
            printf("%d. %s %lf\n",++NoCardCount,head->name,head->money);
        }
        head=head->next;
    }
}

//打印今日上机人数和收入
void printPropleMoney(JsonUser *user){
    int people=0;
    double money=0;
    while(user!=NULL){
        people++;
        money+=user->money;
        user=user->next;
    }
    printf("今日上机人数：%d\n",people);
    printf("今日收入：%lf\n",money);
}

//打印今月上机人数和收入
void printPropleMoneyMonth(JsonUser *user){
    int people=0;//总人数
    int Cardpeople=0;//办卡用户人数
    int NoCardpeople=0;//临时用户人数
    double money=0;//总收入
    double CardMoney=0;//办卡用户收入
    double NoCardMoney=0;//临时用户收入
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
    printf("今月上机人数：%d\n",people);
    printf("今月收入：%lf\n",money);
    printf("今月办卡用户上机人数：%d\n",Cardpeople);
    printf("今月办卡用户收入：%lf\n",CardMoney);
    printf("今月临时用户上机人数：%d\n",NoCardpeople);
    printf("今月临时用户收入：%lf\n",NoCardMoney);
}

//判断yyyy.mm.dd是否相同
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

//判断yyyy.mm是否相同
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

//判断yyyy.mm格式是否正确
bool isMonthTime(const char *date){
    //检查长度是否正确
    if (strlen(date) != 7){
        return false;
    }
    //检查分隔符是否正确
    if (date[4] != '.'){
        return false;
    }

    //检查是否为数字
    for (int i = 0; i < 7; i++){
        if (i == 4){
            continue;
        }
        if (date[i] < '0' || date[i] > '9'){
            return false;
        }
    }
    //检查月份是否正确
    int year,month;
    sscanf(date,"%d.%d",&year,&month);
    if (month < 1 || month > 12){
        return false;
    }
    return true;
}

//释放排序链表
void freeNode(Node *head) {
    Node *current = head; // 当前节点设置为头节点
    Node *nextNode = NULL;

    while (current != NULL) {
        nextNode = current->next; // 保存下一个节点的指针
        free(current); // 释放当前节点
        current = nextNode; // 移动到下一个节点
    }
}

//释放用户链表
void freeJsonUser(JsonUser *head) {
    JsonUser *current = head; // 当前节点设置为头节点
    JsonUser *nextNode = NULL;

    while (current != NULL) {
        nextNode = current->next; // 保存下一个节点的指针
        free(current); // 释放当前节点
        current = nextNode; // 移动到下一个节点
    }
}

//统计日收入
void DayStatistics(cJSON *sessions){
    char date[20];
    printf("请输入统计的日期(yyyy.mm.dd):");
    gets(date);
    while(!validateDate(date)){
        printf("格式错误！请重新输入:");
        gets(date);
    }
    JsonUser *user = NULL;//用户链表
    int size=cJSON_GetArraySize(sessions);
    for (int i = 0; i < size; i++){//遍历sessions
        cJSON *session = cJSON_GetArrayItem(sessions,i);
        if (SameDay(cJSON_GetObjectItem(session,"date")->valuestring,date) && cJSON_GetObjectItem(session,"is_online")->valueint == 0){//需要选定日期并且下机
            char *name = cJSON_GetObjectItem(session,"name")->valuestring;
            double money = cJSON_GetObjectItem(session,"money")->valuedouble;
            AddRankUser(&user,name,money,cJSON_GetObjectItem(session,"id")->valueint,cJSON_GetObjectItem(session,"is_User")->valueint);//添加到用户链表
        }
    }

    Node *head = NULL;//排序链表
    RankUser(&head,&user);//排序
    printPropleMoney(user);//打印总人数和总收入
    printf("消费前十名用户：\n");
    printTopTenUsers(head);//打印前十的客户
    //释放内存
    freeNode(head);
    freeJsonUser(user);
}

void MonthStatistics(cJSON *sessions){
    char date[20];
    printf("请输入统计的月份(yyyy.mm):");
    gets(date);
    while(!isMonthTime(date)){
        printf("格式错误！请重新输入:");
        gets(date);
    }
    JsonUser *user = NULL;//用户链表
    int size=cJSON_GetArraySize(sessions);
    for (int i = 0; i < size; i++){//遍历sessions
        cJSON *session = cJSON_GetArrayItem(sessions,i);
        if (SameMonth(cJSON_GetObjectItem(session,"date")->valuestring,date) && cJSON_GetObjectItem(session,"is_online")->valueint == 0){//需要选定日期并且下机
            char *name = cJSON_GetObjectItem(session,"name")->valuestring;
            double money = cJSON_GetObjectItem(session,"money")->valuedouble;
            AddRankUser(&user,name,money,cJSON_GetObjectItem(session,"id")->valueint,cJSON_GetObjectItem(session,"is_User")->valueint);//添加到用户链表
        }
    }

    Node *head = NULL;//排序链表
    RankUser(&head,&user);//排序
    printPropleMoneyMonth(user);//打印总人数和总收入，办卡用户人数和收入，临时用户人数和收入
    printTopFiveCardUsers(head);//打印前五的办卡用户
    printTopFiveNoCardUsers(head);//打印前五的临时用户
    //释放内存
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