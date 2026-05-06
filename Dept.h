#pragma once
#include"Struct.h"
#include"Tool.h"

//初始化6个科室的链表
Dept* init_dept_list();

//  创建科室节点
Dept* create_dept_node();

//把该医生存入对应科室的医生表中
void add_doctor_to_dept(Dept* dept_list, DoctorNode* doctor);

//删除一个科室的医生表中的一个医生节点
void delete_doctor_from_dept(Dept* dept_list, DoctorNode* doctor);

//释放科室链表
void free_dept_list(Dept* head);