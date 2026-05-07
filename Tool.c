#include <stdlib.h>
#include<stdio.h>    
#include<string.h>
#include<math.h>
#include"Tool.h"
#include"Define.h"
#include"Doctor.h"
#include"Nurse.h"
//  清空输入缓冲区
void clear_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//  获取级别名称（医生）
const char* get_doctor_level_name(int level) {
    const char* names[] = { "Professor", "Chief Physician", "Deputy Chief", "Attending", "Resident", "Intern" };
    if (level >= 0 && level <= 5) return names[level];
    return "Unknown";
}

//  获取级别名称（护士）
const char* get_nurse_level_name(int level) {
    const char* names[] = { "Nurse", "Head Nurse" };
    if (level >= 0 && level <= 1) return names[level];
    return "Unknown";
}

//检查医生/护士的id是否存在
int check_doctor_or_nurse_id_exists(char* id,const char *p) {
    
    if (strcmp(p, "doctor") == 0) {
        DoctorNode* p_temp = doctor_head;
        while (p_temp != NULL) {
            if (strcmp(p_temp->id, id) == 0) return 1;
            p_temp = p_temp->next;
        }
    }
    else if (strcmp(p, "nurse") == 0) {
        NurseNode* p_temp = nurse_head;
        while (p_temp != NULL) {
            if (strcmp(p_temp->id, id) == 0) return 1;
            p_temp = p_temp->next;
        }
    }
    
    return 0;
}


//输入保护——判断是否输入为纯数字
int IsPureDigit(const char* str) {
	if (str == NULL || *str == '\0')
		return 0;

	for (int i = 0;str[i] != '\0';i++) {
		if (!isdigit(str[i]))
			return 0;
	}
	return 1;
}

//输入保护——判断是否输入为纯英文字母
int IsPureEng(const char* str) {
    if (str==NULL||*str == '\0') {
        return 0;
    }
    for (int i = 0;str[i] != '\0';i++) {
        if (!isalpha(str[i])) {
            return 0;
        }
    }
    return 1;
}


// 输入保护——判断是否输入为单个数字，
int Single_number_input() {
    char buf[MENU_BUF_SIZE];
    if (fgets(buf, MENU_BUF_SIZE, stdin) == NULL) {
        if (feof(stdin)) {
            printf("Goodbye!\n");
            exit(0);           
        }
        printf("Invalid format. Please try again!\n");
        return -1;
    }

    buf[strcspn(buf, "\n")] = '\0';
    int len = 0;
    if ((len=strlen(buf) )!= 1) {
        if (len >= MENU_BUF_SIZE - 1) {
            clear_input();
        }
        printf("Invalid format. Please try again!\n");
        return -1;
    }
    if (!IsPureDigit(buf)) {
        printf("Invalid format. Please try again!\n");
        return -1;
    }
    
    return buf[0]-'0';
}

//输入保护——医生护士病房id
char* Id_Input(const int check, char  * temp_id, const char * type) {          
    
    while (1) {
        printf("ID(Enter -1 to go back! ): ");
		//防止输入为空行导致死循环，fgets返回NULL时检查是否为EOF，如果是EOF则正常退出，否则提示输入错误并继续循环
		//防止输入过长导致缓冲区溢出，fgets限制输入长度为LENTH_ID + 2（4位数字 + 换行符 + 结束符），如果输入长度超过这个限制则清空输入缓冲区并提示错误
       if (fgets(temp_id, LENTH_ID + 3, stdin) == NULL) {
            if (feof(stdin)) {
                printf("Goodbye!\n");
                exit(0);
            }
            printf("Invalid format. Please try again!\n");
            continue;
        }
		//去掉换行符
        temp_id[strcspn(temp_id, "\n")] = '\0';
		//-1则返回NULL，表示用户选择放弃输入，回到上一级菜单
		if (strcmp(temp_id, "-1") == 0) {
            printf("go back.\n");
            return NULL;
        }
		//检查输入长度是否为4，如果超过6（4位数字 + 换行符 + 结束符）则清空输入缓冲区并提示错误
        int len = 0;
        if ((len = strlen(temp_id)) != LENTH_ID) {
            if (len >= LENTH_ID + 2) {
                clear_input();
            }
            printf("Invalid format. Please try again!\n");
            continue;
        }
		//检查输入是否为纯数字
        if (!IsPureDigit(temp_id)) {
            printf("Invalid format. Please try again!\n");
            continue;
        }
		//检查输入格式是否正确（医生id首位必0，次位1到6；护士id首位必0，次位1到3）
		if (strcmp(type, "doctor") == 0) {
            if (!recheck_D(temp_id)) {
				//printf("Invalid format. Please try again!\n");
                continue;
            }
        }
        else if (strcmp(type, "nurse") == 0) {
            if (!recheck_N(temp_id)) {
				//printf("Invalid format. Please try again!\n");
                continue;
            }
        }
		else if (strcmp(type, "ward") == 0) {
            //病房id格式检查（首位1到6）
            if (temp_id[0] < '1' || temp_id[0] > '6') {
                printf("Invalid format. Please try again!\n");
                continue;
            }
        }

        if (check!=1) {
            if (check == 0) {
                if (check_doctor_or_nurse_id_exists(temp_id,type)) {
                    printf("Error: ID already exists!\n");
                    continue;
                }
            }
            else if (check == 2) {
                if (!check_doctor_or_nurse_id_exists(temp_id,type)) {
                    printf("Error: ID not found!\n");
                    continue;
                }
            }
        }
        break;
    }
    return temp_id;
}

//医生id合法性检查（首位必0，次位1到6）
int recheck_D(const char* id) {
    if (id[0] != '0') {
        printf("Invalid format. Please try again!\n");
        return 0;
    }
    if (id[1] < '1' || id[1] > '6') {
        printf("Invalid format. Please try again!\n");
        return 0;
    }
    return 1;
}

//护士id合法性检查（首位必0，次位1到3）
int recheck_N(const char* id) {
    if (id[0] != '0') {
        printf("Invalid format. Please try again!\n");
        return 0;
    }
    if (id[1] < '1' || id[1] > '3') {
        printf("Invalid format. Please try again!\n");
        return 0;
    }
    return 1;
}

//输入保护——医生护士姓名                  
char* Name_Input(const char * str,char * temp_name) {       
    while (1) {
        printf("%s",str);
        if (fgets(temp_name, 25, stdin) == NULL) {
            if (feof(stdin)) {
                printf("Goodbye!\n");
                exit(0);
            }
            printf("Invalid format. Please try again!\n");
            continue;
        }
        temp_name[strcspn(temp_name, "\n")] = '\0';
        if (strcmp(temp_name, "-1") == 0) {
            printf("go back.\n");
            return NULL;
        }
        if (strlen(temp_name) > 20) {
            if (strlen(temp_name) >=24) {
                clear_input();
            }
            printf("Name exceeds maximum length. Please try again!\n");
            continue;
        }
        if (!IsPureEng(temp_name)) {
            printf("Invalid format. Please try again!\n");
            continue;
        }
        return temp_name;
    }
}

//输入保护
int Num_Input(int begin,int end,const char * str) {
    char temp_Num[10];
    while (1) {
        printf("%s ",str);
        if (fgets(temp_Num, 10, stdin) == NULL) {
            if (feof(stdin)) {
                printf("Goodbye!\n");
                exit(0);
            }
            printf("Invalid format. Please try again!\n");
            continue;
        }
        temp_Num[strcspn(temp_Num, "\n")] = '\0';
        if (strcmp(temp_Num, "-1") == 0) {
            printf("go back.\n");
            return -1;
        }
        if (strlen(temp_Num) != 1) {
            if (strlen(temp_Num) == 9) {
                clear_input();
            }
            printf("Invalid format. Please try again!\n");
            continue;
        }
        if (!IsPureDigit(temp_Num)) {
            printf("Invalid format. Please try again!\n");
            continue;
        }
        if (*temp_Num < (begin+'0') || *temp_Num > (end+'0')) {
            printf("Invalid format. Please try again!\n");
            continue;
        }
        
        return temp_Num[0] - '0';
    }
}

//输入保护——薪资      
double Salary_Input(const char* str) {
    char temp_salary[50];
    double salary;
    while (1) {
        printf("%s ($): ",str);
        if (fgets(temp_salary, 50, stdin) == NULL) {
            if (feof(stdin)) {
                printf("Goodbye!\n");
                exit(0);

                }
                printf("Invalid format. Please try again!\n");
                continue;
           }
        temp_salary[strcspn(temp_salary, "\n")] = '\0';
        if (strcmp(temp_salary, "-1") == 0) {
            printf("go back.\n");
			return -1;
        }
        if (strlen(temp_salary) == 49) {
            clear_input();
            printf("Invalid format. Please try again!\n");
            continue;
        }
        char* endptr;
        salary=strtod(temp_salary, &endptr);
        if (*endptr != '\0'|| salary < 0) {
            printf("Invalid format. Please try again!\n");
            continue;
        }
        salary = round(salary * 100) / 100.0;               //四舍五入，精确到分
        return salary;
    }
}

//完整展示医生信息
void Show_Doctor(DoctorNode *p) {

    printf("\nID: %-8s Name: %-10s Dept: %d Level: %-18s Salary: $ %.2lf\nthis_day_treat_number %d\n",
        p->id, p->name, p->dept, get_doctor_level_name(p->level), p->salary,
        p->this_day_treat_number);

	ScheduleNode_Doctor* schedule_temp = p->schedule_head;

    printf("Schedule:");

	if (schedule_temp == NULL) {
        printf(" None\n");
        return;
    }

    while (schedule_temp != NULL) {
        printf(" %d-%02d-%02d ", schedule_temp->date.year, schedule_temp->date.month, schedule_temp->date.day);
		printf("( treat_count: %d       ) ",  schedule_temp->treat_count);
        schedule_temp = schedule_temp->next;
    }
    printf("\n");
	return;

}

//完整展示护士信息
void Show_Nurse(NurseNode* p) {
    printf("\nCurrent: %s %s %s Salary:%.2lf\n",
        p->id, p->name, get_nurse_level_name(p->level),
        p->salary);

    printf("Ward Number: %d\n",
        p->ward_number);

    if (p->ward_number == 0) {
        printf("Ward:none  ");
    }
    else {
        printf("Ward(%d):", p->ward_number);
        Nurse_ward_id* p_id = p->ward_head_id;
        while (p_id!=NULL) {
            printf(" %s ", p_id->wards_id);
            p_id = p_id->next;
        }
    }
    ScheduleNode_Nurse* schedule_temp = p->schedule_head;

    printf("\nSchedule:");
    while (schedule_temp != NULL) {
        printf(" %d-%02d-%02d ", schedule_temp->date.year, schedule_temp->date.month, schedule_temp->date.day);
		printf(" Is work: %s    ", schedule_temp->Is_work ? "Work" : "Off");
        schedule_temp = schedule_temp->next;
    }

	printf("\n");
    return;
}

//通过输入编号查找医生（输入编号）（未拿到id，需手动输入id时）	    功能：根据用户输入的ID，查找并返回对应的医生节点
DoctorNode* Find_Doctor_id( ) {
    char keyword[20];
    char temp_id[20];
    strcpy(keyword, Id_Input(1, temp_id, "doctor"));       //输入号码放在keyword
    return get_Doctor_id(keyword);
    
}

//通过输入编号查找医生(查找过程)  （已经拿到ID时）
DoctorNode* get_Doctor_id(const char* id) {
    DoctorNode* p = doctor_head;
    while (p != NULL) {
        if (strcmp(p->id, id) == 0) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

//通过输入编号查找护士（未拿到id，需手动输入id时）		功能：根据用户输入的ID，查找并返回对应的护士节点
NurseNode* Find_Nurse_id() {
    char keyword[20];
    char temp_id[20];
    strcpy(keyword, Id_Input(1, temp_id, "nurse"));       //输入号码放在keyword
    NurseNode* p = nurse_head;
    while (p != NULL) {
        if (strcmp(p->id, keyword) == 0) {
            return p;
        }
        p = p->next;
    }
	return NULL;
}

//通过输入编号查找护士(查找过程)（已经拿到ID时）
NurseNode* get_Nurse_id(const char* id) {
    NurseNode* p = nurse_head;
    while (p != NULL) {
        if (strcmp(p->id, id) == 0) {
            return p;
        }
        p = p->next;
    }
	return NULL;
}

//医生的排班函数（is_add表示是否为新增仅一天排班）
int schedule_Doc(DoctorNode *node,int Is_add) {
    
    int days = 0;
    if (Is_add) {
		printf("you can add a new schedule for the doctor, and the system will automatically delete schedules that exceed 30 days.\n");
		//days = Num_0_30_Input(" Added Scheduling Days (0-30)(Enter -1 to go back! ): ");
        //修改默认是加一天排班
        days = 1;
    }
    else {
        days = Num_0_30_Input("Scheduling Days (0-30)(Enter -1 to go back! ): ");
    }
    if (days == -1) {
        printf("go back.\n");
        return -1;
    }

    if (Is_add == 1) {
        Date* day = Create_DateNode();
        
        Date_Check(&New_Date, *day);
        //内部add_schedule_Doc形参加为1
        //add_schedule_Doc(node, Day_input(day));
        
        free(day);
        clear_input();
		return 0;
    }

    Date* day = Create_DateNode();

    for (int i = 0;i < days;i++) {
                
        add_schedule_Doc(node, Day_input(day),2);
	            	
    }
    free(day);
    clear_input();
    return 0;
}

//护士的排班函数
int schedule_Nurse(NurseNode* node, int Is_add) {
    int days = 0;
    if (Is_add) {
		printf("you can add a new schedule for the nurse, and the system will automatically delete schedules that exceed 30 days.\n");
		days = 1;
    }
    else {
        days = Num_0_30_Input("Scheduling Days (0-30)(Enter -1 to go back! ): ");
    }
    if (days == -1) {
        printf("go back.\n");
        return -1;
    }

	if (Is_add == 1) {
        Date* day = Create_DateNode();
        
        Date_Check(new_date, day);
        //内部add_schedule_Nurse形参加为1
        //add_schedule_Nurse(node, Day_input(day),1);
        
        free(day);
        clear_input();
        return 0;
    }

    Date* day = Create_DateNode();
        for (int i = 0;i < days;i++) {
        
            add_schedule_Nurse(node, Day_input(day),2 );
    
        }

        free(day);
      	clear_input();
        return 0;
    }

//安全输入0到30之间的整数，返回值为输入的整数
int Num_0_30_Input(const char* str) {
    char temp_Num[10];
    int num;
    while (1) {
        printf("Please Input %s", str);
        if (fgets(temp_Num, 10, stdin) == NULL) {
            if (feof(stdin)) {
                printf("Goodbye!\n");
                exit(0);
            }
            printf("Invalid format. Please try again!\n");
            continue;
        }
        temp_Num[strcspn(temp_Num, "\n")] = '\0';
         if (strcmp(temp_Num, "-1") == 0) {
            printf("go back.\n");
            return -1;
		 }
        if (strlen(temp_Num) == 9) {
            clear_input();
            printf("Invalid format. Please try again!\n");
            continue;
        }
        char* endptr;
        num = strtol(temp_Num, &endptr, 10);
        if (*endptr != '\0' || num < 0 || num > 30) {
            printf("Invalid format. Please try again!\n");
            continue;
        }
        return num;
	}
}

//安全输入0到20之间的整数，返回值为输入的整数
int Num_0_20_Input(const char* str) {
    char temp_Num[10];
    int num;
    while (1) {
        printf("Please Input Number: %s", str);
        if (fgets(temp_Num, 10, stdin) == NULL) {
            if (feof(stdin)) {
                printf("Goodbye!\n");
                exit(0);
            }
            printf("Invalid format. Please try again!\n");
            continue;
        }
        temp_Num[strcspn(temp_Num, "\n")] = '\0';
         if (strcmp(temp_Num, "-1") == 0) {
            printf("go back.\n");
            return -1;
         }
        if (strlen(temp_Num) == 9) {
            clear_input();
            printf("Invalid format. Please try again!\n");
            continue;
        }
        char* endptr;
        num = strtol(temp_Num, &endptr, 10);
        if (*endptr != '\0' || num < 0 || num > 20) {
            printf("Invalid format. Please try again!\n");
            continue;
        }
        return num;
	}
}

//临时day_input       安全输入一个day
Date Day_input(Date* day) {
    int y, m, d;
    bool valid;

    while (1) {
        printf("Enter the dates for scheduling (format: YYYY-MM-DD):\n");
        // 检查是否严格按照 x-x-x 格式输入，且输入3个整数
        if (scanf("%d-%d-%d", &y, &m, &d) == 3) {
            // 基础日期合法性校验
            if (m >= 1 && m <= 12 && d >= 1 && d <= 31) {
                valid = true;
            }
            else {
                valid = false;
            }

            if (valid) {
                day->year = y;
                day->month = m;
                day->day = d;
                break;
            }

            if (Date_Compare(day, New_Date) == 0) {
                printf("Invalid date! The date you entered is in the future.\n");
				continue;
            }
        }

        // 输入格式错误 / 日期不合法，清空缓冲区并提示
        while (getchar() != '\n'); // 清空错误输入

        printf("Invalid input! Please use the format YYYY-MM-DD and enter a valid date!\n");
    }
	


    printf("successfully added schedule for %d-%02d-%02d\n", day->year, day->month, day->day);
    


    Date_Check(New_Date, day);

    return *day;
}

//创建一个DateNode
Date* Create_DateNode() {
    Date* node = (Date*)malloc(sizeof(Date));
    if (node == NULL) {
		printf("Memory allocation failed!\n");
		return NULL;
    }
    node->year = 0;
    node->month = 0;
    node->day = 0;
	return node;
}

//医生薪资管理
void Salary_Doc_Management() {
    while (1) {
        printf("\n===== Salary Management =====\n");
        printf("1 : Modify salaries of all doctors with a fixed amount\n");
        printf("2:Modify salaries of one doctor\n");
        printf("0 : Return to previous menu\n");
        int choice = Num_Input(0, 2, "Choice: ");
        switch (choice) {
        case 1: {
            printf("Enter a new fixed amount for all Doctors (Enter -1 to go back! ) : ");

            char temp_salary[50];

            double the_fixed_amount = Salary_Input(temp_salary);

            DoctorNode* p_doc = doctor_head;
            while (p_doc != NULL) {
                Doc_Salary_Adjustment(p_doc, the_fixed_amount);
                p_doc = p_doc->next;
            }

            printf("Salaries updated for all Doctors.\n");
            break;
        }
        case 2: {
            DoctorNode* doctor = Find_Doctor_id();
            if (doctor == NULL) {
                printf("Doctor not found.\n");
                break;
            }
            printf("Doctor %s %s current salary: $%.2lf\n", doctor->id, doctor->name, doctor->salary);
            printf("Enter a new fixed amount for Doctor %s: ", doctor->id);
            char temp_salary[50];
            doctor->salary = Salary_Input(temp_salary);
            printf("Salary updated for Doctor %s: $%.2lf\n", doctor->id, doctor->salary);
            break;
        }
        case 0:
            return;
        }
    }
}

//护士薪资管理
void Salary_Nurse_Management() {
    while (1) {
        printf("\n===== Salary Management =====\n");
        printf("1 : Modify salaries of all nurses with a fixed amount\n");
        printf("2:Modify salaries of one nurse\n");
        printf("0 : Return to previous menu\n");

        int choice = Num_Input(0, 2, "Choice: ");

        switch (choice) {
        case 1: {
            printf("Enter a new fixed amount for all Nurses : ");
            char temp_salary[50];
            double the_fixed_amount = Salary_Input(temp_salary);

            NurseNode* p_nurse = nurse_head;
            while (p_nurse != NULL) {
                Nurse_Salary_Adjustment(p_nurse, the_fixed_amount);
                p_nurse = p_nurse->next;
            }
            printf("Salaries updated for all Nurses.\n");
            break;
        }
        case 2: {
            NurseNode* nurse = Find_Nurse_id();
            if (nurse == NULL) {
                printf("Nurse not found.\n");
                break;
            }
            printf("Nurse %s %s current salary: $%.2lf\n", nurse->id, nurse->name, nurse->salary);
            printf("Enter a new fixed amount for Nurse %s: ", nurse->id);
            char temp_salary[50];
            nurse->salary = Salary_Input(temp_salary);
            printf("Salary updated for Nurse %s: $%.2lf\n", nurse->id, nurse->salary);
            break;
        }
        case 0:
            return;
        }
	}
}

//医生薪资增减函数
void Doc_Salary_Adjustment(DoctorNode* doctor, double modify_salary) {
    doctor->salary +=  modify_salary;
	doctor->salary = round(doctor->salary * 100) / 100.0;               //四舍五入，精确到分
}

//护士薪资增减函数
void Nurse_Salary_Adjustment(NurseNode* nurse, double modify_salary) {
    nurse->salary += modify_salary;
	nurse->salary = round(nurse->salary * 100) / 100.0;               //四舍五入，精确到分
}

//dept的doctor_id节点creat
Doctor_Id* Create_Doctor_id_Node() {
    Doctor_Id* node = (Doctor_Id*)malloc(sizeof(Doctor_Id));
    if (node == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    node->id[0] = '\0';
    node->next = NULL;
	return node;
}

//修改医生某天treat_count数
int Modify_Schedule_Doc(DoctorNode* doctor, ScheduleNode_Doctor* schedule_head, ScheduleNode_Doctor* schedule_tail) {
    if (doctor == NULL) {
        printf("Doctor not found.\n");
        return -1;
    }

    Date day = Day_input(Create_DateNode());
    ScheduleNode_Doctor* temp = doctor->schedule_head;

    while (temp != NULL) {
        if (temp->date.year == day.year && temp->date.month == day.month && temp->date.day == day.day) {
            int new_treat_count = Num_0_20_Input("Enter new treat count for this day (0-20)(Enter -1 to go back! ): ");
            if (new_treat_count == -1) {
                printf("go back.\n");
                return -1;
            }
            temp->treat_count = new_treat_count;
            printf("Schedule for %d-%02d-%02d modified to treat_count: %d\n", day.year, day.month, day.day, temp->treat_count);
            return 0;
        }
        temp = temp->next;
    }
    printf("Schedule for %d-%02d-%02d not found.\n", day.year, day.month, day.day);
	return -1;
}

//修改（翻转一次）护士某天是否上班的排班函数       默认从off切换到work，再调用此切换函数则回到off态     翻转成功返回0，翻转失败返回-1
int Modify_Schedule_Nurse(NurseNode* nurse, ScheduleNode_Nurse* schedule_head, ScheduleNode_Nurse* schedule_tail) {
    if (nurse == NULL) {
        printf("Nurse not found.\n");
        return -1;
    }

    Date day = Day_input(Create_DateNode());

    ScheduleNode_Nurse* temp = nurse->schedule_head;

    while (temp != NULL) {
        if (temp->date.year == day.year && temp->date.month == day.month && temp->date.day == day.day) {
           
            temp->Is_work = !temp->Is_work; // 切换上班状态

            printf("Schedule for %d-%02d-%02d modified to: %s\n", day.year, day.month, day.day, temp->Is_work ? "Work" : "Off");
            return 0;
        }
        temp = temp->next;
    }
    printf("Schedule for %d-%02d-%02d not found.\n", day.year, day.month, day.day);
	return -1;
}