#include "druginformation.h"

void creat_druglist()
{
	int readcheck;
	pdrug p;
	druglistrear = druglisthead;
	FILE *fp = fopen(GrugInfile, "r");
	if (fp == NULL)
	{
		perror("Failed to open incorrect file");
		return druglisthead;
	}
	while (druglistrear->next_drug != NULL)
		druglistrear = druglistrear->next_drug;

	p = (pdrug)malloc(sizeof(tdrug));
	// 假设新基础文件只包含6项：名称  状态 副作用 编号 进价 售价 总库存
	while ((readcheck = fscanf(fp, "%s %d %s %d %f %f %d",
							   p->drug_name, 
							   &p->is_active,
							    p->drug_sideeffect, 
								&p->drug_number, 
								&p->bid, 
								&p->price,
							   &p->drug_inventory)) == 7)
	{

		// 默认不干涉新数组，初始状态设为0，等待initial函数或实际操作时激活
		p->current_day_idx = 0;
		p->valid_days = 0;
		druglistrear->next_drug = p;
		p->next_drug = NULL;
		druglistrear = p;

		p = (pdrug)malloc(sizeof(tdrug));
		if (p == NULL)
		{
			printf("Error: Memory allocation failed\n");
			break;
		}
	}
	fclose(fp);
	free(p);
	printf("Initial drug information created successfully\n");
}

void creat_drugrecordlist()
{
	int readcheck;
	pdrug_record r;
	drugrecordrear = drugrecordhead;
	FILE *fp = fopen(RecordInfile, "r");
	if (fp == NULL)
	{
		perror("Failed to open record file");
		return drugrecordhead;
	}
	while (drugrecordrear->next_record != NULL)
		drugrecordrear = drugrecordrear->next_record;

	r = (pdrug_record)malloc(sizeof(tdrug_record));
	// 读取7项：病号 药品编号 售价 数量 年 月 日
	while ((readcheck = fscanf(fp, "%d %d %f %d %d %d %d",
							   &r->patient_number, &r->drug_number, &r->price, &r->drug_quantity,
							   &r->prescribe_date.year, &r->prescribe_date.month, &r->prescribe_date.day)) == 7)
	{

		drugrecordrear->next_record = r;
		r->next_record = NULL;
		drugrecordrear = r;

		r = (pdrug_record)malloc(sizeof(tdrug_record));
		if (r == NULL)
		{
			printf("Error: Memory allocation failed\n");
			break;
		}
	}
	fclose(fp);
	free(r);
	printf("Initial drug records created successfully\n");
}

void initial_today_drug(Date today) {
	pdrug p = druglisthead->next_drug;//用于遍历
	while (p) {
		p->current_day_idx = p->valid_days % 30;
		p->history[p->current_day_idx].inbound = p->history[p->current_day_idx].outbound = 0;
		p->history[p->current_day_idx].date = today;
		p->valid_days++;
		p = p->next_drug;
	}
	printf("Daily initialization completed.\n");
}

void add_druglist(Date today) {
	int first;//保存药品编号第一位数
	// 编号：第一位由病房管理人员输入，后两位系统自动依次编排
	int choice;
	do {
		pdrug p = (pdrug)malloc(sizeof(tdrug));
		if (p == NULL) {
			printf("Error: Memory allocation failed\n");
			
		}

		first = get_int_range("Please enter the department number associated with the drug:\n0 None 1 Internal Medicine 2 Surgery 3 Emergency Department 4 Obstetrics and Gynecology 5 Pediatrics 6 General Clinic\n", 0, 6);
		p->drug_number = first * 100 + (druglistrear->drug_number) % 100 + 1;

		get_alpha_string("Please enter the drug name:\n", p->drug_name, maxdrugname);

		get_alpha_string("Please enter drug side effects:\n", p->drug_sideeffect, maxeffect);
		p->bid = get_double_range("Please enter the bid (purchase price):\n", 0, 1000);
		p->price = get_double_range("Please enter the selling price:\n", 0, 1000);

		p->drug_inventory = get_int_range("Please enter the total inventory:\n", 0, 10000);
		p->current_day_idx = 0;
		p->valid_days = 1;
		p->is_active = 1;
		p->history[0].date = today;
		p->history[0].inbound = p->drug_inventory;
		p->history[0].outbound = 0;

		druglistrear->next_drug = p;
		druglistrear = p;
		p->next_drug = NULL;
		printf("Drug registration successful\n");
		choice = get_int_range("Do you want to continue adding: 0 No 1 Yes\n", 0, 1);
	} while (choice);
	
}

void search_drug_name() {
	pdrug p = druglisthead->next_drug;
	char keyname[maxdrugname];
	int found = 0; // 加一个标记，如果遍历完没找到可以给个提示
	int choice;
	do {
		get_alpha_string("Please enter the drug name for inquiry:\n", keyname, maxdrugname);

		while (p != NULL) {
			// 加上了安全的大括号，防悬挂缩进
			if (strcmp(keyname, p->drug_name) == 0) {
				printf("Name:%s Number:%3d Status:%d Side Effects: %s Bid:%.2f Price:%.2f Inventory:%d\n",
					p->drug_name, p->drug_number, p->is_active,p->drug_sideeffect,p->bid, p->price, p->drug_inventory);
					found = 1; // 标记已找到至少一个
			}
			p = p->next_drug; // 继续往后找，应对同名的药品
		}

		// 如果整条链表找完都没碰到名字匹配的
		if (!found) {
			printf("Notice: No drug found with the name '%s'.\n", keyname);
		}
		choice = get_int_range("Do you want to continue deleting: 0 No 1 Yes\n", 0, 1);
	} while (choice);
}

pdrug search_druglist() {//检索药品信息函数，利用唯一的编号查找
	pdrug p;
	int keydrugnumber;//被检索药品编号
	p = druglisthead->next_drug;

	keydrugnumber = get_int_range("Please enter the drug number to be queried:\n", 1, 999); // 接收返回值

	while (p != NULL && p->drug_number != keydrugnumber) {
		p = p->next_drug;
	}
	if (p) {
		printf("Retrieved:\nNumber:%3d Name:%s (Side Effects: %s) Bid:%.2f Price:%.2f Inventory:%d\n",
			p->drug_number, p->drug_name, p->drug_sideeffect,
			p->bid, p->price, p->drug_inventory);
		return p;
	}
	else {
		printf("Error: The drug number does not exist\n");
		return NULL;
	}
}

void delete_druglist() {	int choice;//用于判断操作结束
	pdrug p;
	do {
		p = search_druglist();
		if (p) {
			p->is_active = 0;//状态变为不可售
			printf("Successfully deleted\n");
		}

		choice = get_int_range("Do you want to continue deleting: 0 No 1 Yes\n", 0, 1);
	} while (choice);
}

void modify_druglist() {
	pdrug p;
	int choice;//用于选择是否继续操作
	do {
		p = search_druglist();
		if (p) {

			choice = get_int_range("Please enter the information number you want to modify: 1.Name 2.Side Effects 3.Bid 4.Price \n", 1, 4);//其他量不能人为随便改
			switch (choice) {
			case 1:

				get_alpha_string("Please enter the drug name:\n", p->drug_name, maxdrugname);
				printf("Modification successful.\n");
				break;
			case 2:

				get_alpha_string("Please enter drug side effects:\n", p->drug_sideeffect, maxeffect);
				printf("Modification successful.\n");
				break;
			case 3:
				p->bid = get_double_range("Please enter the bid (purchase price):\n", 0, 1000);
				printf("Modification successful.\n");
				break;
			case 4:
				p->price = get_double_range("Please enter the selling price:\n", 0, 1000);
				printf("Modification successful.\n");
				break;
			
			}
		}


		choice = get_int_range("Do you want to continue modifying: 0 No 1 Yes\n", 0, 1);
	} while (choice);
}

void outbound_drug(Date today) {//传入药品链表头
	int out, choice;//此次出库量,循环判断
	pdrug p;
	ppatient q, q0 = NULL;
	do {
		p = search_druglist();//查找对应的药
		if(p)
		{
			if (p->is_active == 0)
			{
				printf("This medicine is not available for sale\n");
			}
			else {
				q = search_patientlist();//查找出库药对应的患者
				if (q != NULL)
				{
					out = get_int_range("Please enter the outbound quantity:\n", 0, 10000);
					if (out > p->drug_inventory) {
						printf("Error: Insufficient inventory! Current inventory is %d.\n", p->drug_inventory);
					}
					else {
						p->history[p->current_day_idx].outbound += out;
						p->drug_inventory -= out;
						pdrug_record r = (pdrug_record)malloc(sizeof(tdrug_record));
						if (r == NULL) {
							printf("Error: Memory allocation failed\n");
							
						}
						r->patient_number = q->patient_number;
						r->drug_number = p->drug_number;
						r->price = p->price;
						r->drug_quantity = out;
						r->next_record = NULL;
						r->prescribe_date = today;
						druglistrear->next_record = r;
						druglistrear = r;
						printf("Outbound successful. Current inventory: %d\n", p->drug_inventory);
					}
				}
			}
			
		}
		

		choice = get_int_range("Do you want to continue modifying: 0 No 1 Yes\n", 0, 1);
	} while (choice);
	
}

void inbound_drug() {//传入药品链表头
	int in, choice;//此次出库量,循环控制
	pdrug p;
	do {
		p = search_druglist();
		if (p)
		{
			if (p->is_active == 0)
			{
				printf("This medicine is not available for sale\n");
			}
			else {

				in = get_int_range("Please enter the inbound quantity:\n", 0, 10000);
				p->history[p->current_day_idx].inbound += in;
				p->drug_inventory += in;
			}
		}

		choice = get_int_range("Do you want to continue modifying: 0 No 1 Yes\n", 0, 1);
	} while (choice);
}

void save_drug_data() {
	// 打开输出文件 (检查文件打开失败)
	FILE* fp = fopen(GrugOutfile, "w");
	if (fp == NULL) {
		perror("Error: Failed to open output file for saving");
		return;
	}

	int write_check;
	pdrug curr_drug = druglisthead->next_drug;
	while (curr_drug != NULL) {
		write_check = fprintf(fp, "%s %d %s %d %.2f %.2f %d \n",
			curr_drug->drug_name,curr_drug->is_active,curr_drug->drug_sideeffect,
			curr_drug->drug_number, curr_drug->bid, curr_drug->price,
			curr_drug->drug_inventory);

		// 检查药品记录写入是否成功 (处理磁盘满等异常)
		if (write_check < 0) {
			printf("Error: Failed to write drug record (No.%d) to file.\n", curr_drug->drug_number);
			fclose(fp);
			return;
		}
		curr_drug = curr_drug->next_drug;
	}

	// 最终底层流错误检查
	if (ferror(fp)) {
		printf("Error: A stream error occurred during file writing operations.\n");
		clearerr(fp); // 清除错误标志
	}
	else {
		printf("Success: All drug data have been successfully saved to %s\n", GrugOutfile);
	}

	// 检查文件关闭失败 (防止缓冲区数据丢失)
	if (fclose(fp) == EOF) {
		perror("Error: Failed to close the output file properly. Data might be corrupted");
	}
}

// 保存开药记录单
void save_drugrecord_data() {
	FILE* fp = fopen(RecordOutfile, "w");
	if (fp == NULL) {
		perror("Error: Failed to open record file for saving");
		return;
	}

	int write_check;
	pdrug_record curr_record = drugrecordhead->next_record;
	while (curr_record != NULL) {
		write_check = fprintf(fp, "%d %d %.2f %d %d %d %d\n",
			curr_record->patient_number,
			curr_record->drug_number,
			curr_record->price,
			curr_record->drug_quantity,
			curr_record->prescribe_date.year,
			curr_record->prescribe_date.month,
			curr_record->prescribe_date.day);

		if (write_check < 0) {
			printf("Error: Failed to write drug record to file.\n");
			fclose(fp);
			return;
		}
		curr_record = curr_record->next_record;
	}

	if (ferror(fp)) {
		printf("Error: A stream error occurred during file writing operations.\n");
		clearerr(fp);
	}
	else {
		printf("Success: All drug records have been successfully saved.\n");
	}
	if (fclose(fp) == EOF) {
		perror("Error: Failed to close the output file properly. Data might be corrupted");
	}
	fclose(fp);
}