#ifndef PATIENTINFORMATION_H //患者信息管理
#define PATIENTINFORMATION_H

/*功能说明：1.不支持患者信息删除功能，从现实世界的法律法规和医院实际运营来看只要患者真的在医院看过病、开过药、甚至只是建了档并进行了诊疗记录，这些信息在十几年来是绝对不允许被物理删除的，因为它们在产生医疗中是核心的法律证据。
2.患者挂号时若已经登记过信息则不用再登记且可修改基本信息（姓名，电话，病史）其他根据实生活默认不能通过诊疗系统修改，患者每天最多在每个科室各挂一个号（科室不同就诊号不同），一个医生每天最多问诊20个病人
3.病人不能通过姓名查询个人信息以防同名信息泄露，医生可以
4.最后一位现场登记的默认为当前时间
5.患者只要挂过一次号其基本信息会一直保存在医院，生成的病号唯一*/
#include "check.h"  
#include "Define.h"
#include "Struct.h"
#include "Druginformation.h"
#include<math.h>
#include<stdlib.h>

extern ppatient patienthead ;
extern ppatient patientrear;
extern pregistration registrationhead ;
extern pregistration registrationresr;
extern patientlist list;

void creat_patientlist();//读取文件创建病患初始信息链表(默认是历史记录，没有当日记录）       #######自动
void initial_today_patient( DateNode day);//0:00时初始化当天patientlist，释放昨日看诊链表(ppatientlist结构体，今日时间）   #######自动
int check_department(int patientnumber, int department);//检验该患者是否已在该科室挂号（总挂号记录链表哨兵节点，病号，科室号）（已挂号return1 未挂号return0）
Time find_last();//找最晚现场挂号时间（总挂号记录链表哨兵节点）
void insert_registration_by_time(DoctorNode* q,pregistration p); // 按时间先后顺序将新节点插入到对应医生下的链表中,并从表尾加入到总挂号记录中(对应医生指针）
ppatient search_patientlist(); //利用身份证查找病患信息并打印函数 （return该患者节点）
pregistration search_registration(int) ;//利用诊疗号查找挂号记录并打印函数（诊疗号）（return该挂号记录节点）
void add_patientlist();//挂号函数，增加患者信息函数(patientlist结构体)
void search_patient_name();//医生输入患者姓名查询相关信息
void modify_doctor();//医生录入或修改对应患者诊疗记录
void modify_patient( char );//仅患者修改个人基本信息(病人一般不知道病号故用身份证查询）(身份证号）
float registerfee(int ) ;//根据诊疗记录第一位计算挂号费（门诊15元，急诊30元）
float hospitalizedfee(char[19]) ;//根据诊疗记录计算住院费（住院天数*200元）
float drugfee(pdrug_record) ;//根据诊疗记录计算药费（遍历药品记录链表，找到对应诊疗号的记录，计算总药费）
float personal_drugfee(int );//根据诊疗号计算个人药费（遍历药品记录链表，找到对应诊疗号的记录，计算总药费）
void print_hospitalization_bill();//打印住院账单（根据诊疗记录计算住院费用，住院天数，押金余额等）（诊疗号）
void save_patient_data();//保存新的患者信息            #######自动
void save_registration_data();//保存今日就诊信息         #######自动
#endif


#pragma once 


