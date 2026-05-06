//文件的读取操作
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"Doctor.h"
#include"Define.h"
#include"Nurse.h"
#include"Struct.h"
#include"Tool.h"
#include"Global.h"
#include"Project.h"
#include"Dept.h"

// 保存医生到文本文件
void save_doctors() {

	// 以写入模式打开文件，如果文件不存在会创建，存在会覆盖
    FILE* fp = fopen(DOCTOR_FILE, "w");

	// 错误处理：如果文件无法打开，输出错误信息并返回
    if (fp == NULL) {
        printf("Error: Cannot create doctor file %s\n", DOCTOR_FILE);
        return;
    }

	// 第一行：保存医生总数，方便读取时知道要读多少条记录
    fprintf(fp, "%d\n", doctor_count);

	// 逐条写入医生信息，字段之间用空格分隔，行末换行
    DoctorNode* p = doctor_head;
    while (p != NULL) {
        fprintf(fp, "%s %s %d %.2f %d %d\n",
            p->id,           // 工号
            p->name,         // 姓名
            p->level,        // 级别
            p->salary,       // double 薪资
            p->dept,         // 科室
            p->this_day_treat_number        // 当前叫号
        );
        p = p->next;
    }

	// 关闭文件,输出保存医生的总数
    fclose(fp);
    printf("Doctor data saved (Total %d).\n", doctor_count);
}



// 从文本文件读取医生文件
void load_doctors() {
	// 以读取模式打开文件
    FILE* fp = fopen(DOCTOR_FILE, "r");

	// 错误处理：如果文件不存在，输出提示信息并返回（不创建空文件）
    if (fp == NULL) {
        printf("Doctor file does not exist, a new database will be created.\n");
        return;
    }

	// 第一行：读取医生总数，确保后续读取的记录数不超过这个值，防止文件损坏导致死循环
    int expected_count = 0;
    if (fscanf(fp, "%d\n", &expected_count) != 1) {
        fclose(fp);
        return;
    }

	// 读取前先清理现有医生链表，避免数据重复或内存泄漏
    free_doctor_list();

	// 逐行读取医生信息，直到达到预期的医生数量或文件末尾
    char line[256];
    DoctorNode* tail = NULL; 
    while (fgets(line, sizeof(line), fp) != NULL && doctor_count < expected_count) {
        DoctorNode* node = create_doctor_node();
        if (!node) break;
        int sched_count = 0;
            int items = sscanf(line, "%9s %19s %d %lf %d %d",
                node->id,           
                node->name,        
                &node->level,       
                &node->salary,     
                &node->dept,                    
                &node->this_day_treat_number       
            );

            if (items < 6) {  
                free(node);
                break;
            }
            if (items == EOF) {
				free(node);
                break;
            }

            if (items == 6) {
                if (doctor_head == NULL) {
                    doctor_head = node;
                    tail = node;
                }
                else {
                    tail->next = node;
                    tail = node;
                }
				//把医生添加到科室链表里，方便查询
				add_doctor_to_dept(dept_list, node); 
				//成功读取一条医生记录，医生数量加1
                doctor_count++;
            }
            else {
                // 格式错误，释放节点
                free(node);
            }
        }
    fclose(fp);
    printf("Successfully loaded %d doctors.\n", doctor_count);
}

// 保存护士到文本文件
void save_nurses() {
	// 以写入模式打开文件，如果文件不存在会创建，存在会覆盖
    FILE* fp = fopen(NURSE_FILE, "w");
	// 错误处理：如果文件无法打开，输出错误信息并返回
    if (fp == NULL) {
        perror("Failed to open nurse file for writing");
        return;
    }


    // 第一行：保存护士总数
    fprintf(fp, "%d\n", nurse_count);

    NurseNode* p = nurse_head;
    while (p != NULL) {
        
        fprintf(fp, "%s %s %d %.2f %d",
            p->id,           // 工号
            p->name,         // 姓名
            p->level,        // 级别
            p->salary,       // 薪资
  
            p->ward_number   // 负责病房数量
        );

        // 写所有病房 ID
        Nurse_ward_id* wid = p->ward_head_id;
        while (wid != NULL) {
            fprintf(fp, " %s", wid->wards_id);
            wid = wid->next;
        }

        fprintf(fp, "\n");

        p = p->next;
    }

    fclose(fp);
    printf("Nurse data saved (Total %d).\n", nurse_count);
}




//从文件输入护士     
void load_nurses() {
	// 以读取模式打开文件
    FILE* fp = fopen(NURSE_FILE, "r");
	// 错误处理：如果文件不存在，输出提示信息并返回（不创建空文件）
    if (fp == NULL) {
        printf("Failed to open nurse file\n");
        return;
    }

    int expected_count = 0;
    //检查 fscanf 返回值，确保读到整数
    if (fscanf(fp, "%d", &expected_count) != 1 || expected_count < 0) {
        printf("Invalid nurse count in file.\n");
        fclose(fp);
        return;
    }

	// 读取前先清理现有护士链表，避免数据重复或内存泄漏
    free_nurse_list();

    NurseNode* tail = NULL;
	// 增加护士数量上限校验，防止过多记录引发内存问题
    while (nurse_count < expected_count ) {
        NurseNode* node = create_nurse_node();
        int items = fscanf(fp, "%9s %19s %d %lf %d",
            node->id,           
            node->name,        
            &node->level,
            &node->salary,
			&node->ward_number
        );
        if (items == EOF) {
            // 文件正常读完
            free(node);
            break;
        }
        if (items == 5) {
            Nurse_ward_id* ward_tail = NULL;
            // 对 ward_number 进行合理范围校验，防止大数字导致内存溢出
            if (node->ward_number < 0 || node->ward_number > 100) {
                node->ward_number = 0;
            }
            bool ward_error = false;
            for (int i = 0; i < node->ward_number; i++) {
                
                char temp_id[16];
                // 读取病房ID时同样限制长度
                if (fscanf(fp, "%15s", temp_id) == 1) {
                    Nurse_ward_id* wid = Creat_Ward_Id();
					if (wid == NULL) {
                        ward_error = true;
                        break;
                    }
                    if (wid) {
                        //防止字符串截断溢出
                        strncpy(wid->wards_id, temp_id, sizeof(wid->wards_id) - 1);
                        wid->wards_id[sizeof(wid->wards_id) - 1] = '\0';
                        if (node->ward_head_id == NULL) {
                            node->ward_head_id = wid;
                            ward_tail = wid;
                        }
                        else {
                            ward_tail->next = wid;
                            ward_tail = wid;
                        }

                    }
                }
                else  {
                    // 读取失败，清理并跳出
                    printf("Warning: expected %d ward IDs, got %d\n",
                        node->ward_number, i);
                    ward_error = true;
                    break;
                }
            }
            if (ward_error) {
                // 统一清理
                while (node->ward_head_id != NULL) {
                    Nurse_ward_id* tmp = node->ward_head_id;
                    node->ward_head_id = tmp->next;
                    free(tmp);
                }
                free(node);
                continue;
            }
            if (nurse_head == NULL) {
                nurse_head = node;
                tail = node;
            }
            else {
                tail->next = node;
                tail = node;
            }
            nurse_count++;
        }
        else {
            // 如果格式不匹配，释放已申请的 node 内存，防止内存泄漏

            while (node->ward_head_id != NULL) { 
                // 释放可能已申请的部分病房节点
                Nurse_ward_id* tmp = node->ward_head_id;
                node->ward_head_id = node->ward_head_id->next;
                free(tmp);
            }
            free(node);
            break;
        }
    }

    fclose(fp);
    printf("Successfully loaded %d nurses.\n", nurse_count);
}


// 保存所有医生的排班
void save_doc_schedules() {
    FILE* fp = fopen(DOCTOR_SCHEDULE_FILE, "w");
    if (fp == NULL) {
        printf("Error: Cannot create schedule file\n");
        return;
    }

    DoctorNode* p = doctor_head;
    while (p != NULL) {
        
        // 数排班天数
        int count = 0;

        ScheduleNode_Doctor* s = p->schedule_head;
        while (s != NULL) {
            count++;
            s = s->next;
        }

        if (count > 0) {  
            // 有排班才写
            fprintf(fp, "%s %d", p->id, count);

            s = p->schedule_head;
            while (s != NULL) {
                fprintf(fp, " %d-%02d-%02d %d",
                    s->date.year, s->date.month, s->date.day,
                    s->treat_count);
                s = s->next;
            }
            fprintf(fp, "\n");
        }

        p = p->next;
    }

    fclose(fp);
    printf("Schedules saved.\n");
}


// 读取医生排班
void load_doc_schedules() {
    FILE* fp = fopen(DOCTOR_SCHEDULE_FILE, "r");
    if (fp == NULL) {
        printf("Schedule file does not exist, skipping schedule loading.\n");
        return;
    }

    char line[1024];
        while (fgets(line, sizeof(line), fp) != NULL) {
            char doc_id[10];
            int count = 0;

            // 读医生ID和排班数
            if (sscanf(line, "%9s %d", doc_id, &count) != 2) continue;

			DoctorNode* doc = NULL;
            // 找对应医生
                if (check_doctor_or_nurse_id_exists(doc_id, "doctor")) {
					doc = get_Doctor_id(doc_id);
                }
            else continue;  // 医生不存在，跳过

            // 清掉旧排班
            while (doc->schedule_head != NULL) {
                ScheduleNode_Doctor* tmp = doc->schedule_head;
                doc->schedule_head = tmp->next;
                free(tmp);
            }
            doc->schedule_tail = NULL;

            // 解析排班数据
            char* p = line;
            // 跳过前两个字段
            while (*p && *p != ' ') p++; while (*p == ' ') p++;  // id
            while (*p && *p != ' ') p++; while (*p == ' ') p++;  // count

            for (int i = 0; i < count && *p; i++) {
                int y, m, d, tc;
                if (sscanf(p, "%d-%d-%d %d", &y, &m, &d, &tc) != 4) break;

                ScheduleNode_Doctor* sn = (ScheduleNode_Doctor*)malloc(sizeof(ScheduleNode_Doctor));
                if (!sn) break;

                sn->date.year = y;
                sn->date.month = m;
                sn->date.day = d;
                sn->treat_count = tc;
                sn->next = NULL;

                // 尾插(最新的放在队列尾)
                if (doc->schedule_head == NULL) {
                    doc->schedule_head = sn;
                    doc->schedule_tail = sn;
                }
                else {
                    doc->schedule_tail->next = sn;
                    doc->schedule_tail = sn;
                }

                // 跳到下一个
                while (*p && *p != ' ') p++; while (*p == ' ') p++;  // 日期
                while (*p && *p != ' ') p++; while (*p == ' ') p++;  // 接诊数
            }
        }

        fclose(fp);
        printf("Schedules loaded.\n");
   
}

// 保存所有护士的排班
void save_nurse_schedules() {
    FILE* fp = fopen(NURSE_SCHEDULE_FILE, "w");
    if (fp == NULL) {
        printf("Error: Cannot create nurse schedule file\n");
        return;
    }

    NurseNode* p = nurse_head;

    while (p != NULL) {
        // 数排班天数
        int count = 0;
        ScheduleNode_Nurse* s = p->schedule_head;
        while (s != NULL) {
            count++;
            s = s->next;
        }

        if (count > 0) {  // 有排班才写
            fprintf(fp, "%s %d", p->id, count);

            s = p->schedule_head;

            while (s != NULL) {

                fprintf(fp, " %d-%02d-%02d %d",

                    s->date.year, s->date.month, s->date.day,
                    s->Is_work);
                s = s->next;
            }
            fprintf(fp, "\n");
        }
        p = p->next;
    }
    fclose(fp);
	printf("Nurse schedules saved.\n");
}

// 读取所有护士的排班
void load_nurse_schedules() {
    FILE* fp = fopen(NURSE_SCHEDULE_FILE, "r");
    if (fp == NULL) {
        printf("Schedule file does not exist, skipping schedule loading.\n");
        return;
    }
    char line[1024];
    while (fgets(line, sizeof(line), fp) != NULL) {
        char nurse_id[10];
        int count = 0;

        // 读护士ID和排班数
        if (sscanf(line, "%9s %d", nurse_id, &count) != 2) continue;

        // 找对应护士
        NurseNode* nurse = NULL;

        // 找对应护士
        if (check_doctor_or_nurse_id_exists(nurse_id, "nurse")) {
            nurse = get_Nurse_id(nurse_id);
        }

        else continue;  // 护士不存在，跳过


        // 清掉旧排班
        while (nurse->schedule_head != NULL) {
            ScheduleNode_Nurse* tmp = nurse->schedule_head;
            nurse->schedule_head = tmp->next;
            free(tmp);
        }
        nurse->schedule_tail = NULL;

        // 解析排班数据
        char* p = line;

        // 跳过前两个字段
        while (*p && *p != ' ') p++; while (*p == ' ') p++;  // id
        while (*p && *p != ' ') p++; while (*p == ' ') p++;  // count

        for (int i = 0; i < count && *p; i++) {
            int y, m, d, Is;
            if (sscanf(p, "%d-%d-%d %d", &y, &m, &d, &Is) != 4) break;

            ScheduleNode_Nurse* sn = (ScheduleNode_Nurse*)malloc(sizeof(ScheduleNode_Nurse));
            if (!sn) break;

            sn->date.year = y;
            sn->date.month = m;
            sn->date.day = d;
            sn->Is_work = Is;
            sn->next = NULL;

            // 尾插(最新的放在队列尾)
            if (nurse->schedule_head == NULL) {
                nurse->schedule_head = sn;
                nurse->schedule_tail = sn;
            }
            else {
                nurse->schedule_tail->next = sn;
                nurse->schedule_tail = sn;
            }

            // 跳到下一个
            while (*p && *p != ' ') p++; while (*p == ' ') p++;  // 日期
            while (*p && *p != ' ') p++; while (*p == ' ') p++;  // 接诊数
        }
    }

    fclose(fp);
    printf("Schedules loaded.\n");
}