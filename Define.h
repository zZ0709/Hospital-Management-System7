// 常量,全局变量定义
#pragma once
#include<stdlib.h>

#define MAX_DEPT 6          // 6个科室
#define MAX_WARD 20		//一个护士最多负责20个病房

//文件
#define DOCTOR_FILE "doctors.txt"
#define NURSE_FILE "nurses.txt"
#define DOCTOR_SCHEDULE_FILE "doctor_schedule.txt"
#define NURSE_SCHEDULE_FILE "nurse_schedule.txt"

// 医生级别
#define LEVEL_PROF 0        // 教授
#define LEVEL_CHIEF 1       // 主任医师
#define LEVEL_VICE_CHIEF 2  // 副主任医师
#define LEVEL_ATTENDING 3   // 主治医师
#define LEVEL_RESIDENT 4    // 住院医师
#define LEVEL_INTERN 5      // 实习医师

// 护士级别
#define LEVEL_HEAD_NURSE 0  // 护士长
#define LEVEL_NURSE 1       // 普通护士

//长度
#define LENTH_ID 4	//工号长度
#define MENU_BUF_SIZE 10	//菜单输入缓存


#define maxdrugname    100//药品名字最大长度
#define maxeffect       100//副作用长度
#define maxname       25//姓名字符串长度     
#define maxhistory    100//病史字符串长度    
#define infile       "F:\\initialpatient.text"//原始文件名
#define outfile      "F:\\newpatient.text"    //新存文件名
#define REG_OUTFILE "F:\\newregistration.text" 
#define MAX_WARD_NURSE 3 //每个病房最多护士数量