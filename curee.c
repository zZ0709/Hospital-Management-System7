#include<stdio.h>
#include"cure.h"
#include"ward.h"
#include"account.h"
// 公共查询遍历工具函数
// type:1查询打印  2删除  3修改
void recordSearchCommon(TreatmentSystem* sys, int type, WARD* wardlist, Fund_System* Account)
{
    int choice;
    char searchKey[50];
    int found = 0;
    char c, op;
    char id[20];
    RecordNode* p = sys->record_head;

    // 菜单选择 共用
    while (1)
    {
        printf("\n===== Menu =====\n");
        printf("1. Search by Create time\n");
        printf("2. Search by Patient Number\n");
        printf("3. Search by Doctor ID\n");
        printf("Enter your choice: ");
        int ret = scanf("%d", &choice);
        if (ret != 1)
        {
            printf("Input error\n");
            while ((c = getchar()) != '\n');
            continue;
        }
        else {
            switch (choice) {
            case 1:break;
            case 2:break;
            case 3:break;
            default:printf("Input error\n"); continue;
            }break;
        }
    }
    while ((c = getchar()) != '\n');

    // 空链表
    if (p == NULL)
    {
        printf("No records in system.\n");
        return;
    }

    // 按分类查询
    switch (choice)
    {
    case 1://通过创立时间进行选择
    {
        printf(" Enter record creation date to search  \n");

        Date1 time = Date_Input1();
        while (p != NULL)
        {
            if (p->is_valid == 1 && compare_time(p->create_time, time) == 0)
            {

                found = 1;//找到该节点
                // 分支：查询 / 删除 / 修改
                if (type == 1)//只输出信息
                {
                    printRecordNode(p);
                }
                else if (type == 2)//删除诊疗记录
                {
                    printRecordNode(p);
                    while (1) {
                        printf("Delete this record? 1.Yes 2.No:");
                        int ret = scanf("%d", &op);
                        if (ret != 1) {
                            printf("Input error\n");
                            while (getchar() != '\n');
                            continue;
                        }
                        else {
                            switch (op) {
                            case 1:break;
                            case 2:break;
                            default:printf("Input error\n"); continue;
                            }break;
                        }
                    }
                    while ((c = getchar()) != '\n');
                    if (op == 1)
                    {
                        p->is_valid = 0;//标记为无效
                        sys->record_count--;//有效诊疗记录数量减一
                        printf("Please enter the delete time\n");
                        Date time = Date_Input();//输入删除时间
                        UpdateAllBedsQueue(wardlist, time);//对床队列状态进行更新
                        double delTotal = p->examfees + p->treatmentfees + p->totalWardCost;
                        if (delTotal > 0) {
                            Auto_Fund(Account, 1, -delTotal, time);
                        }
                        printf("Delete successfully.\n");
                    }
                }
                else if (type == 3)//修改诊疗记录
                {
                    printRecordNode(p);
                    while (1) {
                        printf("Modify this record? 1.Yes 2.No:");
                        int ret = scanf("%d", &op);
                        if (!ret) {
                            printf("Input error\n");
                            while (getchar() != '\n');
                            continue;
                        }
                        else {
                            switch (op) {
                            case 1:break;
                            case 2:break;
                            default:printf("Input error\n"); continue;
                            }break;
                        }
                    }

                    while ((c = getchar()) != '\n');
                    if (op == 1)
                    {
                        // ========== 1. 先保存旧记录关键信息（后续会用到） ==========
                        Date1 old_create_time = p->create_time;
                        int old_stage = p->process_stage;
                        int old_ward_no = p->ward_no;
                        char old_pt_num[20];
                        strcpy(old_pt_num, p->patient_number);
                        double oldTotal = p->examfees + p->treatmentfees + p->totalWardCost;

                        // ========== 2. 先尝试创建新节点（旧记录保持不动） ==========
                        RecordNode* new_node = create_node();
                        if (!new_node) {
                            printf("Malloc failed. Modify aborted, original record remains intact.\n");
                            return;  // 旧记录完全未动，无需恢复
                        }

                        // ========== 3. 新节点创建成功，现在才进行破坏性操作 ==========

                        // 软删除旧记录
                        p->is_valid = 0;
                        sys->record_count--;

                        // 释放旧床位（如果住院中）
                        if (old_stage == 4 && old_ward_no != 0) {
                            char temp[20];
                            sprintf(temp, "%d", old_ward_no);
                            ward* x = Searchward(wardlist, temp);

                            if (x != NULL) {
                                int pid = Check2(old_pt_num);
                                bed* b = x->bedlist;
                                bool bed_found = false;

                                while (b != NULL) {
                                    if (b->patient != NULL && b->patient->patient_number == pid) {
                                        bed_found = true;
                                        if (b->isfree == false) {
                                            b->isfree = true;
                                            b->patient = NULL;
                                            x->freebeds++;
                                            printf("Bed released successfully.\n");
                                        }
                                        else {
                                            b->patient = NULL;
                                            printf("Warning: Bed already free, patient pointer cleared.\n");
                                        }
                                        break;
                                    }
                                    b = b->next;
                                }

                                if (!bed_found) {
                                    printf("Warning: Record shows hospitalized, but patient not found in ward %s.\n", temp);
                                }
                            }
                            else {
                                printf("Warning: Ward %s not found.\n", temp);
                            }
                        }

                        // 冲回旧资金
                        if (oldTotal > 0) {
                            Date oldDate = dateturn(old_create_time);
                            Auto_Fund(Account, 1, -oldTotal, oldDate);
                        }

                        // ========== 4. 填充新节点 ==========
                        new_node->create_time = old_create_time;

                        // 选择患者（必须存在，不能乱输）
                        extern tpatientlist g_patientList;
                        ppatientlist p2 = &g_patientList;
                        ppatient p0 = NULL;
                        ppatient p1;
                        char ptid[20];
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
                        // ===== 复诊状态处理 =====
                        if (strcmp(old_pt_num, new_node->patient_number) == 0) {
                            // 患者没变，继承原记录的复诊属性
                            new_node->is_revisit = p->is_revisit;
                        }
                        else {
                            // 换了患者，重新判断新患者是否有其他有效记录
                            RecordNode* checker = sys->record_head;
                            int has_history = 0;
                            while (checker != NULL) {
                                if (checker->is_valid == 1 && strcmp(checker->patient_number, new_node->patient_number) == 0) {
                                    has_history = 1;
                                    break;
                                }
                                checker = checker->next;
                            }
                            new_node->is_revisit = has_history ? 1 : 0;
                        }

                        DoctorNode* doc;
                        while (1) {
                            doc = Doctor_id();
                            if (doc == NULL) {
                                continue;
                            }
                            else {
                                strcpy(new_node->doctor_id, doc->id);
                                break;
                            }
                        }
                        strcpy(new_node->doctor_name, doc->name);

                        while (1) {
                            printf("Process stage（1.consultation 2.examination 3.treatment 4.hospitalization 5.discharge）：\n");
                            int ret = scanf("%d", &new_node->process_stage);
                            if (ret == 1) {
                                switch (new_node->process_stage) {
                                case 1:break;
                                case 2: Exam(new_node, Account); break;
                                case 3:Exam(new_node, Account); Treatment(new_node, Account); break;
                                case 4:Exam(new_node, Account); Treatment(new_node, Account); inhospital(new_node, wardlist); break;
                                case 5:Exam(new_node, Account); Treatment(new_node, Account); inhospital(new_node, wardlist); HospitalDischarge(new_node, Account, wardlist); break;
                                default:printf("Input error\n"); continue;
                                }break;
                            }
                            else {
                                printf("Input error\n");
                                while (getchar() != '\n');
                                continue;
                            }
                        }

                        new_node->is_valid = 1;
                        new_node->next = NULL;

                        // 新建节点后，直接接在原节点后面
                        new_node->next = p->next;
                        p->next = new_node;
                        sys->record_count++;
                        p = new_node;//让p指向新创建的节点，否则p=p->next会导致一个节点重复修改
                        printf("Please enter the modify time\n");
                        Date time = Date_Input();
                        UpdateAllBedsQueue(wardlist, time);
                        printf("Modify successfully\n");
                    }
                }

            }
            p = p->next;
        }if (found == 0) {
            printf("No records meets your demand\n");
            return;
        }
        break;
    }
    case 2://通过患者编号选择
    {
        while (1)
        {
            printf("Please enter the patient_id:\n");
            printf("You can enter to 48 characters\n");
            while ((c = getchar()) != '\n' && c != EOF);
            fgets(searchKey, 50, stdin);
            searchKey[strcspn(searchKey, "\n")] = '\0';
            if (strlen(searchKey) == 49)
            {
                printf("Your input is too long,please enter again\n");
                continue;
            }
            break;
        }
        while (p != NULL)
        {
            if (p->is_valid == 1 && strcmp(p->patient_number, searchKey) == 0)
            {
                found = 1;
                if (type == 1)//只输出信息
                {
                    printRecordNode(p);
                }
                else if (type == 2)//删除诊疗记录
                {
                    printRecordNode(p);
                    while (1) {
                        printf("Delete this record? 1.Yes 2.No:");
                        int ret = scanf("%d", &op);
                        if (ret != 1) {
                            printf("Input error\n");
                            while (getchar() != '\n');
                            continue;
                        }
                        else {
                            switch (op) {
                            case 1:break;
                            case 2:break;
                            default:printf("Input error\n"); continue;
                            }break;
                        }
                    }
                    while ((c = getchar()) != '\n');
                    if (op == 1)
                    {
                        p->is_valid = 0;//标记为无效
                        sys->record_count--;//有效诊疗记录减一
                        printf("Please enter the delete time\n");
                        Date time = Date_Input();//输入删除时间
                        UpdateAllBedsQueue(wardlist, time);//对床队列状态进行更新
                        double delTotal = p->examfees + p->treatmentfees + p->totalWardCost;
                        if (delTotal > 0) {
                            Auto_Fund(Account, 1, -delTotal, time);
                        }
                        printf("Delete successfully.\n");
                    }
                }

                else if (type == 3)//修改诊疗记录
                {
                    printRecordNode(p);
                    while (1) {
                        printf("Modify this record? 1.Yes 2.No:");
                        int ret = scanf("%d", &op);
                        if (!ret) {
                            printf("Input error\n");
                            while (getchar() != '\n');
                            continue;
                        }
                        else {
                            switch (op) {
                            case 1:break;
                            case 2:break;
                            default:printf("Input error\n"); continue;
                            }break;
                        }
                    }

                    while ((c = getchar()) != '\n');
                    if (op == 1)
                    {
                        // ========== 1. 先保存旧记录关键信息（后续会用到） ==========
                        Date1 old_create_time = p->create_time;
                        int old_stage = p->process_stage;
                        int old_ward_no = p->ward_no;
                        char old_pt_num[20];
                        strcpy(old_pt_num, p->patient_number);
                        double oldTotal = p->examfees + p->treatmentfees + p->totalWardCost;

                        // ========== 2. 先尝试创建新节点（旧记录保持不动） ==========
                        RecordNode* new_node = create_node();
                        if (!new_node) {
                            printf("Malloc failed. Modify aborted, original record remains intact.\n");
                            return;  // 旧记录完全未动，无需恢复
                        }

                        // ========== 3. 新节点创建成功，现在才进行破坏性操作 ==========

                        // 软删除旧记录
                        p->is_valid = 0;
                        sys->record_count--;

                        // 释放旧床位（如果住院中）
                        if (old_stage == 4 && old_ward_no != 0) {
                            char temp[20];
                            sprintf(temp, "%d", old_ward_no);
                            ward* x = Searchward(wardlist, temp);

                            if (x != NULL) {
                                int pid = Check2(old_pt_num);
                                bed* b = x->bedlist;
                                bool bed_found = false;

                                while (b != NULL) {
                                    if (b->patient != NULL && b->patient->patient_number == pid) {
                                        bed_found = true;
                                        if (b->isfree == false) {
                                            b->isfree = true;
                                            b->patient = NULL;
                                            x->freebeds++;
                                            printf("Bed released successfully.\n");
                                        }
                                        else {
                                            b->patient = NULL;
                                            printf("Warning: Bed already free, patient pointer cleared.\n");
                                        }
                                        break;
                                    }
                                    b = b->next;
                                }

                                if (!bed_found) {
                                    printf("Warning: Record shows hospitalized, but patient not found in ward %s.\n", temp);
                                }
                            }
                            else {
                                printf("Warning: Ward %s not found.\n", temp);
                            }
                        }

                        // 冲回旧资金
                        if (oldTotal > 0) {
                            Date oldDate = dateturn(old_create_time);
                            Auto_Fund(Account, 1, -oldTotal, oldDate);
                        }

                        // ========== 4. 填充新节点 ==========
                        new_node->create_time = old_create_time;

                        // 选择患者（必须存在，不能乱输）
                        extern tpatientlist g_patientList;
                        ppatientlist p2 = &g_patientList;
                        ppatient p0 = NULL;
                        ppatient p1;
                        char ptid[20];
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
                        // ===== 复诊状态处理 =====
                        if (strcmp(old_pt_num, new_node->patient_number) == 0) {
                            // 患者没变，继承原记录的复诊属性
                            new_node->is_revisit = p->is_revisit;
                        }
                        else {
                            // 换了患者，重新判断新患者是否有其他有效记录
                            RecordNode* checker = sys->record_head;
                            int has_history = 0;
                            while (checker != NULL) {
                                if (checker->is_valid == 1 && strcmp(checker->patient_number, new_node->patient_number) == 0) {
                                    has_history = 1;
                                    break;
                                }
                                checker = checker->next;
                            }
                            new_node->is_revisit = has_history ? 1 : 0;
                        }

                        DoctorNode* doc;
                        while (1) {
                            doc = Doctor_id();
                            if (doc == NULL) {
                                continue;
                            }
                            else {
                                strcpy(new_node->doctor_id, doc->id);
                                break;
                            }
                        }
                        strcpy(new_node->doctor_name, doc->name);

                        while (1) {
                            printf("Process stage（1.consultation 2.examination 3.treatment 4.hospitalization 5.discharge）：\n");
                            int ret = scanf("%d", &new_node->process_stage);
                            if (ret == 1) {
                                switch (new_node->process_stage) {
                                case 1:break;
                                case 2: Exam(new_node, Account); break;
                                case 3:Exam(new_node, Account); Treatment(new_node, Account); break;
                                case 4:Exam(new_node, Account); Treatment(new_node, Account); inhospital(new_node, wardlist); break;
                                case 5:Exam(new_node, Account); Treatment(new_node, Account); inhospital(new_node, wardlist); HospitalDischarge(new_node, Account, wardlist); break;
                                default:printf("Input error\n"); continue;
                                }break;
                            }
                            else {
                                printf("Input error\n");
                                while (getchar() != '\n');
                                continue;
                            }
                        }

                        new_node->is_valid = 1;
                        new_node->next = NULL;

                        // 新建节点后，直接接在原节点后面
                        new_node->next = p->next;
                        p->next = new_node;
                        sys->record_count++;
                        p = new_node;//让p指向新创建的节点，否则p=p->next会导致一个节点重复修改

                        printf("Please enter the modify time\n");
                        Date time = Date_Input();
                        UpdateAllBedsQueue(wardlist, time);
                        printf("Modify successfully\n");
                    }
                }
            }p = p->next;
        }
        if (found == 0) {
            printf("No records meets your demand\n");
            return;
        }
        break;
    }
    case 3://通过医生编号选择
    {
        while (1)
        {
            printf("Please enter the doctor_id:\n");
            printf("You can enter to 48 characters\n");
            while ((c = getchar()) != '\n' && c != EOF);
            fgets(searchKey, 50, stdin);
            searchKey[strcspn(searchKey, "\n")] = '\0';
            if (strlen(searchKey) == 49)
            {
                printf("Your input is too long,please enter again\n");
                continue;
            }
            break;
        }
        while (p != NULL)
        {
            if (p->is_valid == 1 && strcmp(p->doctor_id, searchKey) == 0)
            {
                found = 1;
                if (type == 1)//只输出信息
                {
                    printRecordNode(p);
                }
                else if (type == 2)//删除诊疗记录
                {
                    printRecordNode(p);
                    while (1) {
                        printf("Delete this record? 1.Yes 2.No:");
                        int ret = scanf("%d", &op);
                        if (ret != 1) {
                            printf("Input error\n");
                            while (getchar() != '\n');
                            continue;
                        }
                        else {
                            switch (op) {
                            case 1:break;
                            case 2:break;
                            default:printf("Input error\n"); continue;
                            }break;
                        }
                    }
                    while ((c = getchar()) != '\n');
                    if (op == 1)
                    {
                        p->is_valid = 0;//标记诊疗记录为无效
                        sys->record_count--;//有效诊疗记录数减1
                        printf("Please enter the delete time\n");
                        Date time = Date_Input();//输入删除时间
                        UpdateAllBedsQueue(wardlist, time);//对床队列状态进行更新
                        double delTotal = p->examfees + p->treatmentfees + p->totalWardCost;
                        if (delTotal > 0) {
                            Auto_Fund(Account, 1, -delTotal, time);
                        }
                        printf("Delete successfully.\n");
                    }
                }
                else if (type == 3)//修改诊疗记录
                {
                    printRecordNode(p);
                    while (1) {
                        printf("Modify this record? 1.Yes 2.No:");
                        int ret = scanf("%d", &op);
                        if (!ret) {
                            printf("Input error\n");
                            while (getchar() != '\n');
                            continue;
                        }
                        else {
                            switch (op) {
                            case 1:break;
                            case 2:break;
                            default:printf("Input error\n"); continue;
                            }break;
                        }
                    }

                    while ((c = getchar()) != '\n');
                    if (op == 1)
                    {
                        // ========== 1. 先保存旧记录关键信息（后续会用到） ==========
                        Date1 old_create_time = p->create_time;
                        int old_stage = p->process_stage;
                        int old_ward_no = p->ward_no;
                        char old_pt_num[20];
                        strcpy(old_pt_num, p->patient_number);
                        double oldTotal = p->examfees + p->treatmentfees + p->totalWardCost;

                        // ========== 2. 先尝试创建新节点（旧记录保持不动） ==========
                        RecordNode* new_node = create_node();
                        if (!new_node) {
                            printf("Malloc failed. Modify aborted, original record remains intact.\n");
                            return;  // 旧记录完全未动，无需恢复
                        }

                        // ========== 3. 新节点创建成功，现在才进行破坏性操作 ==========

                        // 软删除旧记录
                        p->is_valid = 0;
                        sys->record_count--;

                        // 释放旧床位（如果住院中）
                        if (old_stage == 4 && old_ward_no != 0) {
                            char temp[20];
                            sprintf(temp, "%d", old_ward_no);
                            ward* x = Searchward(wardlist, temp);

                            if (x != NULL) {
                                int pid = Check2(old_pt_num);
                                bed* b = x->bedlist;
                                bool bed_found = false;

                                while (b != NULL) {
                                    if (b->patient != NULL && b->patient->patient_number == pid) {
                                        bed_found = true;
                                        if (b->isfree == false) {
                                            b->isfree = true;
                                            b->patient = NULL;
                                            x->freebeds++;
                                            printf("Bed released successfully.\n");
                                        }
                                        else {
                                            b->patient = NULL;
                                            printf("Warning: Bed already free, patient pointer cleared.\n");
                                        }
                                        break;
                                    }
                                    b = b->next;
                                }

                                if (!bed_found) {
                                    printf("Warning: Record shows hospitalized, but patient not found in ward %s.\n", temp);
                                }
                            }
                            else {
                                printf("Warning: Ward %s not found.\n", temp);
                            }
                        }

                        // 冲回旧资金
                        if (oldTotal > 0) {
                            Date oldDate = dateturn(old_create_time);
                            Auto_Fund(Account, 1, -oldTotal, oldDate);
                        }

                        // ========== 4. 填充新节点 ==========
                        new_node->create_time = old_create_time;

                        // 选择患者（必须存在，不能乱输）
                        extern tpatientlist g_patientList;
                        ppatientlist p2 = &g_patientList;
                        ppatient p0 = NULL;
                        ppatient p1;
                        char ptid[20];
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
                        // ===== 复诊状态处理 =====
                        if (strcmp(old_pt_num, new_node->patient_number) == 0) {
                            // 患者没变，继承原记录的复诊属性
                            new_node->is_revisit = p->is_revisit;
                        }
                        else {
                            // 换了患者，重新判断新患者是否有其他有效记录
                            RecordNode* checker = sys->record_head;
                            int has_history = 0;
                            while (checker != NULL) {
                                if (checker->is_valid == 1 && strcmp(checker->patient_number, new_node->patient_number) == 0) {
                                    has_history = 1;
                                    break;
                                }
                                checker = checker->next;
                            }
                            new_node->is_revisit = has_history ? 1 : 0;
                        }
                        DoctorNode* doc;
                        while (1) {
                            doc = Doctor_id();
                            if (doc == NULL) {
                                continue;
                            }
                            else {
                                strcpy(new_node->doctor_id, doc->id);
                                break;
                            }
                        }
                        strcpy(new_node->doctor_name, doc->name);

                        while (1) {
                            printf("Process stage（1.consultation 2.examination 3.treatment 4.hospitalization 5.discharge）：\n");
                            int ret = scanf("%d", &new_node->process_stage);
                            if (ret == 1) {
                                switch (new_node->process_stage) {
                                case 1:break;
                                case 2: Exam(new_node, Account); break;
                                case 3:Exam(new_node, Account); Treatment(new_node, Account); break;
                                case 4:Exam(new_node, Account); Treatment(new_node, Account); inhospital(new_node, wardlist); break;
                                case 5:Exam(new_node, Account); Treatment(new_node, Account); inhospital(new_node, wardlist); HospitalDischarge(new_node, Account, wardlist); break;
                                default:printf("Input error\n"); continue;
                                }break;
                            }
                            else {
                                printf("Input error\n");
                                while (getchar() != '\n');
                                continue;
                            }
                        }

                        new_node->is_valid = 1;
                        new_node->next = NULL;

                        // 新建节点后，直接接在原节点后面
                        new_node->next = p->next;
                        p->next = new_node;
                        sys->record_count++;
                        p = new_node;//让p指向新创建的节点，否则p=p->next会导致一个节点重复修改
                        printf("Please enter the modify time\n");
                        Date time = Date_Input();
                        UpdateAllBedsQueue(wardlist, time);
                        printf("Modify successfully\n");
                    }
                }
            }
            p = p->next;
        }
        if (found == 0) {
            printf("No records meets your demand\n");
            return;
        }
        break;
    }
    }
}