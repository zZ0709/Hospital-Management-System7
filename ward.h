#pragma once
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include"date1.h"
#include "Nurse.h"
#include "Tool.h"
#include "Date.h"
#include "account.h"

typedef struct patient tpatient;  // 声明结构体类型
typedef tpatient * ppatient;        // 声明指针类型
typedef struct patientlist tpatientlist;//声明结构体类型
typedef tpatientlist* ppatientlist;//声明指针类型
extern tpatientlist g_patientList;
#define wardmax 80//病房最多数量
#define inpatient 60//住院患者最多数量
#define ptbeds 4//普通病房床数
#define jzbeds 3//急诊病房床数
#define icubeds 1//ICU病房床数
#define startmoney 200000//医院起始资金
// 护士编号链表节点（一个病房对应多个护士）
typedef struct nurse_id_node {
	char nurseid[20];
	struct nurse_id_node* next;
} NurseIDNode;
// 单日床位状态节点
typedef struct bed_point {
	Date date;
	bool is_use;
}bed_point;
// 一个月30天队列
typedef struct {
	bed_point is_Use[31];//默认队尾为空
	int front;//第一个有效数据下标
	int rear;//指向下一个要添加的下标
}bed_que;
typedef struct bed {//床位结构体
	char bedID[20];//床位编号
	bool isfree;//1为空，2为不空
	ppatient patient;//患者信息
	bed_que dayQueue;//床近30天是否为空队列
	struct bed* next;//下一个床位节点
}bed;

typedef  struct ward {//病房结构体
	 char roomID[20];//病房编号，一共四位，第一位为科室编号，第二位为病房类型，后两位为序号，每个病房类型都从01开始
	// 病房总数不超过20
	int wardtype;//病房类型（1.普通病房，2.急诊病房，3.ICU病房）
	int totalbeds;//总床数
	int freebeds;//空闲床数
	int nursenum;//护士总数
	int department;//关联科室1.intetnal 2.sugery 3.child 4.gune 5.emergency 6.full
	NurseIDNode* nurselist; // 值班护士编号链表（新建病房时为NULL）
	bed* bedlist;//床位链表头节点
	struct ward* next;//下一病房节点
}ward;
typedef struct {//病房类型结构体
	ward* wardhead;//各病房类型头节点
	int wardnum;//对应病房类型数量
}WARD;//设立一个WARD类型wardlist[3]每种类型都创建一个链表
//--------------------------------------------------------增删查改等功能
bed* Createbed(char *str);//创建床位
void Addward(WARD *wardlist);//增加病房
void Deleteward(WARD* wardlist);//删除病房信息
void Searchwardprintf(WARD* wardlist);//查找特定病房并显示信息
void SearchwardbyID(WARD* wardlist);//通过病房编号查询
void Searchwardbytype(WARD* wardlist);//通过病房类型查询
void Searchwardbykeshi(WARD* wardlist);//通过病房相关科室查询
void printfwarddata(ward* a);//输出病房的所有信息
void Searchwardbyfreebedsall(WARD* wardlist);//通过病房是否有空床
ward * Searchwardbylxfreebeds(WARD* wardlist);//通过对应科室病房是否有空床
void Searchwardbynurseid(WARD* wardlist);//通过护士编号查询病房
void Searchwardbynursename(WARD* wardlist);//通过护士姓名查询病房
void Modifyward(WARD* wardlist);//修改病房信息
ward* Searchward(WARD* wardlist, char* str);//通过编号寻找特定病房节点
int  Searchwardint(WARD* wardlist, char* str);//通过编号查询对应节点查到返回1，未查到返回0
bed* findfirstfreebeds(ward* p);//寻找有空床病房中第一个空床
//-----------------------------------------------------------判断输入
int Check1(char* str, int max, int min);//判断输入的是否为整数，是否在max和min之间
int Check2(char* str);//判断字符串是否全为数字
//------------------------------------------
void Showmenu();//病房系统菜单
void Showward(WARD* wardlist);//显示病房信息
void Openfile(WARD* wardlist,const char *filename);//从文件中读取数据
int IDtwo(char* str);//返回对应数的后两位
void initial(WARD* wardlist);//初始化病房类型变量
ward* prevfind(ward* p, WARD* wardlist, int i);//寻找前驱节点
void savefile(WARD* wardlist,const char *filename);//将之前数据保存到文件中
//--------------------------------------------------------床位使用情况--------------------------
// ==================== 队列基本操作函数 ====================

// 1. 初始化队列
void InitQueue(bed_que * q);
// 2. 判断队列是否为空
bool QueueEmpty(bed_que* q);
// 3. 判断队列是否为满
bool QueueFull(bed_que* q);// 4. 入队操作（向队列尾部添加元素）
// 参数：q-队列指针，elem-要入队的元素
bool EnQueue(bed_que* q, bed_point elem);
// 5. 出队操作（从队列头部取出元素）
// 参数：q-队列指针，elem-用于存储出队元素的地址
bool DeQueue(bed_que* q, bed_point* elem);
// 6. 获取队列当前元素个数
int QueueLength(bed_que* q);
// 7. 获取队头元素（不出队）
bool GetHead(bed_que* q, bed_point* elem);
// 护士链表工具函数
int NurseCount(NurseIDNode* head);
void PrintNurseList(ward* a);
void FreeNurseList(NurseIDNode* head);
void AddNurseToWard(ward* w);
void RemoveNurseFromWard(ward* w);



