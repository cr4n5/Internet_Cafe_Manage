#ifndef USERMANAGE_H
#define USERMANAGE_H

#include "cJSON.h"

typedef struct User
{
    char name[20];
    int gender;//性别,0代表女，1代表男
    char birthday[20];//生日
    char phone[20];
    int id;
    char application_time[20];//办卡时间
    char effective_time[20];//有效时间 ，到几月几号
    double money;
    struct User *next;
} User;

void addUser(cJSON *users);
void DeleteUser(cJSON *users);
void loadUser(cJSON **users);
void modifyUser(cJSON *users);
void showUser(cJSON *users);
void exitUser(cJSON *users);

bool isLeapYear(int year);
bool isValidDate(int year, int month, int day);
bool checkDateFormat(const char *date);
bool validateDate(const char *date);

#endif