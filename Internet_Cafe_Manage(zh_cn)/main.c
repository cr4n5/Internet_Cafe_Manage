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
    //加载用户和会话信息
    cJSON *users=NULL;
    cJSON *sessions=NULL;
    loadUser(&users);
    loadSession(&sessions);

    printf("--------------------------------------\n");
    printf("-       欢迎使用网吧客户管理系统     -\n");
    printf("--------------------------------------\n");

    while(1){
        printf("\n");
        printf("--------------------------------------\n");
        printf("-              主菜单                -\n");
        printf("-           1.办卡服务               -\n");
        printf("-           2.注销卡片服务           -\n");
        printf("-           3.修改用户信息           -\n");
        printf("-           4.查看用户信息           -\n");
        printf("-           5.客户上网登记           -\n");
        printf("-           6.客户下网结算           -\n");
        printf("-           7.查看日统计额           -\n");
        printf("-           8.查看月统计额           -\n");
        printf("-           9.退出系统               -\n");
        printf("--------------------------------------\n");
        printf("请输入您的选择：");
        
        char ch[10];
        fgets(ch,10,stdin);
        choice=atoi(ch);
        while(choice<1||choice>9){
            printf("输入错误，请重新输入：");
            fgets(ch,10,stdin);
            choice=atoi(ch);
        }
        switch(choice){
            case 1:
                addUser(users);
                printf("按Enter回到菜单...");
                getchar();
                break;
            case 2:
                DeleteUser(users);
                printf("按Enter回到菜单...");
                getchar();
                break;
            case 3:
                modifyUser(users);
                printf("按Enter回到菜单...");
                getchar();
                break;
            case 4:
                showUser(users);
                printf("按Enter回到菜单...");
                getchar();
                break;
            case 5:
                StartSession(sessions,users);
                printf("按Enter回到菜单...");
                getchar();
                break;
            case 6:
                EndSession(sessions,users);
                printf("按Enter回到菜单...");
                getchar();
                break;
            case 7:
                DayStatistics(sessions);
                printf("按Enter回到菜单...");
                getchar();
                break;
            case 8:
                MonthStatistics(sessions);
                printf("按Enter回到菜单...");
                getchar();
                break;
            case 9:
                exitUser(users);
                exitSession(sessions);
                return 0;
        }
    }
}