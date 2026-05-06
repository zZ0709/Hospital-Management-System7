// 全局变量（链表头指针）
#pragma once
#include"Struct.h"
#include<stdlib.h>

extern DoctorNode* doctor_head;   // 医生链表头
extern NurseNode* nurse_head ;     // 护士链表头
extern int doctor_count ;             // 医生数量统计
extern int nurse_count ;              // 护士数量统计
extern Dept* dept_list;              // 科室链表头指针