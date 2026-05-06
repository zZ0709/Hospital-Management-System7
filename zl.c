#include<stdio.h>
#include"ward.h"
#include"cure.h"
#include"zl.h"
#include"account.h"
//判断闰年
int isLeapYear(int year)
{
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        return 1;
    return 0;
}
// 只算年月日差值
int dateDiffDay(Date1 inTime, Date1 outTime)
{
    int monthDay[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
    int y, m;
    long long sum1 = 0, sum2 = 0;

    for (y = 1; y < inTime.year; y++)
        sum1 += (isLeapYear(y) ? 366 : 365);
    for (m = 1; m < inTime.month; m++)
        sum1 += (m == 2 && isLeapYear(inTime.year)) ? 29 : monthDay[m];
    sum1 += inTime.day;

    for (y = 1; y < outTime.year; y++)
        sum2 += (isLeapYear(y) ? 366 : 365);
    for (m = 1; m < outTime.month; m++)
        sum2 += (m == 2 && isLeapYear(outTime.year)) ? 29 : monthDay[m];
    sum2 += outTime.day;

    return sum2-sum1;
}

int getRealWardDays(Date1 inTime, Date1 outTime)//求具体住院天数
{
    int baseDays = dateDiffDay(inTime, outTime);

    // 八点之后出院，多算当天
    if (outTime.hour >= 8)
    {
        baseDays += 1;
    }

    // 最少1天
    if (baseDays < 1)
        baseDays = 1;

    return baseDays;
}

// 输入住院押金
double inputDeposit(int planDays) {
   double minDeposit = 200 * planDays;
   double deposit;

    while (1) {//检验输入是否满足条件
        printf("Please enter deposit (100 multiples, min %lf): \n", minDeposit);
       int ret= scanf("%lf", &deposit);
       if (ret != 1) {
           printf("Input error\n");
           while (getchar() != '\n');
           continue;
       }
        if (fabs(fmod(deposit, 100.0)) < 1e-6 && deposit >= minDeposit) {
            break;
        }
        printf("Invalid input!\n");
        printf("Rules: 1.Must be multiple of 100  2.Not less than %lf\n", minDeposit);
    }
    return deposit;
}
//每天八点准时扣费且押金不足要提醒
void autoDeductDeposit(double* deposit) {
    *deposit -= dailyfee;
    
}
void RechargeDeposit(double* currentDeposit)//补交押金
{
    int addAmount;
    double need;
    printf("\n----- Deposit Recharge -----\n");

    while (1)
    {
        // 计算还差多少才到1000
        need =  depositmin- *currentDeposit;
        while (1) {
            printf("Current deposit: %.2lf\n", *currentDeposit);
            printf("Need at least %.2f more to reach safe balance(1000)\n", need);
            printf("Please input recharge amount (multiple of 100): ");
            int ret=scanf("%d", &addAmount);
            if (ret!=1) {
                printf("Input error\n");
                while (getchar() != '\n');
                continue;

            }
            else {
                break;
            }
        }

        // 条件1：补交金额是100倍数、正数
        if (addAmount <= 0 || addAmount % 100 != 0)
        {
            printf("Amount must be positive and multiple of 100.\n");
            continue;
        }
        // 条件2：补交后 押金 >= 1000
        if (*currentDeposit + addAmount >= depositmin)
        {
            *currentDeposit += addAmount;
            printf("Recharge success! New deposit: %.2lf\n", *currentDeposit);
            break;
        }
        else
        {
            printf("Recharge failed! Balance still less than 1000 after recharge.\n");
           
        }
    }
}
void inhospital(RecordNode *p, WARD* wardlist) {//住院整体函数
    
    ward *k=  Searchwardbylxfreebeds(wardlist);//利用查找对应科室病房是否有空床
    if (p->process_stage == 4) {
        return;//不能重复入院
    }
  if (k==NULL) {
      printf(" The department has no available beds, hospitalization cannot be processed.\n");
      return;
  }
  else {
      p->process_stage = 4;//阶段进行到住院
      printf("Please enter the admisssion time\n");
      Date1 intime = Date_Input1();//输入入院时间
      p->inTime = intime;
      p->ward_no=Check2(k->roomID);//填写病房编号
      p->ishospital = 1;//住院
      bed* firstfree = findfirstfreebeds(k);
      k->freebeds--;//空闲床数减一
      firstfree->isfree = false;//床由空变为不空
      char str[20];
      strcpy(str, p->patient_number);//将患者编号赋值给字符串
      int id = Check2(str);
      ppatient p1 = g_patientList.nextpatient;

      while (p1) {
          if (p1->patient_number == id)
          {
              break;
          }
          p1 = p1->next_patient;
      }
      if (p1 != NULL)
          firstfree->patient = p1;
      while (1) {
          printf("Please enter the proposed hospital stay days\n");//输入拟住院天数
         int ret= scanf("%d", &p->planInDays);
         if (ret == 0) {
             printf("Input error\n");
             while (getchar() != '\n');
             continue;
         }
         else {
             break;
         }
      }
      int plandays = p->planInDays;
      double deposit = inputDeposit(plandays);//输入对应押金
      p->deposit = deposit;  
  }
}
// 办理出院结算
void HospitalDischarge(RecordNode* p, Fund_System* Account, WARD* wardlist)
{
    if (p->process_stage != 4 || p->is_valid != 1)
    {
        printf("The patient is not in hospitalized status; discharge processing is unavailable!\n");
        return;
    }

    // 1. 录入出院时间
    printf("Please enter discharge time:\n");
    p->outTime = Date_Input1();

    // 2. 计算实际住院天数（00:00-08:00出院当天不计费）
    p->realInDays = getRealWardDays(p->inTime, p->outTime);

    // 3. 计算住院总费用
    p->totalWardCost = p->realInDays * dailyfee;

    // ========== 逐日扣除押金（模拟每日08:00自动扣费） ==========
    // 每次扣200，若低于1000，autoDeductDeposit内部会自动打印警告
    int count = 0;
    printf("\n--- Processing daily deposit deduction (%d days) ---\n", p->realInDays);
    for (int i = 0; i < p->realInDays; i++) {
        autoDeductDeposit(&p->deposit);
        if (p->deposit < 1000) {
            count++;
        }
        if (p->deposit < 1000&&count==1) {
            printf("WARNING: Deposit is less than 1000, please recharge!\n");
        }
    }

    // 4. 出院结算：退还剩余押金（此时deposit已被逐日扣完）
    printf("\n=====================================\n");
    if (p->deposit >= 0)
    {
        printf("Refundable deposit: %.2f yuan\n", p->deposit);
    }
    else
    {
        printf("WARNING: Deposit insufficient! Outstanding payment: %.2f yuan\n", -p->deposit);
        printf("The patient needs to make up the difference before discharge.\n");

        // 若出院时押金为负，强制要求结清（医院不允许欠费出院）
        while (p->deposit < 0)
        {
            printf("Current balance: %.2f yuan. Recharge required to settle.\n", p->deposit);
            RechargeDeposit(&p->deposit);
        }
        if (p->deposit > 0) {
            printf("Refundable deposit after settlement: %.2f yuan\n", p->deposit);
        }
    }
    printf("=====================================\n");

    // 5. 住院费用计入医院资金账户
    Date billDate;
    billDate.year = p->outTime.year;
    billDate.month = p->outTime.month;
    billDate.day = p->outTime.day;
    Auto_Fund(Account, 1, p->totalWardCost, billDate);

    // 6. 出院状态更新与床位释放
    p->process_stage = 5;
    char temp[20];
    sprintf(temp, "%d", p->ward_no);
    ward* x = Searchward(wardlist, temp);
    if (x == NULL) {
        printf("Ward not found!\n");
        return;
    }
    x->freebeds++;

    int id = Check2(p->patient_number);
    bed* head = x->bedlist;
    while (head) {
        if (head->patient != NULL && head->patient->patient_number == id) {
            break;
        }
        head = head->next;
    }
    if (head == NULL) {
        printf("Error: Bed not found for this patient!\n");
        return;
    }
    head->isfree = true;
    head->patient = NULL;

    p->deposit = 0; // 剩余押金清零（已退还或充抵欠费）

    printf("Discharge successfully!\n");
}


void Exam(RecordNode* p, Fund_System *Account)//检查函数
{
    if (p == NULL || p->is_valid == 0)//判断是否为空且记录是否有效
    {
        printf("Invalid record, cannot add exam items!\n");
        return;
    }
    if (p->process_stage < 1)//判断是否已看完诊
    {
        printf("Please complete the diagnosis first!\n");
        return;
    }
    if (p->process_stage == 2) {//已检查完就不用检查了
        return;
    }
    printf("Please enter  exam time\n");
    Date1 examtime = Date_Input1();//输入检查完毕的时间
    Date Examtime;
    Examtime.year = examtime.year;
    Examtime.month = examtime.month;
    Examtime.day = examtime.day;
        printf("===== Enter Exam Items =====\n");
        //  清理输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        printf("Please input exam items: ");//输入检查项目
        fgets(p->exam_items, 100, stdin);
        int len = strlen(p->exam_items);
        if (len > 0 && p->exam_items[len - 1] == '\n')//去除换行符
        {
           p->exam_items[len - 1] = '\0';
        }
        double examfee;
        while (1) {
            printf("Please enter the exam fees:\n");
            int ret = scanf("%lf", &examfee);
            if (ret == 1) {
                break;
            }
            else {
                printf("Input error\n");
                while (getchar() != '\n');
                continue;
            }
        }
        p->examfees = examfee;//赋值检查费用
        Auto_Fund(Account, 1, examfee, Examtime);
    p->process_stage = 2;//阶段改为2
    printf("Exam items added successfully!\n");
}

void Treatment(RecordNode* p, Fund_System *Account)//治疗函数
{
    if (p == NULL || p->is_valid == 0)//判断是否为空且记录是否有效
    {
        printf("Invalid record, cannot add exam items!\n");
        return;
    }
    if (p->process_stage < 2)//判断是否已检查完
    {
        printf("Please complete the check first!\n");
        return;
    }
    if (p->process_stage == 3) {
        return;   //治疗完不再治疗
    }
    printf("Please enter the treatment time\n");
    Date1 treatmenttime = Date_Input1();//输入检查完毕的时间
    Date Treatmenttime;
    Treatmenttime.year = treatmenttime.year;
    Treatmenttime.month = treatmenttime.month;
    Treatmenttime.day = treatmenttime.day;
    printf("===== Enter treatment Items =====\n");
    //  清理输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf("Please input treatment items: ");//输入检查项目
    fgets(p->treatment_items, 100, stdin);
    int len = strlen(p->treatment_items);
    if (len > 0 && p->treatment_items[len - 1] == '\n')//去除换行符
    {
        p->treatment_items[len - 1] = '\0';
    }
    double treatmentfee;
    while (1) {
        printf("Please enter the treatment fees:\n");
        int ret = scanf("%lf", &treatmentfee);
        if (ret == 1) {
            break;
        }
        else {
            printf("Input error\n");
            while (getchar() != '\n');
            continue;
        }
    }
    p->treatmentfees = treatmentfee;//赋值治疗费用
    Auto_Fund(Account, 1, treatmentfee, Treatmenttime);
    p->process_stage = 3;
    printf("Treatment items added successfully!\n\n");
}
