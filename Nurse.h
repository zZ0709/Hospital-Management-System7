#pragma once
#include"Define.h"
#include"Global.h"
#include"Struct.h"

//创建一个护士的排班表节点
ScheduleNode_Nurse* creat_schedule_node_Nurse();

//  创建护士节点
NurseNode* create_nurse_node();

// 给护士添加一天排班（尾插入队列，最新的放在队列尾）
void add_schedule_Nurse(NurseNode* nurse, Date date, int day_or_days);

//创建一个护士的病房节点
Nurse_ward_id* Creat_Ward_Id();

// 增加护士
void add_nurse();

//  删除护士
void delete_nurse();

//  修改护士
void modify_nurse();

// 查询护士
void query_nurse();

//  释放护士链表
void free_nurse_list();

