#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<sys/stat.h>
#include"SessionManage.h"
#include"UserManage.h"
#include"StatisticsRank.h"
#include"cJSON.h"
#include<time.h>

int main(){
    int choice;
    //�����û��ͻỰ��Ϣ
    cJSON *users=NULL;
    cJSON *sessions=NULL;
    loadUser(&users);
    loadSession(&sessions);

    printf("--------------------------------------\n");
    printf("-       ��ӭʹ�����ɿͻ�����ϵͳ     -\n");
    printf("--------------------------------------\n");

    while(1){
        printf("\n");
        printf("--------------------------------------\n");
        printf("-              ���˵�                -\n");
        printf("-           1.�쿨����               -\n");
        printf("-           2.ע����Ƭ����           -\n");
        printf("-           3.�޸��û���Ϣ           -\n");
        printf("-           4.�鿴�û���Ϣ           -\n");
        printf("-           5.�ͻ������Ǽ�           -\n");
        printf("-           6.�ͻ���������           -\n");
        printf("-           7.�鿴��ͳ�ƶ�           -\n");
        printf("-           8.�鿴��ͳ�ƶ�           -\n");
        printf("-           9.�˳�ϵͳ               -\n");
        printf("--------------------------------------\n");
        printf("����������ѡ��");
        
        char ch[10];
        fgets(ch,10,stdin);
        choice=atoi(ch);
        while(choice<1||choice>9){
            printf("����������������룺");
            fgets(ch,10,stdin);
            choice=atoi(ch);
        }
        switch(choice){
            case 1:
                addUser(users);
                printf("��Enter�ص��˵�...");
                getchar();
                break;
            case 2:
                DeleteUser(users);
                printf("��Enter�ص��˵�...");
                getchar();
                break;
            case 3:
                modifyUser(users);
                printf("��Enter�ص��˵�...");
                getchar();
                break;
            case 4:
                showUser(users);
                printf("��Enter�ص��˵�...");
                getchar();
                break;
            case 5:
                StartSession(sessions,users);
                printf("��Enter�ص��˵�...");
                getchar();
                break;
            case 6:
                EndSession(sessions,users);
                printf("��Enter�ص��˵�...");
                getchar();
                break;
            case 7:
                DayStatistics(sessions);
                printf("��Enter�ص��˵�...");
                getchar();
                break;
            case 8:
                MonthStatistics(sessions);
                printf("��Enter�ص��˵�...");
                getchar();
                break;
            case 9:
                exitUser(users);
                exitSession(sessions);
                return 0;
        }
    }
}