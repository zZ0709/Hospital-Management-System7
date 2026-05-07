// ==================== 护士链表操作 ====================
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Define.h"
#include"Struct.h"
#include"Tool.h"
#include"Global.h"
#include"Nurse.h"

 NurseNode* nurse_head = NULL;
 int nurse_count = 0;
 
 //创建一个护士的排班表节点
 ScheduleNode_Nurse* creat_schedule_node_Nurse() {
     ScheduleNode_Nurse* node = (ScheduleNode_Nurse*)malloc(sizeof(ScheduleNode_Nurse));
     if (node == NULL) {
         printf("Memory allocation failed!\n");
         return NULL;
     }

     node->date.year = 0;
     node->date.month = 0;
     node->date.day = 0;
     node->Is_work = 0;
     node->next = NULL;
     return node;
 }
 
 //  创建护士节点
NurseNode* create_nurse_node() {
    NurseNode* node = (NurseNode*)malloc(sizeof(NurseNode));
    if (node == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    strcpy(node->id, "");
    strcpy(node->name, "");
    node->level = 0;
    node->salary = 0.0;
    node->next = NULL;

    node->ward_number = 0;      //默认0病房数
    node->ward_head_id = NULL;

    node->schedule_head = NULL;
    node->schedule_tail = NULL;

    return node;
}

// 给护士添加一天排班（尾插入队列，最新的放在队列尾）
void add_schedule_Nurse(NurseNode* nurse, Date date,int day_or_days) {
    ScheduleNode_Nurse* node = creat_schedule_node_Nurse();
    if (node == NULL) return;

    node->date = date;
    if (day_or_days == 1) {
		node->Is_work = 0; //默认新添加的这一天是off状态
    }
    else if (day_or_days == 2) {
		printf("Is Nurse working on %d-%02d-%02d? (0: Off, 1: Work): ", date.year, date.month, date.day);
        int work_status;
        while (1) {
            work_status = Single_number_input();
            if (work_status == -1) {
                free(node);
                return;
            }
            if (work_status == 0 || work_status == 1) {
                node->Is_work = work_status;
                break;
            }
            printf("Invalid input. Please enter 0 or 1.\n");
        }
    }

    node->next = NULL;

    if (nurse->schedule_head == NULL) {
        nurse->schedule_head = node;
        nurse->schedule_tail = node;
    }
    else {
        nurse->schedule_tail->next = node;
        nurse->schedule_tail = node;
    }
    //计数
    int count = 0;
    ScheduleNode_Nurse* temp = nurse->schedule_head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }

    // 如果超过30天，删除队头
    while (count > 30) {
        temp = nurse->schedule_head;
        nurse->schedule_head = nurse->schedule_head->next;
        if (nurse->schedule_head == NULL) {
            nurse->schedule_tail = NULL;
        }
        free(temp);
        count--;
    }
}


//创建一个护士的病房节点
Nurse_ward_id* Creat_Ward_Id() {
    Nurse_ward_id* p = (Nurse_ward_id*)malloc(sizeof(Nurse_ward_id));
    if (p == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    strcpy(p->wards_id, "");
    p->next = NULL;
    return p;
}


// 增加护士
void add_nurse() {
    NurseNode* node = create_nurse_node();
    if (node == NULL) return;

    printf("\n===== Enter Nurse Info =====\n");
    //护士id
    char temp_id[20];
	printf("ID=0i('i' is the type of ward)+xy(code). \n");
	if (Id_Input(0, temp_id, "nurse") == NULL) {
        free(node);
        return;
    }
    strcpy(node->id, temp_id);
        

    //护士name
	char temp_name[25];
	if (Name_Input("Name(Enter -1 to go back! ) :  ", temp_name) == NULL) {
        free(node);
        return;
    }
    strcpy(node->name,temp_name);

    //护士level
    int level = Num_Input(0, 1, "Level (0-Head Nurse 1-Nurse): ");
    if (level == -1) {
        free(node);
		return;
    }
    node->level = level;
    
   //护士salary
	double temp_salary = Salary_Input("Salary (Enter -1 to go back! ) : ");
    if (temp_salary == -1) {
        free(node);
		return;
    }
    node->salary = temp_salary;

    //给护士排班
    int test= schedule_Nurse(node,0);
   if(test==-1) {
        free(node);
        return;
   }

   
    //护士负责病房数
	int number = Num_Input(0, 80, "Number of wards in charge of the nurse (Enter -1 to go back! ): ");
    if (number==-1) {
        free(node);
        return;
    }
    node->ward_number = number;


    //护士负责的病房
    for (int i = 0;i < node->ward_number;i++) {

        Nurse_ward_id* ward_id_node = Creat_Ward_Id();
        char temp_id[20];
		char temp_ward_id[20];
        strcpy(temp_ward_id, Id_Input(1, temp_id, "ward"));
        
        // 检测此病房是真实存在    
        if (Searchwardint(wardlist, temp_ward_id) == 0) {
			printf("Ward ID does not exist! Please re-enter.\n");
			free(ward_id_node);
            continue;
        }
		//检测此病房是否与护士的病房类型匹配
        if (temp_ward_id[1] != node->id[1]) {
            printf("Ward ID does not match nurse's ward type! Please re-enter.\n");
            free(ward_id_node);
			continue;
        }

		//检测此病房护士是否满员
		ward* temp_ward = Searchward(wardlist, temp_ward_id);
        if (temp_ward->nursenum >= MAX_WARD_NURSE) {
            printf("This ward already has %d nurses in charge! Please re-enter.\n", MAX_WARD_NURSE);
            free(ward_id_node);
			continue;
        }
        
        else {
            //把护士id放入病房里护士id链表，表示此护士负责这个病房了
			NurseIDNode* nurse_id_node = (NurseIDNode*)malloc(sizeof(NurseIDNode));
			if (nurse_id_node == NULL) {
                printf("Memory allocation failed!\n");
                free(ward_id_node);
                return;

            }
			strcpy(nurse_id_node->nurseid, node->id);

			if (temp_ward->nurselist == NULL) {
                temp_ward->nurselist = nurse_id_node;
            }
            else {
                NurseIDNode* p = temp_ward->nurselist;
                while (p->next != NULL) {
                    p = p->next;
                }
                p->next = nurse_id_node;
            }

            temp_ward->nursenum++;
        }

        //检测完成，把病房放入护士的病房id这个链表
		strcpy(ward_id_node->wards_id, temp_ward_id);


        if (node->ward_head_id == NULL) {
            node->ward_head_id = ward_id_node;
        }
        else {
            Nurse_ward_id* p = node->ward_head_id;
            while (p->next != NULL) {
                p = p->next;
            }
            p->next = ward_id_node;
            
        }
        printf("the ward is added successfully!\n");
    }
    if (nurse_head == NULL) {
        nurse_head = node;
    }
    else {
        NurseNode* p = nurse_head;
        while (p->next != NULL) p = p->next;
        p->next = node;
    }
    nurse_count++;
    printf("Nurse added successfully! Current count: %d\n", nurse_count);
}

// 删除护士
void delete_nurse() {
    char id[10];
    printf("\nEnter nurse ID to delete  : ");
    char temp_id[20];
    if( Id_Input(1, temp_id, "nurse")==NULL) {
        printf("Deletion cancelled.\n");
        return;
	}
	strcpy(id, temp_id);

    NurseNode* p = nurse_head;
    NurseNode* prev = NULL;

    while (p != NULL) {
        if (strcmp(p->id, id) == 0) {
            if (prev == NULL) {
                nurse_head = p->next;
            }
            else {
                prev->next = p->next;
            }
            //释放护士日期记录链表节点
            ScheduleNode_Nurse* schedule_temp = p->schedule_head;

            while (schedule_temp != NULL) {
                ScheduleNode_Nurse* temp = schedule_temp;
                schedule_temp = schedule_temp->next;
                free(temp);
            }

			//释放护士负责病房id链表节点
            Nurse_ward_id* ward_temp = p->ward_head_id;
            while (ward_temp != NULL) {
				ward* temp_ward = Searchward(wardlist, ward_temp->wards_id);

				//从病房的护士链表中移除该护士
				NurseIDNode* nurse_temp = temp_ward->nurselist;

				NurseIDNode* nurse_prev = NULL;

				while (nurse_temp != NULL) {
					if (strcmp(nurse_temp->nurseid, p->id) == 0) {
						if (nurse_prev == NULL) {
							temp_ward->nurselist = nurse_temp->next;
						} else {
							nurse_prev->next = nurse_temp->next;
						}
						free(nurse_temp);
						temp_ward->nursenum--;
						break;
					}
					nurse_prev = nurse_temp;
					nurse_temp = nurse_temp->next;
				}

                Nurse_ward_id* temp = ward_temp;
                ward_temp = ward_temp->next;
                free(temp);
            }
            free(p);
            nurse_count--;
            printf("Nurse %s deleted! Remaining: %d\n", id, nurse_count);
            return;
        }
        prev = p;
        p = p->next;
    }
    printf("Nurse with ID %s not found.\n", id);
}

// 修改护士
void modify_nurse() {
    char id[10];
    char temp_id[20];
    printf("Enter nurse ID to modify : ");
    if (Id_Input(2, temp_id, "nurse") == NULL) {

        printf("Modification cancelled.\n");
		return;
   }
	strcpy(id, temp_id);

    NurseNode* p = nurse_head;
    while (p != NULL) {
        if (strcmp(p->id, id) == 0) {

            Show_Nurse(p);
           
            printf("\n1-Modify Name 2-Modify Level 3-Modify Salary 4-Modify Wards 5-Modify Schedule 0-Cancel\n");
            int choice;
            while (1) {
                printf("Choice: ");

                if ((choice = Single_number_input()) == -1) {                //鲁棒性：直到输入单个数字为止

                    continue;
                }
                break;
            }

            switch (choice) {
            case 1: {
				char temp_name[25];
                if (Name_Input("\nNew Name (Enter -1 to go back! ) : ", temp_name) == NULL) {
                    printf("Modification cancelled.\n");
					break;
                }
				strcpy(p->name, temp_name);
                printf("Modified successfully!\n");
                break;             
            }
            case 2: {
				int level = Num_Input(0, 1, "\nNew Level (0-Nurse,1-Head_Nurse)(Enter -1 to go back! ): ");
                if (level == -1) {
                    printf("Modification cancelled.\n");
                    break;
                }
                p->level = level;
                printf("Modified successfully!\n");
                break;
            }
            case 3: {
                printf("\nNew Salary (Enter -1 to go back! ): ");
				double temp_salary = Salary_Input("New Salary: ");
                if (temp_salary==-1) {
                    printf("Modification cancelled.\n");
					break;
                }
                p->salary = temp_salary;
                printf("Modified successfully!\n");
                break;
            }
            case 4:
            {
                printf("\nModify Way:1-add wards,2-delete ward 0-cancel\n ");
                int choice1;
                while (1) {
                    printf("Choice: ");

                    if ((choice1 = Single_number_input()) == -1) {                //鲁棒性：直到输入单个数字为止
                       
                        continue;
                    }
                    break;
                }
                switch (choice1) {
                
                case 1: {
                    printf("\nPlease enter the ward number you want to add: ");

                    Nurse_ward_id* ward_id_node1 = Creat_Ward_Id();

                    char temp_id[20];
                    char temp_ward_id[20];
                    strcpy(temp_ward_id, Id_Input(1, temp_id, "ward"));

                    // 检测此病房是真实存在    
                    if (Searchwardint(wardlist, temp_ward_id) == 0) {
                        printf("Ward ID does not exist! Please re-enter.\n");
                        free(ward_id_node1);
                        continue;
                    }

                    //检测此病房是否与护士的病房类型匹配
                    if (temp_ward_id[1] != p->id[1]) {
                        printf("Ward ID does not match nurse's ward type! Please re-enter.\n");
                        free(ward_id_node1);
                        continue;
                    }
                    //检测此病房是否已经有护士负责
                    ward* temp_ward = Searchward(wardlist, temp_ward_id);
                    if (temp_ward->nurse_id[0] != '\0') {
                        printf("This ward already has a nurse in charge! Please re-enter.\n");
                        free(ward_id_node1);
                        continue;
                    }
                    else {
                        //把护士id放入病房节点，表示这个病房由这个护士负责
                        strcpy(temp_ward->nurseid, p->id);
                    }

					//检测完成，放入链表
					strcpy(ward_id_node1->wards_id, temp_ward_id);
                  

                    clear_input();
                    if (p->ward_head_id == NULL) {              //p是我们想修改的护士的节点
                        p->ward_head_id = ward_id_node1;
                        
                    }

                    else {
                        Nurse_ward_id* p_id = p->ward_head_id;
                        while (p_id->next!= NULL) {
                            p_id = p_id->next;
                        }
                        p_id->next = ward_id_node1;
                       

                    }
                    printf("the ward is added successfully!\n");        
                
                    (p->ward_number)++;
                          break;
                      }

                case 2: {
                    printf("\nPlease enter the ward number you want to delete: ");
                   
                    Nurse_ward_id* ward_id_node1 = Creat_Ward_Id();

                    char temp_id[20];
                    char temp_ward_id[20];
                    strcpy(temp_ward_id, Id_Input(1, temp_id, "ward"));

                    // 检测此病房是真实存在    
                    if (Searchwardint(wardlist, temp_ward_id) == 0) {
                        printf("Ward ID does not exist! Please re-enter.\n");
						free(ward_id_node1);
                        continue;
                    }

                    //检测此病房是否与护士的病房类型匹配
                    if (temp_ward_id[1] != p->id[1]) {
                        printf("Ward ID does not match nurse's ward type! Please re-enter.\n");
						free(ward_id_node1);
                        continue;
                    }

                    //检测此病房是否已经有护士负责
                    ward* temp_ward = Searchward(wardlist, temp_ward_id);
                    if (temp_ward->nurse_id[0] != '\0') {
                        printf("This ward already has a nurse in charge! Please re-enter.\n");
                        free(ward_id_node1);
                        continue;
                    }
                    

                    clear_input();
                    int found = 0;
                    Nurse_ward_id* prep = NULL, * p1 = p->ward_head_id;
                    if (p->ward_head_id == NULL) {
                        printf("No wards available!\n");
                        break;
                    }
                    if (strcmp(p1->wards_id, temp_ward_id) == 0) {
                        prep = p1;
                        p->ward_head_id = p1->next;
                        found = 1;
                        free(prep);

                    }
                    else {
                        prep = p1;
                        p1 = p1->next;
                        while (p1!=NULL) {
                            if (strcmp(p1->wards_id, temp_ward_id) == 0) {
                                prep->next = p1->next;
                                free(p1);
                                found = 1;
                                break;
                            }
                            prep = p1;
                            p1 = p1->next;
                        }
                    }
                    if (found) {
                        printf("the ward is deleted successfully!\n");
                        p->ward_number--;
                    }
                    else {
                        printf("This nurse is not in charge of this ward!\n");
                    }
                    break;
                }
               
                case 0: {
                    printf("Modification cancelled.\n");
                    break;
                }
                
                }
                break;
            }

            case 5: {
                int test = schedule_Nurse(p, 1); //新增一天排班，排班函数里会自动删除超过30天的排班
                if (test == -1) {
                    printf("Modification cancelled.\n");
                    break;
                }
                printf("Modified successfully!\n");
                break;
            }

            case 0:
                printf("Modification cancelled.\n");
                break;
            }
            return;
        }
        p = p->next;
    }
    printf("Nurse not found！\n");
}

// 查询护士
void query_nurse() {
    printf("\n===== Query Nurse =====\n");
    printf("1-By ID 2-By Name 3-Show All 0-Return\n ");
    int choice;
    while (1) {
        printf("Choice: ");

        if ((choice = Single_number_input()) == -1) {                //鲁棒性：直到输入单个数字为止

            continue;
        }
        break;
    }

    char keyword[20];
    int found = 0;
    NurseNode* p = nurse_head;

    switch (choice) {
    case 1: {
        char temp_id[20];
        if (Id_Input(1, temp_id, "nurse") == NULL) {                        //输入号码放在keyword
			printf("Query cancelled.\n");
            return;
        }

       strcpy(keyword, temp_id);

        while (p != NULL) {                 //遍历护士链表

            if (strcmp(p->id, keyword) == 0) {

                Show_Nurse(p);                 //找到编码为keyword的这个护士，并展示信息

                found = 1;       //表示已找到
            }
            p = p->next;
        }
        if (!found) {
            printf("No records found.\n");
        }
        break;
    }
    case 2: {
		char temp_name[25];
        if (Name_Input("Name (Enter -1 to go back! ) : ", temp_name) == NULL) {
            printf("Query cancelled.\n");
			return;
        }

        strcpy(keyword, temp_name);      //输入的名字放在keyword

        while (p != NULL) {              //遍历护士链表
            if (strcmp(p->name, keyword) == 0) {
                Show_Nurse(p);                     //找到名字为keyword的这个护士，并展示信息

                found = 1;                      //表示已找到
            }
            if (!found) {
                printf("No records found.\n");
            }
            p = p->next;
        }
        break;
    }
    
    
    case 3: {
        printf("\n===== All Nurses List (Total %d) =====\n", nurse_count);
        if (p == NULL) {
            printf("No nurses available.\n");
            break;
        }

        while (p != NULL) {
            found = 1;
            Show_Nurse(p);
            p = p->next;
        }
		if (!found) {
            printf("No records found.\n");
        }

        break;
    }
    case 0:
        return;
    default:
        printf("Invalid choice.\n");
    }

}

//释放护士链表
void free_nurse_list() {
    NurseNode* p = nurse_head;

    while (p != NULL) {
        NurseNode* temp = p;
        p = p->next;
        while (temp->ward_head_id != NULL) {
            Nurse_ward_id* temp_id = temp->ward_head_id;
            temp->ward_head_id = temp->ward_head_id->next;
            free(temp_id);
        }
        ScheduleNode_Nurse* s = temp->schedule_head;
        while (s != NULL) {
            ScheduleNode_Nurse* t = s;
            s = s->next;
            free(t);
        }

        free(temp);
    }
    nurse_head = NULL;
    nurse_count = 0;
}