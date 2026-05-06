#ifndef DrugInformation_H
#define DrugInformation_H

#include "check.h"
#include "patientinformation.h"
#include<math.h>
#include<stdlib.h>

#define maxeffect     200 //副作用最大长度
#define MAX_HISTORY    30   // 记录最近30天
#define GrugInfile         "F:\\initialdrugfile.text" //原始文件名
#define GrugOutfile         "F:\\newdrugfile.text" //原始文件名
#define RecordInfile    "F:\\initialrecord.text"
#define RecordOutfile    "F:\\newrecord.text"

extern pdrug drug_head;
extern pdrug_record record_head;

typedef struct {
	Date date;          // 记录日期
	int inbound;        // 当日入库量
	int outbound;       // 当日出库量
} DailyRecord;

typedef struct drug {//药品信息结构体
	char drug_name[maxdrugname];//药品名称
	int  is_active;//判断是否在售(1表示在售)
	char drug_sideeffect[maxeffect];//副作用
	int  drug_number;//药品编号：规定第一位为相关科室编号（为0则是通用药品），第二位为排序（从01开始递增），为三位int数型(药品种类不超过99）
	float  bid, price;//进价，买价(默认不超过1000元）
	int drug_inventory;//总库存
	DailyRecord history[MAX_HISTORY]; // 环形数组存30天记录
	int current_day_idx;              // 当前日期在数组中的索引 (0—29)
	int valid_days;                   //已经记录的天数
	struct drug* next_drug;
}tdrug;
typedef tdrug* pdrug;

typedef struct drug_record { // 每一笔具体的开药记录
	int patient_number;      // 开药病人病号
	int  drug_number;//药品编号
	float price;             // 药品单价
	int drug_quantity;       // 开药数量
	Date prescribe_date;     // 开药的具体日期 
	struct drug_record* next_record;
} tdrug_record;
typedef tdrug_record* pdrug_record;

void creat_druglist();//创建药品初始表(仅仅创建药品信息不涉及出入库）  ####自动
void creat_drugrecordlist();//同上传入初始开药记录                                       ####自动
void initial_today_drug(Date);//初始化今日药品出入库（今日日期）（规定每天24:00进行初始化）  ###自动
void add_druglist(Date);//添加药品信息（今日日期）（return 链表尾） 
void search_drug_name();//通过药品名称查询药品基本信息
pdrug search_druglist();//搜索药品信息（return 查询药品节点指针）
void delete_druglist();//删除药品信息
void modify_druglist();//修改药品信息
void outbound_drug(Date);//药品出库附带开药记录
void inbound_drug();//药品入库操作

void save_drug_data();//保存当日药品信息
#endif
#pragma once
