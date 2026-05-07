#include<stdio.h>
#include"cure.h"
#include"ward.h"
#include"account.h"
#include "Patientinformation.h"

Date1 Date_Input1()//日期输入
{
	Date1 ret;
	printf("please input the date\n");
	printf("year(2000-2026):");
	ret.year = Int_Input(2000, 2026);
	printf("\n");
	printf("month:");
	ret.month = Int_Input(1, 12);
	printf("\n");
	printf("day:");
	ret.day = Int_Input(1, 31);
	printf("\n");
    printf("hour:");
    ret.hour = Int_Input(0, 23);
    printf("\n");
    printf("minute:");
    ret.minute = Int_Input(0, 59);
    printf("\n");
	return ret;
}

void init_system(TreatmentSystem * sys) {//初始化诊疗管理系统的函数
    sys->record_head = NULL;//头节点置空
    sys->record_count = 0;//诊疗有效记录为0
}
int compare_time(Date1 t1, Date1 t2) {//比较两个时间 -1表示t1<t2,0表示相等，1表示t1>t2
    if (t1.year != t2.year)return (t1.year< t2.year) ? -1 : 1;//比较月份
	if (t1.month != t2.month)return (t1.month < t2.month) ? -1 : 1;//比较月份
	if (t1.day != t2.day)return (t1.day < t2.day) ? -1 : 1;//比较日期
	if (t1.hour != t2.hour)return (t1.hour< t2.hour) ? -1 : 1;//比较小时
	if (t1.minute != t2.minute)return (t1.minute < t2.minute) ? -1 : 1;//比较分钟
	return 0;
}
RecordNode* create_node() {
	RecordNode* node = (RecordNode*)malloc(sizeof(RecordNode)); // 分配内存
    if (node == NULL) {         // 检查内存分配是否成功
        printf("Failed to add\n"); // 内存分配失败
        exit(EXIT_FAILURE);//强制退出
   }
    // 入院时间清零
    node->inTime.year = 0;
    node->inTime.month = 0;
    node->inTime.day = 0;
    node->inTime.hour = 0;
    node->inTime.minute = 0;
    node->examfees = 0;
    node->treatmentfees = 0;
    // 出院相关 全部清零
    node->outTime.year = 0;
    node->outTime.month = 0;
    node->outTime.day = 0;
    node->outTime.hour = 0;
    node->outTime.minute = 0;
    node->realInDays = 0;
    node->totalWardCost = 0.0;
	node->next = NULL;          // 下一个指针置空
	node->ward_no = 0;          // 标记未分配病房
    node->is_valid = 1;          //标记为有效
    node->ishospital = 0;       //初始标记为未住院
    node->is_revisit = 0;//标记为初诊
	return node;                // 返回新节点指针
}
void add_record(TreatmentSystem* sys,WARD *wardlist){
    printf("Please enter the add record time\n");
   Date1 reg_time=Date_Input1();//输入时间
   char id[22]; char ptid[22];
   RecordNode* new_node = create_node();   // 调用创建节点函数
   extern tpatientlist g_patientList;
   ppatientlist p2 = &g_patientList;
   ppatient p0 = NULL;  // search_patientlist 的参数，根据实际情况调整
   ppatient p1;

   while (1) {
       p1 = search_patientlist(p2->nextpatient, p0);
       if (p1 == NULL) {
           continue;
       }
       else {
           sprintf(ptid, "%d", p1->patient_number);
           strcpy(new_node->patient_number, ptid);
           break;
       }
   }
   strcpy(new_node->patient_name, p1->patient_name);
       // ===== 复诊自动判断 =====
       RecordNode* checker = sys->record_head;
   while (checker != NULL) {
       if (checker->is_valid == 1 && strcmp(checker->patient_number, new_node->patient_number) == 0) {
           break;
       }
       checker = checker->next;
   }
   if (checker != NULL) {
       printf("Note: The patient has historical records. Automatically marked as REVISIT.\n");
       new_node->is_revisit = 1;
   }
   else {
       new_node->is_revisit = 0;
   }

   // ======================
   // 3. 选择医生（必须存在，不能乱输）
   // ======================
   DoctorNode* doc;
   while (1) {
       doc = Doctor_id(); //检索医生信息函数，利用唯一的编号查找节点
       if (doc == NULL) {
           continue;
       }
       else {
           //找到节点则将对应医生编号输入进去
           strcpy(new_node->doctor_id, doc->id);
           break;
       }
   }
   strcpy(new_node->doctor_name, doc->name);//对应医生姓名
   new_node->create_time = reg_time;   // 设置创建时间
   Date k = dateturn(new_node->create_time);
   UpdateAllBedsQueue(wardlist, k);//对床队列状态进行更新
    // 初始化诊疗流程
    new_node->process_stage = 1;      // 初始阶段为1（挂号/看诊）
    strcpy(new_node->exam_items, "unchecked");     // 初始化检查项目
    strcpy(new_node->treatment_items, "untreated"); // 初始化治疗项目
    // 将新节点插入链表尾部
    
    if (sys->record_head == NULL) {
        sys->record_head = new_node;
    }
    else {
        RecordNode* p = sys->record_head;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = new_node;
        new_node->next = NULL;
        
    }sys->record_count++;//有效诊疗记录数量加1
    printf("Registratiom successfully\n");
    system("pause");
    system("cls");
    return ;                   // 返回成功
}//////////////////////////////////////////////////////////////////////////////要修改
void delete_record(TreatmentSystem* sys,WARD *wardlist,Fund_System *Account) {//删除诊疗记录
    recordSearchCommon(sys, 2, wardlist,Account);
    
    system("pause");
    system("cls");
}
void modify_record(TreatmentSystem* sys,WARD* wardlist,Fund_System *Account) {//修改诊疗记录
    recordSearchCommon(sys, 3, wardlist,Account);
    system("pause");
    system("cls");
}
void searchRecordprintf(TreatmentSystem * sys, WARD * wardlist,Fund_System *Account) {//通过创立时间或患者编号或医生编号三种方式查询对应诊疗记录并输出
    printf("Please enter current system date to update bed status\n");
    Date time = Date_Input();//输入查找时间
    UpdateAllBedsQueue(wardlist, time);//对床队列状态进行更新 
    recordSearchCommon(sys, 1, wardlist,Account);
    system("pause");
    system("cls");
    }
void printallrecordnode(TreatmentSystem* sys,WARD *wardlist) {
    RecordNode* head = sys->record_head;
    while (head) {
        printRecordNode(head);
        head = head->next;
    }
    system("pause");
    system("cls");
}
 void printRecordNode(RecordNode* node) {
     if (node == NULL) {
         printf("Record does not exist!\n");
         return;
     }

     printf("=========================================\n");
     // 基本信息（无论什么情况都打印）
     printf("Create Time: %d-%02d-%02d\n", node->create_time.year, node->create_time.month, node->create_time.day);
     printf("Patient Number: %s ", node->patient_number);
     printf("Patient Name: %s ", node->patient_name);
     printf("Doctor ID: %s ", node->doctor_id);
     printf("Doctor Name: %s\n ", node->doctor_name);
     printf("Revisit Status: %s\n", node->is_revisit ? "Revisit" : "First Visit");
     // 打印当前流程阶段
     switch (node->process_stage) {
     case 1: printf("Current Stage: Consultation\n"); break;
     case 2: printf("Current Stage: Examination\n"); break;
     case 3: printf("Current Stage: Treatment\n"); break;
     case 4: printf("Current Stage: Hospitalization\n"); break;
     case 5: printf("Current Stage: Discharge\n"); break;
     default: printf("Current Stage: Unknown\n"); break;
     }

     // 检查和治疗信息（通用）
     printf("Exam Items: %s ", node->exam_items);
     printf("Exam Fees: %.2f\n", node->examfees);
     printf("Treatment Items: %s ", node->treatment_items);
     printf("Treatment Fees: %.2f\n", node->treatmentfees);

     // ===================== 住院信息逻辑 =====================
     if (node->ishospital == 0) {
         // 没住过院：不输出任何住院信息，只提示
         printf("Hospitalization Status: Not hospitalized\n");
     }
     else {
         // 住过院
         if (node->process_stage == 4) {
             // 住院中，未出院：只打印 拟住院天数、押金、入院时间
             printf("Hospitalization Status: In hospital, not discharged\n");
             printf("Plan In Days: %d ", node->planInDays);
             printf("Deposit: %lf ", node->deposit);
             printf("In Time: %d-%02d-%02d\n", node->inTime.year, node->inTime.month, node->inTime.day);
         }
         else {
             // 已出院：打印完整住院信息
             printf("Hospitalization Status: Discharged\n");
             printf("Plan In Days: %d ", node->planInDays);
             printf("Deposit: %lf\n", node->deposit);
             printf("In Time: %d-%02d-%02d ", node->inTime.year, node->inTime.month, node->inTime.day);
             printf("Out Time: %d-%02d-%02d ", node->outTime.year, node->outTime.month, node->outTime.day);
             printf("Real In Days: %d\n", node->realInDays);
             printf("Total Ward Cost: %.2f ", node->totalWardCost);
             printf("Ward No: %d\n", node->ward_no);
         }
     }

     // 记录有效性
     if (node->is_valid == 0) {
         printf("Record Status: Invalid (Deleted)\n");
     }
     else {
         printf("Record Status: Valid\n");
     }
     printf("=========================================\n\n");
 }
//将数据保存到文件中
   void saveRecordsToFile(TreatmentSystem* sys, const char* filename) {
          if (!sys || !filename) return;
          if (strlen(filename) == 0) {
              printf("Empty filename\n");
              system("pause");
              system("cls");
              return;
          }

          FILE* fp = fopen(filename, "w");
          if (!fp) {
              printf("File open failed: %s\n", filename);
              perror("Reason");
              system("pause");
              system("cls");
              return;
          }

          int count = 0;
          RecordNode* p = sys->record_head;
          while (p) {
              if (p->is_valid == 1) {
                  fprintf(fp, "%d %d %d %d %d\n",
                      p->create_time.year, p->create_time.month, p->create_time.day,
                      p->create_time.hour, p->create_time.minute);
                  fprintf(fp, "%s %s %s %s\n",
                      p->patient_number, p->patient_name, p->doctor_id, p->doctor_name);
                  fprintf(fp, "%d %d %s %s %.2f %.2f\n",
                      p->process_stage, p->is_revisit,p->exam_items, p->treatment_items,
                      p->examfees, p->treatmentfees);
                  fprintf(fp, "%d %d %lf %d %d %d %d %d %d %d %d %.2f %d\n",
                      p->ishospital, p->ward_no, p->deposit, p->planInDays,
                      p->inTime.year, p->inTime.month, p->inTime.day,
                      p->outTime.year, p->outTime.month, p->outTime.day,
                      p->realInDays, p->totalWardCost, p->is_valid);
                  count++;
              }
              p = p->next;
          }

          fclose(fp);
          if (count == 0)
              printf("No valid records to save\n");
          else
              printf("Save successfully, %d records saved to %s\n", count, filename);
          system("pause");
          system("cls");
   }

      //从文件中读取数据
      void loadRecordsFromFile(TreatmentSystem* sys, const char* filename) {
          if (!sys || !filename) return;
          if (strlen(filename) == 0) {
              printf("Empty filename\n");
              return;
          }

          // 清空现有数据
          RecordNode* p = sys->record_head;
          while (p) {
              RecordNode* tmp = p;
              p = p->next;
              free(tmp);
          }
          sys->record_head = NULL;
          sys->record_count = 0;

          FILE* fp = fopen(filename, "r");
          if (!fp) {
              printf("File doesn't exist: %s\n", filename);
              return;
          }

          int count = 0;
          while (1) {
              RecordNode* node = create_node();
              if (!node) {
                  printf("Failed to create node\n");
                  break;
              }

              int ret = fscanf(fp, "%d %d %d %d %d",
                  &node->create_time.year, &node->create_time.month, &node->create_time.day,
                  &node->create_time.hour, &node->create_time.minute);
              if (ret != 5) { free(node); break; }

              ret = fscanf(fp, "%19s %19s %19s %19s",
                  node->patient_number, node->patient_name, node->doctor_id, node->doctor_name);
              if (ret != 4) { free(node); break; }

              ret = fscanf(fp, "%d %d %99s %99s %lf %lf",
                  &node->process_stage,&node->is_revisit, node->exam_items, node->treatment_items,
                  &node->examfees, &node->treatmentfees);
              if (ret != 6) { free(node); break; }

              ret = fscanf(fp, "%d %d %lf %d %d %d %d %d %d %d %d %lf %d",
                  &node->ishospital, &node->ward_no, &node->deposit, &node->planInDays,
                  &node->inTime.year, &node->inTime.month, &node->inTime.day,
                  &node->outTime.year, &node->outTime.month, &node->outTime.day,
                  &node->realInDays, &node->totalWardCost, &node->is_valid);
              if (ret != 13) { free(node); break; }

              if (sys->record_head == NULL) {
                  sys->record_head = node;
              }
              else {
                  RecordNode* tail = sys->record_head;
                  while (tail->next != NULL) tail = tail->next;
                  tail->next = node;
              }
              node->next = NULL;
              count++;
              sys->record_count++;
              // 插入链表后，根据阶段恢复关键业务状态
              switch (node->process_stage) {
              case 1: // 仅挂号，无需额外操作
                  break;

              case 2: // 已检查
              case 3: // 已治疗
                  
                  break;

              case 4: // 正在住院 —— 这是最核心要恢复的
                  if (node->ishospital == 1 && node->ward_no != 0) {
                      char temp[20];
                      sprintf(temp, "%d", node->ward_no);
                      ward* w = Searchward(wardlist, temp);

                      if (w != NULL && w->freebeds > 0) {
                          // 在全局患者链表中找回该患者指针，绑定床位
                          int pid = Check2(node->patient_number);
                          ppatient p1 = g_patientList.nextpatient;
                          while (p1) {
                              // 根据你们 patient_number 的实际类型调整比较方式
                              if (p1->patient_number == pid) {
                                  break;
                              }
                              p1 = p1->next_patient;
                          }

                          if (p1 != NULL) {
                              bed* b = findfirstfreebeds(w);
                              if (b != NULL) {
                                  b->isfree = false;
                                  b->patient = p1;      // 重建患者-床位绑定
                                  w->freebeds--;
                                  node->ishospital = 1; // 确保标志位正确
                              }
                          }
                      }
                  }
                  break;

              case 5: // 已出院
                  // 床位已在出院时释放，不需要恢复床位占用
                  node->ishospital = 1; // 标记住过院，用于打印历史信息
                  break;
              }

          }

          fclose(fp);
          if (count == 0)
              printf("No valid records loaded from %s\n", filename);
          else
              printf("Load successfully, %d records loaded from %s\n", count, filename);
      }

  
  Date dateturn(Date1 time) {//将Date1类型转换为Date类型
    Date time1;
    time1.year = time.year;
    time1.month=time.month;
    time1.day = time.day;
    return time1;
}
  void cureShowmenu() {//诊疗系统菜单
      printf("Cure Record System\n");//诊疗记录系统
      printf("1.Add cure record\n");//添加诊疗记录
      printf("2.Show cure record information\n");//显示所有诊疗记录
      printf("3.Delete  cure record\n");//删除诊疗记录
      printf("4.Modify cure record\n");//修改诊疗记录
      printf("5.Search cure record\n");//查询诊疗记录
      printf("6.Change Treatment Stage\n");//改变诊疗阶段
      printf("7.show all inpatient information\n");//显示所有住院患者信息
      printf("8.Save to file\n");//将数据保存到文件中
      printf("9.Exit cure record system\n");//退出诊疗记录系统
  }
  void ChangeProcessStage(TreatmentSystem* sys, WARD* wardlist, Fund_System* Account) {
      if (sys->record_head == NULL) {
          printf("No record!\n");
          system("pause");
          system("cls");
          return;
      }

      char no[20];
      printf("Enter patient number: ");
      fgets(no, 20, stdin);
      no[strcspn(no, "\n")] = '\0';
      int count = 0;
      RecordNode* p = sys->record_head;

      while (p != NULL) {
          if (strcmp(p->patient_number, no) == 0 && p->is_valid == 1) {
              int choice;
              count++;

              printf("\n===== Change Treatment Stage =====\n");
              printf("Current stage: ");
              switch (p->process_stage) {
              case 1: printf("Consultation\n"); break;
              case 2: printf("Examination\n"); break;
              case 3: printf("Treatment\n"); break;
              case 4: printf("Hospitalization\n"); break;
              case 5: printf("Discharge\n"); break;
              default: printf("Unknown\n"); break;
              }

              printf("1. Check       \n");
              printf("2. Treatment   \n");
              printf("3. Hospitalize \n");
              printf("4. Discharge   \n");
              printf("Choose target stage: ");
              choice = Int_Input(1, 4);

              bool ok = true;  // 标记整个流程是否顺利推进

              switch (choice) {
              case 1: // 目标：检查 (stage 2)
                  if (p->process_stage >= 2) {
                      printf("Patient has already completed examination.\n");
                  }
                  else if (p->process_stage < 1) {
                      printf("Please complete the diagnosis first!\n");
                      ok = false;
                  }
                  else {
                      Exam(p, Account);  // 内部会把 stage 设为 2
                  }
                  break;

              case 2: // 目标：治疗 (stage 3)
                  if (p->process_stage >= 3) {
                      printf("Patient has already completed treatment.\n");
                  }
                  else {
                      if (p->process_stage < 1) {
                          printf("Please complete the diagnosis first!\n");
                          ok = false;
                      }
                      else {
                          if (p->process_stage < 2) {
                              Exam(p, Account);
                          }
                          if (p->process_stage == 2) {
                              Treatment(p, Account);  // 内部会把 stage 设为 3
                          }
                          else {
                              printf("Failed to proceed to treatment.\n");
                              ok = false;
                          }
                      }
                  }
                  break;

              case 3: // 目标：住院 (stage 4)
                  if (p->process_stage >= 4) {
                      printf("Patient is already hospitalized.\n");
                  }
                  else {
                      if (p->process_stage < 1) {
                          printf("Please complete the diagnosis first!\n");
                          ok = false;
                      }
                      else {
                          if (p->process_stage < 2) {
                              Exam(p, Account);
                          }
                          if (p->process_stage == 2) {
                              Treatment(p, Account);
                          }
                          if (p->process_stage == 3) {
                              inhospital(p, wardlist);  // 内部会设置 ishospital=1 和 stage=4（仅成功时）
                              if (p->process_stage != 4) {
                                  printf("Hospitalization failed! No available beds or other error.\n");
                                  ok = false;
                              }
                          }
                          else {
                              printf("Cannot hospitalize: prerequisite stages not completed.\n");
                              ok = false;
                          }
                      }
                  }
                  break;

              case 4: // 目标：出院 (stage 5)
                  if (p->process_stage == 5) {
                      printf("Patient has already been discharged.\n");
                  }
                  else if (p->process_stage == 4) {
                      HospitalDischarge(p, Account, wardlist);  // 内部会把 stage 设为 5
                  }
                  else {
                      // 需要先补完前面所有阶段
                      if (p->process_stage < 1) {
                          printf("Please complete the diagnosis first!\n");
                          ok = false;
                      }
                      else {
                          if (p->process_stage < 2) Exam(p, Account);
                          if (p->process_stage == 2) Treatment(p, Account);
                          if (p->process_stage == 3) {
                              inhospital(p, wardlist);
                          }
                          if (p->process_stage == 4) {
                              HospitalDischarge(p, Account, wardlist);
                          }
                          else {
                              printf("Cannot discharge: patient is not hospitalized.\n");
                              ok = false;
                          }
                      }
                  }
                  break;
              }

              if (!ok) {
                  printf("Stage change aborted.\n");
              }
          }
          p = p->next;
      }

      if (count == 0) {
          printf("Record not found!\n");
          system("pause");
          system("cls");
          return;
      }

      printf("Please enter the modify time\n");
      Date time = Date_Input();
      UpdateAllBedsQueue(wardlist, time);
      printf("Modify successfully\n");
      system("pause");
      system("cls");
      return;
  }


  // 显示所有住院患者信息 
  void ShowAllInHospitalPatients(TreatmentSystem* sys)
  {
      if (sys == NULL || sys->record_head == NULL)
      {
          printf("No inpatient records found!\n");
          system("pause");
          system("cls");
          return;
      }

      char dupCheck[200][20];
      int dupCnt = 0;
      int total = 0;

      RecordNode* cur = sys->record_head;

      while (cur != NULL)
      {
          if (cur->is_valid == 1 && cur->process_stage == 4)
          {
              bool hasPrint = false;
              for (int i = 0; i < dupCnt; i++)
              {
                  if (strcmp(dupCheck[i], cur->patient_number) == 0)
                  {
                      hasPrint = true;
                      break;
                  }
              }

              if (!hasPrint)
              {
                  
                  printf("-----------------------------------\n");
                  printf("Patient Number: %s\n", cur->patient_number);
                  printf("Patient Name: %s\n", cur->patient_name);
                  printf("Ward No: %d\n", cur->ward_no);
                  printf("Deposit: %.2f\n", cur->deposit);
                  printf("In Time: %d-%02d-%02d %02d:%02d\n",
                      cur->inTime.year, cur->inTime.month, cur->inTime.day,
                      cur->inTime.hour, cur->inTime.minute);
                  printf("Plan Days: %d\n", cur->planInDays);
                  printf("-----------------------------------\n");

                  if (dupCnt < 200) {
                      strcpy(dupCheck[dupCnt++], cur->patient_number);
                  }
                  total++;
              }
          }
          cur = cur->next;
      }

      if (total == 0)
      {
          printf("No inpatients at present.\n");
      }
      else
      {
          printf("Total inpatients: %d\n", total);
      }
      system("pause");
      system("cls");
  }
