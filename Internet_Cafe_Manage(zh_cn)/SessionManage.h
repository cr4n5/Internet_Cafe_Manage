#ifndef SESSIONMANAGE_H
#define SESSIONMANAGE_H

#include "cJSON.h"

typedef struct Session
{
    int id;
    char name[20];
    int machine_id;
    char date[20];//当天日前
    char end_date[20];//结束日期
    char start_time[20];
    char end_time[20];
    int is_online;//1代表在线，0代表不在线
    double money;//结算金额
    int is_User;//1代表有用户，0代表没有用户 
    struct Session *next;
} Session;

void loadSession(cJSON **sessions);
void exitSession(cJSON *sessions);
void StartSession(cJSON *sessions,cJSON *users);
void EndSession(cJSON *sessions,cJSON *users);

bool isDayTime(const char *timeStr);
bool isMachineIdAvailable(int machine_id, int available_machine_id[], int size);
int calculate_hours(const char *start_date, const char *start_time, const char *end_date, const char *end_time);
void CalculateMoney(cJSON *session,cJSON *users,int id);

#endif