#pragma once
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include"ward.h"
#include"zl.h"
#include"date1.h"
#include "account.h"
#include "Date.h"
#define Str_Len 20   //最大长度
#define patientghmax 5//每位患者每天最多可挂五个号
#define hgdepartmentmax 1 //每位患者同一科室最多可挂一个号
#define patinetmax 200 //每天最多两百名患者
#define hospitalmax 500//每天医院最多可受理500个号
#define doctorghmax 20//每位医生每天可最多挂20个号
#define doctormax 30//最多三十名医生
#define dailyfee 200 //每日住院200元

typedef struct RecordNode {//诊疗记录结构体
	Date1 create_time;//诊疗记录创立时间
	char patient_number[20];//患者编号
	char patient_name[20];//患者姓名
	char doctor_id[20];//医生编号
	char doctor_name[20];//医生姓名
	int process_stage;//当前流程阶段，1看诊，2检查，3治疗，4住院，5出院 注意检查完既可以治疗也可以住院
	int planInDays;          // 拟住院天数 N
	double deposit;          // 住院押金
	Date1 inTime;            // 入院时间
	Date1 outTime;           // 出院时间
	int realInDays;          // 实际住院天数
	double totalWardCost;    // 住院总费用
	char exam_items[100];//检查项目，如CT或抽血
	double examfees;//检查费用
	double treatmentfees;//治疗费用
	char treatment_items[100];//治疗项目，输液，手术，拿药
	int ward_no;//病房编号，若不住院则为0
	int is_revisit; //0=初诊，1=复诊
	int is_valid;//记录是否有效，0无效（删除）1有效
	int ishospital;//是否住过院，1为住过，0为没住过
	struct RecordNode* next;//下一节点
}RecordNode;
typedef struct TreatmentSystem{//诊疗管理系统结构体
	RecordNode* record_head;//诊疗记录链表头节点
	int record_count;//当前有效诊疗记录数量
}TreatmentSystem;
extern tpatientlist g_patientList;
extern WARD wardlist[3];//全局变量 病房类型数组
extern TreatmentSystem sys;//声明全局变量
Date1 Date_Input1();//日期输入具体到分钟
Date Date_Input();//日期输入具体到日
int Int_Input(int min, int max);//整型循环输入，并满足在最大值和最小值之间

void init_system(TreatmentSystem *sys);//初始化诊疗管理系统函数
int compare_time(Date1 t1, Date1 t2);//比较两个时间
//----------------------------------------------------------------------//
RecordNode* create_node();//创建诊疗记录节点
void add_record(TreatmentSystem* sys,WARD *wardlist);//添加诊疗记录
void delete_record(TreatmentSystem* sys,WARD *wardlist,Fund_System *Account);//删除诊疗记录
void modify_record(TreatmentSystem * sys, WARD * wardlist,Fund_System *Account);//修改诊疗记录
void searchRecordprintf(TreatmentSystem* sys,WARD* wardlist,Fund_System *Account); //通过创立时间或患者编号或医生编号三种方式查询对应诊疗记录并输出
void printRecordNode(RecordNode * node);//输出单个诊疗记录信息
void printallrecordnode(TreatmentSystem * sys, WARD * wardlist);//显示并输出所有诊疗记录信息
void recordSearchCommon(TreatmentSystem * sys, int type, WARD * wardlist, Fund_System * Account);//公共遍历函数
// 快捷修改：诊疗流程阶段（检查、治疗、住院、出院）
void ChangeProcessStage(TreatmentSystem* sys, WARD* wardlist, Fund_System *Account);
// 显示所有住院患者信息 
void ShowAllInHospitalPatients(TreatmentSystem* sys);
//------------------------------------------------------------------------//
void saveRecordsToFile(TreatmentSystem* sys, const char* filename);//将数据保存到文件中
void loadRecordsFromFile(TreatmentSystem* sys, const char* filename);//从文件中读取数据

Date dateturn(Date1 time);//将Date1类型转换为Date类型
void cureShowmenu();//诊疗系统菜单
