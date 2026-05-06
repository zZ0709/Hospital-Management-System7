#ifndef DrugInformation_H
#define DrugInformation_H

#include "check.h"
#include "Struct.h"
#include "patientinformation.h"
#include<math.h>
#include<stdlib.h>

#define maxeffect     200 //副作用最大长度
#define MAX_HISTORY    30   // 记录最近30天
#define GrugInfile         "F:\\initialdrugfile.text" //原始文件名
#define GrugOutfile         "F:\\newdrugfile.text" //原始文件名
#define RecordInfile    "F:\\initialrecord.text"
#define RecordOutfile    "F:\\newrecord.text"

extern pdrug druglisthead; 
extern pdrug druglistrear;
extern pdrug_record drugrecordhead;
extern pdrug_record drugrecordrear;

void creat_druglist();//创建药品初始表(仅仅创建药品信息不涉及出入库）  ####自动
void creat_drugrecordlist();//同上传入初始开药记录                                       ####自动
void initial_today_drug(Date);//初始化今日药品出入库（今日日期）（规定每天24:00进行初始化）  ###自动
void add_druglist(Date);//添加药品信息（今日日期）（return 链表尾） 
void search_drug_name();//通过药品名称查询药品基本信息
void search_drug_department();//通过科室查询药品基本信息
pdrug search_druglist();//搜索药品信息（return 查询药品节点指针）
void delete_druglist();//删除药品信息
void modify_druglist();//修改药品信息
void outbound_drug(Date);//药品出库附带开药记录
void inbound_drug();//药品入库操作

void save_drug_data();//保存当日药品信息
#endif
#pragma once
