#include "patientinformation.h"

/*void query_doctor_by_dept(int target_dept) {//输入科室号打印医生信息
	
	DoctorNode* p = doctor_head;
	int found = 0;

	while (p != NULL) {
		
		if (p->dept == target_dept) {
			Show_Doctor(p);  
			found = 1;
		}
		
		p = p->next;
	}

	
	if (!found) {
		printf("There is currently no doctor information available in this department\n");
	}
}*/


ppatient creat_patientlist(ppatient head) {//需传入链表头， #默认原文件数据正确,数据个数不超范围,存历史数据非当日#
	int readcheck;//检验是否读取正确
	ppatient p, rear = head;
	FILE* fp = fopen(infile, "r");
	if (fp == NULL)
	{
		perror("Failed to open incorrect file");//文件打开失败提醒
		return head;
	}
	while (rear->next_patient != NULL) rear = rear->next_patient;//  先找到 head 链表目前的末尾，确保是从末尾开始接数据
	p = (ppatient)malloc(sizeof(tpatient));
	while ((readcheck = fscanf(fp, "%d %s %s %d %d %d %d %s %s",  &p->patient_number, p->patient_ID,p->patient_name,
		&p->patient_gender, &p->patient_birthday.year, &p->patient_birthday.  month, &p->patient_birthday. day, p->patient_phonenumber,
		 p->medical_history)) == 9) {//原文件存储形式：患者病号 身份证号码 姓名 性别 出生日期 电话 病史    
		rear->next_patient = p;
		p->next_patient = NULL;
		rear = p;

		p = (ppatient)malloc(sizeof(tpatient));
		if (p == NULL) {// 文件读取错误处理（解决：无法确保数据正确读写）
			printf("Error: Memory allocation failed while continuing to read\n");
			break;
		}

	}//while结束
	fclose(fp);//关闭文件 
	free(p);
	if (ferror(fp)) {
		printf("Error: Exception occurred while reading file\n");//处理文件读过程中的异常
	}
	else {
		if (readcheck == EOF)
			printf("Successfully read to the end of the text\n");
		else
			printf("Read prematurely ended\n");
	}

	printf("Initial patient information created successfully\n");
	return rear;//返回链表尾，便于后续插入
}

void initial_today_patient(patientlist p,pregistration head , Date day) {//########需要doctornumber初始化为0
	
		pregistration r, r0 = head->next_record;
		r = r0->next_record;
		while (r) {
			r0->next_record = r->next_record;
			free(r);
			r = r0->next_record;
		}
		head->next_record = NULL;
	p.today = day;
	p.outpatient_number = p.emergencypatient_number = p.totalpatient_number= 0;
	p.hospitalizedpatient_number = 0;//       ########需传当日病房总占用数########
	
}

int check_department(pregistration head, int patientnumber, int department) {
	pregistration p = head->next_record;
	while (p) {
		if (p->patient_number == patientnumber && (p->medical_number) / 100== department) {
			return 1;
		}
	}
	return 0;
}

Time find_last(pregistration head) {
	pregistration r = head->next_record;
	Time last;
	if (r) {
		last = r->time;
		r=r->next_record;
		while (r) {
			if (r->register_type == 0 && s_later(r->time, last))
				last = r->time;
			r = r->next_record;
		}
		return last;
	}
	else {
		last.hour = 0;
		last.minute = 0;
		return last;
	}
}


pregistration insert_registration_by_time(DoctorNode* q, pregistration p,pregistration rear) {
	pregistration r, r0 = (pregistration)q->schedule_head;//###默认是哨兵节点
	r = r0->next_doctor_record;
	
	while (r!= NULL && is_later(p->time, r->time)) {
		r0 = r;
		r = r->next_doctor_record;
	}

	p->next_doctor_record = r;
	r0->next_doctor_record = p;
	rear->next_record = p;
	p->next_record = NULL;
	rear = p;
	return rear;
}


ppatient search_patientlist(ppatient head,char id[19]) { 
	ppatient p;//p为p0后驱

	p = head->next_patient;


	while (p != NULL && strcmp(id, p->patient_ID) != 0) {
		
		p = p->next_patient;
	}
	if (p) {
		printf("The patient has been found\n");
		printf(" Number; % d   ID:% s   Name:% s   Gender:% d   Birthday:% d .%d.%d  Phonenumber: % s   Medical History% s", p->patient_number, p->patient_ID, p->patient_name,
			p->patient_gender, p->patient_birthday.year, p->patient_birthday.month, p->patient_birthday.day, p->patient_phonenumber,
			p->medical_history);
		return p;
	}
	else {

		return NULL;
	}

}

ppatient add_patientlist(patientlist list, ppatient head,ppatient rear, pregistration begin,pregistration end) {
	int choice;//用于判断患者选择急诊还是门诊
	int chance;//选择继续或者退出,判断预约时间是否正确
	int department;//科室编号
	char patientid[19];//患者身份证号
	int patientnumber;//病号
	
	DoctorNode* doctor;
	ppatient p,r;

	choice = get_int_range("Please choose the treatment method: 0 outpatient 1 emergency\n", 1, 2);
	switch (choice) {
	case 1:
		if (list.outpatient_number >= 120) {
			printf("The  number of outpatients exceeds the maximum capacity of the hospital, and the addition failed\n");
			return rear;
		}
		
		break;
	case 2:
		if (list.emergencypatient_number >= 80) {
		   	printf("The  number of emergency patients exceeds the maximum capacity of the hospital, and the addition failed\n");
			return rear;
		}
		
		break;

	};
	get_alpha_string("Please enter ID number number:\n", patientid,19);
	r=search_patientlist(head,patientid);
	if (r) {
		chance = get_int_range("Do you need to modify personal information: 0 No 1 Yes\n", 0,1);
		if(chance)
		modify_patient(head, patientid);
		patientnumber = r->patient_number;
		do{
			department = get_int_range("Please enter the department:/n", 1, 6);//###打印科室###
			if (check_department(begin, patientnumber, department) == 1) {
				printf("Cannot repeat registration in the same department.\n");
				chance = get_int_range("Please choose: 1. Re select department 2. Exit registration\n", 1, 2);
				if (chance == 2)
					return rear;
			}
			else {
				peintf("Department selection successful.\n");
				chance = 0;
			}
		}while (chance == 1);
	}
	else {
		p = (ppatient)malloc(sizeof(tpatient));

		p->patient_number = patientnumber = rear->patient_number + 1;

		strcpy(p->patient_ID, patientid);

		p->patient_gender = get_int_range("Please enter gender: 0 male 1 female\n", 0, 1);

		p->patient_birthday.year = get_int_range("Please enter year of birth: \n", 1876, 2016);

		p->patient_birthday.month = get_int_range("Please enter month of birth: \n", 1, 12);

		p->patient_birthday.day = get_int_range("Please enter day of birth: \n", 1, 31);//####检查28,29,30,31

		get_numeric_string_fixed("Please enter phone number:\n", p->patient_phonenumber, 12);


		get_alpha_string("Please enter medical history:\n", p->medical_history, maxhistory);
		department = get_int_range("Please enter the department:/n", 1, 6);
		peintf("Department selection successful.\n");
	}

	query_doctor_by_dept(department);//#####能不能只打印可以挂号的医生
	do {
		doctor = DoctorNode * Find_Doctor_id();//####
		if (doctor->number>= 20) {//#######待输入
			printf("The queue is full.\n");
			chance = get_int_range("Please choose: 1. Re select department 2. Exit registration\n", 1, 2);
			if (chance == 2)
				return rear;
		}
	} while (chance == 1);
	doctor->number++;//#######待输入,可以挂号
	pregistration record;
	Time latesttime;//现场挂号的最晚时间
	record = (pregistration)malloc(sizeof(tregistration));
	record->medical_number = department*100+doctor->number;
	strcpy(record ->doctor_id ,doctor->id);
	strcpy(record->medical_record, "0000000000");
	record->patient_number = patientnumber;
	record->deposit_balance= get_int_multiple_100("Please enter the deposit amount:\n");
	record->register_type= get_int_range("Please choose the registration method: 0 on-site registration 1 appointment registration\n", 0,1);
	if (record->register_type == 0) {
		record->time=input_time();
	}
	else {//预约需检验输入时间晚于最后一位现场登记者
		do {
			record->time = input_time();
			latesttime = find_last(doctor->schedule_head);
			chance = is_later(record->time,latesttime);
			if (chance)
				printf("Please enter a valid time.\n");
		} while (chance);
	
	}
	insert_registration_by_time(doctor,record,end);
	if (r == NULL) {//保存第一次挂号的患者基本信息
		rear->next_patient = p;
		rear = p;
		p->next_patient = NULL;
	}
	if (choice == 1) {//对应门、急诊人数变化
		list.outpatient_number++;
	}else {
		list.emergencypatient_number++;
	}


	
	
	printf("Registration successful\n");
	return rear;
}

void search_patient_name(ppatient head) {
	ppatient p = head->next_patient;
	char keyname[maxname];

	get_alpha_string("Please enter the patient's name for inquiry:\n", keyname, maxname);
	while (p != NULL) {
		if (strcmp(keyname, p->patient_name) == 0)
		{
			printf(" Number; % d   ID:% s   Name:% s   Gender:% d   Birthday:% d .%d.%d  Phonenumber: % s   Medical History% s",  p->patient_number, p->patient_ID, p->patient_name,
				p->patient_gender, p->patient_birthday.  year, p->patient_birthday.  month, p->patient_birthday. day, p->patient_phonenumber,
				p->medical_history);
		}
		p = p->next_patient;
	}

}




	

void modify_doctor(patientlist list) {
	pregistration p;
	int choice;//用于选择是否继续操作
	int number;//诊疗号
	char initialrecord[10];//保存最初诊疗记录
	DoctorNode* doctor; // 医生指针

	do {
		doctor = Find_Doctor_id(); 
		number = get_int_range("Please enter the patient visit ID to modify:/n", 1001, 6200);

		p = NULL; 
		if (doctor != NULL && doctor->schedule_head != NULL) {
			p = doctor->schedule_head->next_doctor_record; // 直接指向该医生的队列
			while (p) {
				if (p->medical_number == number) {
					break; // 找到了就跳出循环，此时 p 就是对应的诊疗节点
				}
				p = p->next_doctor_record;
			}
		}

		if (p) {
			strcpy(initialrecord, p->medical_record);

			get_char_medicalrecord(p->medical_record);
			if (p->medical_record[1] == '2' && (initialrecord[1] == '0' || initialrecord[1] == '1'))//#######医生首次录入，或者原本不住院现住院时住院人数加一
				list.hospitalizedpatient_number++;
			break;
		}



		choice = get_int_range("Do you want to continue modifying: 0 No 1 Yes\n", 0, 1);
	} while (choice);
}


void modify_patient(ppatient head,char id) {
	ppatient p;
	int choice;//用于选择是否继续操作
	do {
		p = search_patientlist(head,id);
		if (p) {

			choice = get_int_range("Please enter the information number you want to modify: 1.Name 2.Phone 3.Medical history \n", 1, 3);//生活实际中身份证，性别，出生日期不可轻易修改
			switch (choice) {
			case 1:
				get_alpha_string("Please enter your name:\n", p->patient_name, maxname);
				break;
			case 2:
				get_numeric_string_fixed("Please enter phone number:\n", p->patient_phonenumber, 12);
				break;
			case 3:
				get_alpha_string("Please enter medical history:\n", p->medical_history, maxhistory);
				break;
			}
		}
		else {
			printf("Error: The patient number does not exist\n");
		}


		choice = get_int_range("Do you want to continue modifying: 0 No 1 Yes\n", 0, 1);
	} while (choice);
}








void save_patient_data(ppatient head) {
	FILE* fp = fopen(outfile, "w");
	if (fp == NULL) {// 检查文件打开失败
		perror("Error: Failed to open output file for saving");
		return;
	}

	int write_check;
	ppatient curr_patient = head->next_patient;
	while (curr_patient != NULL) {
		write_check = fprintf(fp, "%d %s %s %d %d %d %d %s  %s \n",
			curr_patient->patient_number, curr_patient->patient_ID,curr_patient->patient_name,
			curr_patient->patient_gender, curr_patient->patient_birthday.year,
			curr_patient->patient_birthday.month, curr_patient->patient_birthday.day,
			curr_patient->patient_phonenumber,
			curr_patient->medical_history);

		if (write_check != 8) {
			printf("Error: Failed to write patient record (ID: %d) to file.\n", curr_patient->patient_number);
			fclose(fp);
			return;
		}
		curr_patient = curr_patient->next_patient;
	}

	// 最终底层流错误检查
	if (ferror(fp)) {
		printf("Error: A stream error occurred during file writing operations.\n");
		clearerr(fp); // 清除错误标志
	}
	else {
		printf("Success: All patient data and statistics have been successfully saved to %s\n", outfile);
	}

	// 检查文件关闭失败 (防止缓冲区数据丢失)
	if (fclose(fp) == EOF) {
		perror("Error: Failed to close the output file properly. Data might be corrupted");
	}
	
	void save_registration_data(pregistration head){
		FILE* fp = fopen(REG_OUTFILE, "w");
		if (fp == NULL) { // 检查文件打开失败
			perror("Error: Failed to open output file for saving");
			return;
		}

		int write_check;
		// 顺着总链表 (next_record) 遍历，把所有挂号记录写进去
		pregistration p = head->next_record;
		while (p != NULL) {
			// 写入 诊疗号, 病号, 医生工号, 挂号类型, 时, 分, 押金, 诊疗记录
			write_check = fprintf(fp, "%d %d %s %d %d %d %d %s\n",
				p->medical_number,
				p->patient_number,
				p->doctor_id,
				p->register_type,
				p->time.hour,
				p->time.minute,
				p->deposit_balance,
				p->medical_record);

			if (write_check < 0) { // fprintf返回值<0才代表底层写入失败
				printf("Error: Failed to write registration record (Medical No: %d) to file.\n", p->medical_number);
				fclose(fp);
				return;
			}
			p = p->next_record;
		}

		// 最终底层流错误检查
		if (ferror(fp)) {
			printf("Error: A stream error occurred during file writing operations.\n");
			clearerr(fp); // 清除错误标志
		}
		else {
			printf("Success: All registration data have been successfully saved to %s\n", REG_OUTFILE);
		}

		// 检查文件关闭失败 (防止缓冲区数据丢失)
		if (fclose(fp) == EOF) {
			perror("Error: Failed to close the output file properly. Data might be corrupted");
		}
	}