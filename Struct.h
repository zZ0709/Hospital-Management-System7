//结构体定义
#pragma once
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "Define.h"

//日期节点
typedef struct {
    int year;//年
    int month;//月
    int day;//日
}DateNode;
 

//医生这个月的排班表，链表形式，每个节点存储一天的日期和是否上班
typedef struct ScheduleNode_Doctor {
    DateNode date;               // 日期
	int treat_count;          // 这一天接诊了几个人，挂了几个号（默认0，接诊一次加1，最多20人次）
    struct ScheduleNode_Doctor* next; // 队列指针
} ScheduleNode_Doctor;


// 医生链表节点
typedef struct  Doctor {
    //五大基本信息
    char id[10];        // 工号（唯一）    例：01（科室）+00（编号）
    char name[20];      // 姓名    例：zhangsan
    int level;          // 级别   （共六级）
    double salary;      // 薪资    (精确到分)
    int dept;           // 科室 1-6// 1.内科 internal  2. 外科 surgery  3.儿科 child  4. 妇科 gynecology  5. 急诊 emergency  6. 全科 general

    int this_day_treat_number;            //今天叫几个号了 ，时刻更新记录，最多20个
    
    ScheduleNode_Doctor* schedule_head;  // 队列头
    ScheduleNode_Doctor* schedule_tail;  // 队列尾

    struct Doctor * next;  // 指向下一个节点

} DoctorNode;


//用于科室中记录医生id的链表节点
typedef struct Doctor_Id {
    char id[10];
    struct Doctor_Id* next;
}Doctor_Id;


//科室链表节点 仅供查询医护       共六个科室
typedef struct Dept {
    int doct_count;      //该科室医生数量
    Doctor_Id* Dept_doctor_head;  //每个科室最多10个医生
    struct Dept* next;             //指向下一个节点
}Dept;



//护士负责的病房id链表
typedef struct Nurse_ward_id {
    char wards_id[8];
   struct  Nurse_ward_id* next;
}Nurse_ward_id;


//护士这个月的排班表，链表形式，每个节点存储一天的日期和是否上班
typedef struct ScheduleNode_Nurse {
    DateNode date;               // 日期
	int Is_work;          //    0:不上班 1:上班
    struct ScheduleNode_Nurse* next; // 队列指针
} ScheduleNode_Nurse;


// 护士链表节点
typedef struct Nurse {      
    //五大基本信息
    char id[10];        // 工号（唯一）例: 01（病房类型共三种：普通，急诊，icu）+00（编号）
    char name[20];      // 姓名
    int level;          // 级别   （共两级）
    double salary;         // 薪资
    Nurse_ward_id * ward_head_id  ;   //记录护士负责的病房id的头节点

    int ward_number;    //负责的病房数量
   
    ScheduleNode_Nurse* schedule_head;  // 队列头
    ScheduleNode_Nurse* schedule_tail;  // 队列尾

    struct  Nurse * next;   // 指向下一个节点
} NurseNode;



