#ifndef PATIENTINFORMATION_H //患者信息管理
#define PATIENTINFORMATION_H

/*功能说明：1.不支持患者信息删除功能，从现实世界的法律法规和医院实际运营来看只要患者真的在医院看过病、开过药、甚至只是建了档并进行了诊疗记录，这些信息在十几年来是绝对不允许被物理删除的，因为它们在产生医疗中是核心的法律证据。
2.患者挂号时若已经登记过信息则不用再登记且可修改基本信息（姓名，电话，病史）其他根据实生活默认不能通过诊疗系统修改，患者每天最多在每个科室各挂一个号（科室不同就诊号不同），一个医生每天最多问诊20个病人
3.病人不能通过姓名查询个人信息以防同名信息泄露，医生可以
4.最后一位现场登记的默认为当前时间
5.患者只要挂过一次号其基本信息会一直保存在医院，生成的病号唯一*/
#include "check.h"  
#include<math.h>
#include<stdlib.h>
#define maxdrugname    100//药品名字最大长度
#define maxname       25//姓名字符串长度     
#define maxhistory    100//病史字符串长度    
#define infile       "F:\\initialpatient.text"//原始文件名
#define outfile      "F:\\newpatient.text"    //新存文件名
#define REG_OUTFILE "F:\\newregistration.text" 

typedef struct {
	int hour;//24小时制
	int minute;
}Time;

typedef struct {
	int year;
	int month;
	int day;
}Date;

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
typedef tregistration ScheduleNode;//###别名
typedef struct {//当日患者人数记录    
	Date today;//当日日期
	int outpatient_number;//门诊人数：规定每晚24:00重置,最多120
	int emergencypatient_number;//急诊人数：规定每晚24:00重置，最多80（为简化，默认没有危重症和急症）
	int hospitalizedpatient_number;//住院人数：#####24:00后根据病房信息重置（有些患者只住一天），规定一个患者可以同时占门/急诊和住院名额
	}patientlist;


ppatient creat_patientlist(ppatient head);//读取文件创建病患初始信息链表(ppatient空链表头)（return patient链表尾）(默认是历史记录，没有当日记录）
void initial_today_patient(patientlist p, pregistration head, Date day);//0:00时初始化当天patientlist，释放昨日看诊链表(ppatientlist结构体,总挂号记录链表哨兵节点，今日时间）
int check_department(pregistration head, int patientnumber, int department);//检验该患者是否已在该科室挂号（总挂号记录链表哨兵节点，病号，科室号）（已挂号return1 未挂号return0）
Time find_last(pregistration head);//找最晚现场挂号时间（总挂号记录链表哨兵节点）
pregistration insert_registration_by_time(DoctorNode* q, pregistration p, pregistration rear); // 按时间先后顺序将新节点插入到对应医生下的链表中,并从表尾加入到总挂号记录中(对应医生指针，挂号记录指针，总挂号记录尾节点，总挂号记录链表哨兵节点）（return新尾节点）
ppatient search_patientlist(ppatient head,char id[19]) {//利用身份证查找病患信息并打印函数 （患者信息链表哨兵节点，身份证号）（return该患者节点）
ppatient add_patientlist(patientlist p, ppatient head, ppatient rear, pregistration begin,pregistration  end);//挂号函数，增加患者信息函数(patientlist结构体，patient链表头+尾，registration链表头+尾)(return 新患者信息链表尾）
void search_patient_name(ppatient head);//医生输入患者姓名查询相关信息(ppatient空链表头)
void modify_doctor(patientlist list);//医生录入或修改对应患者诊疗记录
void modify_patient(ppatient head, char id);//仅患者修改个人基本信息(病人一般不知道病号故用身份证查询）(患者信息链表头，身份证号）
void save_patient_data(ppatient);//保存新的患者信息
void save_registration_data(pregistration head);//保存今日就诊信息
#endif


#pragma once
