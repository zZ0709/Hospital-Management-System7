#pragma once
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#define depositmin 1000

#include"ward.h"
#include"date1.h"
#include"cure.h"
#include "Druginformation.h"
#include "account.h"
#include "Date.h"
typedef struct RecordNode  RecordNode;
struct TreatmentSystem;

extern tpatientlist g_patientList;
int isLeapYear(int year);//判断闰年
// 只算年月日差值
int dateDiffDay(Date1 inTime, Date1 outTime);
int getRealWardDays(Date1 inTime, Date1 outTime);//求具体住院天数
// 输入住院押金
double inputDeposit(int planDays);
//每天八点准时扣费且押金不足要提醒
void autoDeductDeposit(double* deposit);
void RechargeDeposit(double* currentDeposit);//补交押金
void inhospital(RecordNode* p, WARD* wardlist);//住院整体函数
// 办理出院结算
void HospitalDischarge(RecordNode* p, Fund_System *Account, WARD* wardlist);
void Exam(RecordNode* p, Fund_System *Account);//检查函数
void Treatment(RecordNode* p, Fund_System *Account);//治疗函数