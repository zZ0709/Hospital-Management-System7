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
	int medical_number;//该病人当日的诊疗号       **********************
	int  drug_number;//药品编号
	char drug_name[maxdrugname];//药品名称
	float price;             // 药品单价
	int drug_quantity;       // 开药数量
	Date prescribe_date;     // 开药的具体日期 
	struct drug_record* next_record;
} tdrug_record;
typedef tdrug_record* pdrug_record;

pdrug creat_druglist(pdrug);//创建药品初始表（传入pdrug空链表头）（return链表尾）(仅仅创建药品信息不涉及出入库）
pdrug_record creat_drugrecordlist(pdrug_record head);//同上传入初始开药记录
void initial_today_drug(pdrug,Date);//初始化今日药品出入库（传入pdrug空链表头，今日日期）（规定每天24:00进行初始化）  ####统计资金需要当日统计
pdrug add_druglist(pdrug,Date);//添加药品信息（传入pdrug链表尾,今日日期）（return 链表尾）
void search_drug_name(pdrug);//通过药品名称查询药品基本信息（传入pdrug链表尾）
pdrug search_druglist(pdrug, pdrug*);//搜索药品信息（传入pdrug空链表头，前置节点地址）（return 查询药品节点指针）
void delete_druglist(pdrug);//删除药品信息（传入pdrug空链表头）
void modify_druglist(pdrug);//修改药品信息（传入pdrug空链表头）
pdrug_record outbound_drug(pdrug, pdrug_record, ppatient,Date);//药品出库附带开药记录（传入pdrug空链表头，pdrug_record链表尾，ppatient空链表头）（return 新pdrug_record链表尾）
void inbound_drug(pdrug);//药品入库操作（传入pdrug空链表头）

void save_drug_data(pdrug);//保存当日药品信息
#endif