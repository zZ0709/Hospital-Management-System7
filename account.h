#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#define Init_Amount 200000.00//账户初始资金
#define Str_Len 20
#define Account_Filename "account.txt"

bool isValidTwoDecimal(const char* str);
//=========================日期===============================
typedef struct {
	int year;//年
	int month;//月
	int day;//日
}Date;
//=========================账户资金变动记录===========================
typedef struct Fund_Record{
	bool Is_Valid;//是否有效
	int order_Number;//序号
	double cash;//涉及金额
	int type;//来源/类型：1.住院 2.挂号 3.疗诊 4.工资发放 5.药品采购 6.药品出售
	Date date;//日期
	Fund_Record* next;
}Fund_Record;
//========================账户数据====================================
typedef struct {
	Fund_Record* Fund_List;//资金记录
	int sumAmount;//账户余额
	int Record_Num;//记录条数
}Fund_System;
Fund_System Account;
extern Fund_System Account;

bool Check_Double(const char* str);
int Date_Compare(Date Start, Date End);//判断两个日期的先后是否合法
void Record_Print(Fund_Record *record);//某一条记录的打印
Date Date_Input();//日期输入
void Type_Choose();//资金记录中资金类型选择
int Int_Input(int min,int max);//整型输入
double Double_Input();//浮点数输入
int Check_Int(char* str, int min, int max);//检查输入整型变量是否合法
void Init_Account(Fund_System *Account);//初始化资金账户
//==================资金系统增删查改打印=====================
void Fund_Add(Fund_System *Account);//资金记录增加
void Auto_Fund(Fund_System *Account, int type, int cash, Date date);//自动添加（无提示）
//输入类型，金额，日期
void Fund_Change(Fund_System *Account);//资金记录修改
void Fund_Delete(Fund_System *Account);//资金记录删除
void Foud_Search(Fund_System *Account);//资金记录查找
void Print_Allrecord(Fund_System *Account);//资金记录打印（打印某一时间段内的记录）
int Account_Save_File(Fund_System *Account);//保存到文件
int Account_Save_Read(Fund_System *Account);//文件读取