#pragma once
#include"Define.h"
#include"Global.h"
#include"Struct.h"	

//创建一个医生的排班表节点
ScheduleNode_Doctor* creat_schedule_node_Doc();

//  创建医生节点
DoctorNode* create_doctor_node();

// 给医生添加一天排班（尾插入队列）
void add_schedule_Doc(DoctorNode* doctor, DateNode date, int day_or_days);

//  增加医生
void add_doctor();

//  删除医生
void delete_doctor();

//  修改医生信息
void modify_doctor();

//  查询医生
void query_doctor();

// 释放医生链表以及医生排班表链表的内存
void free_doctor_list();
