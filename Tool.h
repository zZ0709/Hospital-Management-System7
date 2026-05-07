// 工具函数
#pragma once

#include <stdio.h>
#include<ctype.h>
#include <stdbool.h>
#include "Define.h"
#include "Struct.h"

//  获取级别名称（医生）
const char* get_doctor_level_name(int level);

//  获取级别名称（护士）
const char* get_nurse_level_name(int level);

//	清空输入缓存区
void clear_input();

//检查医护id是否存在		 ID 查重工具
int check_doctor_or_nurse_id_exists(char* id, const char* p);

//输入保护——判断是否输入为纯数字
int IsPureDigit(const char* str);

//输入保护——判断是否输入为纯英文字母
int IsPureEng(const char* str);

//输入保护——判断是否输入为单个数字   功能：读取菜单选项，严格限制只能输入【单个数字 0~9】   返回值：成功返回数字 0~9，失败返回 -1
int Single_number_input();

//输入保护——医生护士和病房id
// 功能：安全输入医生 / 护士 / 病房的 4 位数字 ID
// check 参数含义：
//      0 = 新增医生/护士时用 → 检查 ID 是否已经存在（不能重复）
//      1 = 只检查格式，不检查是否存在（如添加病房ID）
//      2 = 修改/删除时用 → 检查 ID 必须存在（不存在不让过）
// temp_id：传入字符数组，用来存储输入的ID
// 返回值：返回合法的 ID 字符串
//ward_id输入时，check传1，type传"ward"，不检查是否存在(此工作不在此处负责)；医生护士id输入时，check传0或2，type传"doctor"或"nurse"，分别检查是否存在
char* Id_Input(const int check, char* temp_id,const char * type);

//医生id合法性检查		合法返回1，不合法返回0
int recheck_D(const char* id);

//护士id合法性检查	合法返回1，不合法返回0
int recheck_N(const char* id);

//输入保护——医生护士姓名			最多输入20个字符，必须是纯英文字母
char * Name_Input(const char * str,char * temp_name);

//输入保护——单数字		必须是单个数字，且在指定范围内（如2-4），用于读取菜单选项等
int Num_Input(int begin,int end,const char *str);		

//输入保护——医生薪资（double数）		  必须是合法的正数 / 小数，才能通过自动精确到分，输入 8000.123 ， 自动变成 8000.12
double Salary_Input(const char* str);

//完整展示医生信息		展示5大基本信息 + 是否出诊 + 今日接诊数 + 一周上班天数 + 叫号记录
void Show_Doctor(DoctorNode* p);

//完整展示护士信息	展示5大基本信息 + 是否出诊 + 一周上班天数 + 负责病房数量 
void Show_Nurse(NurseNode* p);

//通过输入编号查找医生		功能：根据用户输入的ID，查找并返回对应的医生节点
DoctorNode* Find_Doctor_id();

//通过输入编号查找医生(查找过程)  
DoctorNode* get_Doctor_id(const char* id);

//通过输入编号查找护士		功能：根据用户输入的ID，查找并返回对应的护士节点
NurseNode* Find_Nurse_id();

//通过输入编号查找护士(查找过程)
NurseNode* get_Nurse_id(const char* id);

//医生的排班函数		
int schedule_Doc(DoctorNode* node,int Is_add);

//护士的排班函数
int schedule_Nurse(NurseNode* node, int Is_add);

//安全输入0到30之间的整数，返回值为输入的整数
int Num_0_30_Input(const char* str);

//安全输入0到20之间的整数，返回值为输入的整数
int Num_0_20_Input(const char* str);

//临时day_input
Date Day_input(Date* day);

//创建一个DateNode
Date* Create_DateNode();

//医生薪资管理
void Salary_Doc_Management();

//护士薪资管理
void Salary_Nurse_Management();

//医生薪资增减函数
void Doc_Salary_Adjustment(DoctorNode* doctor, double modify_salary);

//护士薪资增减函数
void Nurse_Salary_Adjustment(NurseNode* nurse, double modify_salary);

//dept的doctor_id节点creat
Doctor_Id* Create_Doctor_id_Node();


//修改医生某天treat的人数	修改成功返回0，修改失败返回-1
int Modify_Schedule_Doc(DoctorNode* doctor, ScheduleNode_Doctor* schedule_head, ScheduleNode_Doctor* schedule_tail);

//修改（翻转一次）护士某天是否上班的排班函数       默认从off切换到work，再调用此切换函数则回到off态     翻转成功返回0，翻转失败返回-1
int Modify_Schedule_Nurse(NurseNode* nurse, ScheduleNode_Nurse* schedule_head, ScheduleNode_Nurse* schedule_tail);