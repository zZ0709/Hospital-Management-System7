//结构体定义
#pragma once
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "Define.h"
#include "Date.h"

//医生这个月的排班表，链表形式，每个节点存储一天的日期和是否上班
typedef struct ScheduleNode_Doctor {
    Date date;               // 日期
	int treat_count;          // 这一天接诊了几个人，挂了几个号（默认0，接诊一次加1，最多20人次）
    struct ScheduleNode_Doctor* next; // 队列指针
} ScheduleNode_Doctor;


// 医生链表节点
typedef struct  Doctor {
    //五大基本信息
    char id[10];        // 工号（唯一）    例：01（科室）+00（编号）
    char name[20];      // 姓名    例：zhangsan
    int level;          // 级别   （共六级）
    double salary;      // 薪资    (精确到分)
    int dept;           // 科室 1-6// 1.内科 internal  2. 外科 surgery  3.儿科 child  4. 妇科 gynecology  5. 急诊 emergency  6. 全科 general

    int this_day_treat_number;            //今天叫几个号了 ，时刻更新记录，最多20个
    
    ScheduleNode_Doctor* schedule_head;  // 队列头
    ScheduleNode_Doctor* schedule_tail;  // 队列尾

    struct Doctor * next;  // 指向下一个节点

} DoctorNode;


//用于科室中记录医生id的链表节点
typedef struct Doctor_Id {
    char id[10];
    struct Doctor_Id* next;
}Doctor_Id;


//科室链表节点 仅供查询医护       共六个科室
typedef struct Dept {
    int doct_count;      //该科室医生数量
    Doctor_Id* Dept_doctor_head;  //每个科室最多10个医生
    struct Dept* next;             //指向下一个节点
}Dept;



//护士负责的病房id链表
typedef struct Nurse_ward_id {
    char wards_id[8];
   struct  Nurse_ward_id* next;
}Nurse_ward_id;


//护士这个月的排班表，链表形式，每个节点存储一天的日期和是否上班
typedef struct ScheduleNode_Nurse {
    Date date;               // 日期
	int Is_work;          //    0:不上班 1:上班
    struct ScheduleNode_Nurse* next; // 队列指针
} ScheduleNode_Nurse;


// 护士链表节点
typedef struct Nurse {      
    //五大基本信息
    char id[10];        // 工号（唯一）例: 01（病房类型共三种：普通，急诊，icu）+00（编号）
    char name[20];      // 姓名
    int level;          // 级别   （共两级）
    double salary;         // 薪资
    Nurse_ward_id * ward_head_id  ;   //记录护士负责的病房id的头节点

    int ward_number;    //负责的病房数量
   
    ScheduleNode_Nurse* schedule_head;  // 队列头
    ScheduleNode_Nurse* schedule_tail;  // 队列尾

    struct  Nurse * next;   // 指向下一个节点
} NurseNode;

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
	DailyRecord history[30]; // 环形数组存30天记录
	int current_day_idx;              // 当前日期在数组中的索引 (0—29)
	int valid_days;                   //已经记录的天数
	struct drug* next_drug;
}tdrug;
typedef tdrug* pdrug;

typedef struct drug_record { // 每一笔具体的开药记录
	int patient_number;      // 开药病人病号
	int medical_number;//该病人当日的诊疗号       
	int  drug_number;//药品编号
	char drug_name[maxdrugname];//药品名称
	float price;             // 药品单价
	int drug_quantity;       // 开药数量
	Date prescribe_date;     // 开药的具体日期 
    struct drug_record* next_record; // 指向下一笔开药记录
} tdrug_record;
typedef tdrug_record* pdrug_record;

typedef struct Time{	//main中建立latesttime保存现场挂号的最晚时间模拟当前时间
	int hour;		//24小时制
	int minute;
}Time;


typedef struct patient {//患者信息结构体  
	int patient_number;//病号：规定按患者登记个人信息先后顺序系统自动排号，与病人一一对应
	char patient_ID[19];//身份证号
	char patient_name[maxname];//患者姓名（可能重名）
	int patient_gender;//患者性别：规定男为0女为1
	Date patient_birthday;//出生日期
	char patient_phonenumber[12];//患者电话（11位）
	char medical_history[maxhistory]; //病史                             
	struct patient* next_patient; //指向下一个患者
}tpatient;
typedef tpatient* ppatient;//患者信息结构体指针

typedef struct registration {//#####head中medical_number为0
	int medical_number;//就诊号，第一位为科室，第二三位根据登记顺序自动递增
	int patient_number;//病号
	char doctor_id[10];//工号
	int register_type;//挂号类型0为现场1为预约
	Time time;//现场时间，预约时间
	int deposit_balance;//剩余押金：住院时缴纳，规定必须为100的整数，且不低于200*N（N为住院天数），低与1000会进行警告
	char medical_record[10];// 诊疗记录：规定第一位为患者状态（1门诊「15元」，2急诊「30元」），第二位为是否住院（1不住院2住院），第三四五位为住院天数（系统最多记录一年的信息，故规定天数不超过三位「住院费一天200元」），（根据实际默认次数不会超过9次）第六位为CT「70元」次数，第七位为抽血「10元」次数，第八位为输液次数「25元」，第九位为手术次数「2000元」）   
	struct registration* next_record; // 用于总挂号链表（不排序，一直往后接）
	struct registration* next_doctor_record; // 用于医生个人队列（按时间排序）
}tregistration;

typedef struct registration* pregistration;

typedef tregistration ScheduleNode;

typedef struct {//当日患者人数记录    
	Date today;//当日日期
	int outpatient_number;//门诊人数：规定每晚24:00重置,最多120
	int emergencypatient_number;//急诊人数：规定每晚24:00重置，最多80（为简化，默认没有危重症和急症）
	}patientlist;
