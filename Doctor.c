// ====================  医生链表操作 ====================
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Define.h"
#include"Tool.h"
#include"Global.h"
#include"Doctor.h"
#include"Dept.h"

 DoctorNode* doctor_head = NULL;
 int doctor_count = 0;

 //创建一个医生的排班表节点
 ScheduleNode_Doctor* creat_schedule_node_Doc(){
     ScheduleNode_Doctor* node = (ScheduleNode_Doctor*)malloc(sizeof(ScheduleNode_Doctor));
    if (node == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    node->date.year = 0;
    node->date.month = 0;
    node->date.day = 0;
    node->treat_count = 0;  
    node->next = NULL;
	return node;
 }
 
 // 创建医生节点
DoctorNode* create_doctor_node() {
    DoctorNode* node = (DoctorNode*)malloc(sizeof(DoctorNode));
    if (node == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    strcpy(node->id, "");
    strcpy(node->name, "");

    node->level = 0;
    node->salary = 0.0;
    node->dept = 0;
 
    node->this_day_treat_number = 1;
    node->next = NULL;

    node->schedule_head = NULL;
    node->schedule_tail = NULL;

    return node;
}

// 给医生添加一天排班（尾插入队列，最新的放在队列尾）
void add_schedule_Doc(DoctorNode* doctor, Date date, int day_or_days) {
    ScheduleNode_Doctor* node = creat_schedule_node_Doc();
    if (node == NULL) return;

    node->date = date;
    //判断是add医生的加过去n天，还是修改医生里的新增一天
    if (day_or_days == 1) {
         node->treat_count = doctor->this_day_treat_number; //默认这一天的treat_count和当前this_day_treat_number一致，并记录
    }
    else if (day_or_days == 2) {
		printf("Enter the treat_count for the day :  ");
        node->treat_count = Num_Input(0, 20, "treat_count (0-20): ");
    }
    
    node->next = NULL;
    
    if (doctor->schedule_head == NULL) {
        doctor->schedule_head = node;
        doctor->schedule_tail = node;
    }
    else {
        doctor->schedule_tail->next = node;
        doctor->schedule_tail = node;
	}

    //计数
    int count = 0;
    ScheduleNode_Doctor* temp = doctor->schedule_head;

    while (temp != NULL) {
        count++;
        temp = temp->next;
    }

    // 如果超过30天，删除队头
    while (count > 30) {
        temp = doctor->schedule_head;
        doctor->schedule_head = doctor->schedule_head->next;
        if (doctor->schedule_head == NULL) {
            doctor->schedule_tail = NULL;
        }
        free(temp);
        count--;
    }
}


// 增加医生
void add_doctor() {
    DoctorNode* node = create_doctor_node();
    if (node == NULL) return;
  
    printf("\n===== Enter Doctor Info =====\n");
    //安全输入id
    printf("\nID=0i('i' is the department)+xy(code) \n");
        char temp_id[20];
        if (Id_Input(0, temp_id, "doctor") == NULL) {
			free(node);
            return;
        }
        strcpy(node->id, temp_id);

    //安全输入name
    char temp_name[25];
    if (Name_Input("\nName (Enter -1 to go back! ) :  ", temp_name) == NULL) {
        free(node);
        return;
    }
	strcpy(node->name, temp_name);

    //安全输入level
    int temp_level = Num_Input(0, 5, "\nLevel (0-Prof 1-Chief 2-Deputy 3-Attending 4-Resident 5-Intern)(Enter -1 to go back! ): ");
     if( temp_level== -1) {
        free(node);
        return;
    }
    node->level = temp_level;

    //安全输入salary
    double temp_salary = Salary_Input("\nSalary(Enter -1 to go back! ): ");
    if(temp_salary == -1) {
        free(node);
        return;
    }
	node->salary = temp_salary;

    //自动补齐department
    
	printf("\nDepartment is determined by the first digit of ID, automatically assigned as %s\n", get_doctor_level_name( node->id[1] - '0'));
    node->dept = node->id[1] - '0';

    add_doctor_to_dept(dept_list, node); //把医生添加到科室链表里，方便查询
        

    //给医生排班
    int test = schedule_Doc(node,0);
    if(test==-1) {
        free(node);
        return;
	}

    if (doctor_head == NULL) {
        doctor_head = node;
    }
    else {
        DoctorNode* p = doctor_head;
        while (p->next != NULL) p = p->next;
        p->next = node;
    }
    doctor_count++;
    printf("\nDoctor added successfully! Current count: %d\n", doctor_count);

}

// 删除医生
void delete_doctor() {
    char id[10];
    printf("\nEnter doctor ID to delete:\n ");
    char temp_id[20];
    
    if (Id_Input(1, temp_id, "doctor") == NULL) {             //输入一个想删除的医生的id
        printf("\nDeletion cancelled.\n");
		return;
    }
    strcpy(id, temp_id);


    DoctorNode* p = doctor_head;                
    DoctorNode* prev = NULL;

    while (p != NULL) {                 //开始检查遍历
        if (strcmp(p->id, id) == 0) {
            if (prev == NULL) {
                doctor_head = p->next;
            }
            else {
                prev->next = p->next;
            }
			//同时删除医生的排班链表，释放内存
			ScheduleNode_Doctor* schedule_temp = p->schedule_head;
            while (schedule_temp != NULL) {
                ScheduleNode_Doctor* temp = schedule_temp;
                schedule_temp = schedule_temp->next;
                free(temp);
            }

            free(p);
            doctor_count--;
            printf("\nDoctor %s deleted! Remaining: %d\n", id, doctor_count);
            return;
        }
        prev = p;
        p = p->next;
    }
    printf("\nDoctor with ID %s not found.\n", id);
    return;
}

//  修改医生信息      仅可通过id查询医生，支持精确修改姓名，级别，薪资，日期排班
void modify_doctor() {
    char id[10];
    printf("\nEnter doctor ID to modify\n");
	char temp_id[20];

    if (Id_Input(2, temp_id, "doctor") == NULL) {
        printf("\nModification cancelled.\n");
        return;
    }

    strcpy(id, temp_id);

    DoctorNode* p = doctor_head;
    while (p != NULL) {
        if (strcmp(p->id, id) == 0) {
			Show_Doctor(p);

            printf("\n1-Modify Name 2-Modify Level 3-Modify Salary 4-Modify Schedule 0-Return\n");
            int choice;
            while (1) {
                printf("Choice: ");

                if ((choice = Single_number_input()) == -1) {                //鲁棒性：直到输入单个数字为止

                    continue;
                }
                break;
            }
            switch (choice) {
            case 1:
            {
                char temp_name[25];
                strcpy(p->name, Name_Input("New Name: ", temp_name));
                printf("Modified successfully!\n");
                break;
            }
            case 2: {
                p->level = Num_Input(0, 5, "\nNew Level (0-Prof 1-Chief 2-Deputy 3-Attending 4-Resident 5-Intern): ");
                printf("Modified successfully!\n");
                break;
            }
            case 3: {
                printf("\nNew Salary: ");
                p->salary = Salary_Input("New Salary: ");
                printf("Modified successfully!\n");
                break;
            }
          
            case 4: {
			
				int test = schedule_Doc(p,1); //新增一天排班，排班函数里会自动删除超过30天的排班

                if(test==-1) {
                    printf("\nModification cancelled.\n");
                    break;
                }

                printf("\nModified successfully!\n");
                break;
			}
            case 0:
                return;
            }
            return;
        }
        p = p->next;
    }
    printf("Doctor not found.\n");
    return;
}

//  查询医生        
void query_doctor() {
    printf("\n===== Query Doctor =====\n");
    printf("\n1-By ID 2-By Name 3-By Dept 4-Show All 0-Return\n");    //支持通过id，姓名，科室查询，支持显示所有
    int choice=0;
    while (1) {
        printf("Choice: ");                         
        
        if ((choice = Single_number_input()) == -1) {                //鲁棒性：直到输入单个数字为止
           
            continue;
        }
        break;
    }
   
    char keyword[20];               
    int dept;
    int found = 0;
    DoctorNode* p = doctor_head;

    switch (choice) {
    case 1:
    {
        char temp_id[20];
        if (Id_Input(1, temp_id,"doctor") == NULL) {
            printf("Query cancelled.\n");
            return;
        }

        strcpy(keyword, temp_id);
     
        while (p != NULL) {                 //遍历医生链表

            if (strcmp(p->id, keyword) == 0) {     
              
                Show_Doctor(p);                 //找到编码为keyword的这个医生，并展示信息   
                found = 1;       //表示已找到
            }
            p = p->next;
        }
        if (!found) printf("\nNo matching records found.\n");
        break;
    }

    case 2: {
		char temp_name[25];
        if (Name_Input("Name: ", temp_name) == NULL) {
            printf("Query cancelled.\n");
            return;
        }
        strcpy(keyword, temp_name);      //输入的名字放在keyword
       
        while (p != NULL) {              //遍历医生链表
            if (strcmp(p->name, keyword) == 0) {
                Show_Doctor(p);                     //找到名字为keyword的这个医生，并展示信息
        
                found = 1;                      //表示已找到
            }
            p = p->next;
        }
        if (!found) printf("\nNo matching records found.\n");
        break;
    }

    case 3: {
		int temp_dept = Num_Input(1, 6, "\nDepartment(1 - 6)(Enter -1 to go back! ):");          //输入数字，放在temp_dept表示输入的科室
        if (temp_dept == -1) {
             printf("\nQuery cancelled.\n");
             return;
        }
        dept = temp_dept;
        while (p != NULL) {                         //遍历医生链表
            if (p->dept == dept) {
                Show_Doctor(p);                         //找到在dept科室里的所有医生，并展示他们信息
                found = 1;      //表示已找到
            }
            p = p->next;
        }
        if (!found) printf("\nNo matching records found.\n");
        break;
    }

    case 4: {
        printf("\n===== All Doctors List (Total %d) =====\n", doctor_count);            //展示所有医生信息
        if (p == NULL) {                                    //如果有0个医生
            printf("No doctors available.\n");
            break;
        }
        int i = 1;
        while (p != NULL) {                     //遍历医生链表，依次展示
            printf("%d. ", i++);
            Show_Doctor(p);
			printf("\n");
            p = p->next;
        }
        found = 1;                              //展开成功
        break;
    }

    case 0:
        return;

    default: {
             printf("Invalid choice. \n");
            return;
        }
    }

    return;
}

// 释放医生链表以及医生排班表链表的内存
void free_doctor_list() {
    DoctorNode* p = doctor_head;
    while (p != NULL) {
        DoctorNode* temp = p;
        p = p->next;
        ScheduleNode_Doctor* s = temp->schedule_head;
        while (s != NULL) {
            ScheduleNode_Doctor* t = s;
            s = s->next;
            free(t);
        }
        free(temp);
    }
    doctor_head = NULL;
    doctor_count = 0;
}