# Internet_Cafe_Manage

HDU杭电程序设计课程设计-网吧管理系统

## 目的

设计一个网吧客户管理程序，实现临时客户和办卡客户的上网登记管理、收费管理。网吧按照每小时2元收费。对办卡客户，享受8折优惠。临时客户连续上网小于8小时，则每小时2元，超过8小时，打9折。

## 功能要求

（1）客户办卡服务：对办卡客户信息进行管理。客户信息主要包括：姓名，性别，出生日期，手机号，卡号，卡有效时间，办卡日期，充值金额。支持客户信息添加、删除、修改、查询，以及用户充值。
（2）客户上网登记：对办卡客户，登记其卡号、使用机器位置、上网日期、开始上网时间；对临时客户，分配临时卡号，并登记使用机器位置、上网日期、开始上网时间；
（3）客户下网结算：对办卡客户，根据卡号和上网时间，查询上网记录，登记下网时间，结算费用；根据卡号，查询客户信息，从充值卡上扣除上网费用。
（4）输入指定日期，统计输出网吧营业额，并给出消费前十名客户的姓名。
（5）输入指定月份，提供按月对办卡客户、临时客户的消费金额统计，并排序输出。

## 总体设计

![alt text](/docs/image/image.png)

1.基本思路和设计思想：
（1）使用面向对象的方法，用户管理类、上网管理类和统计排序类。
（2）设计主控制器main，负责协调各功能模块之间的交互。
（3）设计合适的数据结构（cJSON）存储客户信息、办卡信息和上网记录。

2.功能模块划分图及函数关系：
（1）主控制器（包括客户管理、上网管理、统计排序等方法）
（2）用户卡类（包括充值卡信息的增删改查、扣费、充值等功能）
（3）上网记录类（记录客户信息、上网信息、费用计算等）

## 数据及数据结构设计描述

```c
cJSON *sessions=NULL;
{
        "id":   1,
        "name": "halo",
        "machine_id":   90,
        "date": "2001.10.10",
        "start_time":   "09.09",
        "is_online":    0,
        "is_User":  1,
        "end_date": "2001.10.11",
        "end_time": "09.10",
        "money":    40
}
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

cJSON *users=NULL;
{
        "id":   11,
        "name": "crane",
        "gender":   1,
        "birthday": "2004.10.10",
        "phone":    "18888888888",
        "effective_time":   "2023.10.10",
        "application_time": "2022.10.10",
        "money":    73.4
}
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

//统计排序
typedef struct JsonUser {//将json转入链表
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

```

程序中主要变量为cJSON *sessions ， cJSON *users。保存文件为json
在统计排序中，处理数据所用的为struct JsonUser，struct Node。

## 运行

```shell
./main
```

or

click the main.exe
