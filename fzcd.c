#include<stdio.h>
#include"ward.h"
#include"cure.h"
#include"account.h"
void wardsystem(){
	ward* a;
	//initial(wardlist);
	//Openfile(wardlist,"bf.txt");//从文件中读取数据
	while (1) {
		Showmenu();
		int choice;
		while (1) {
			printf("Please enter the operation number:\n");
			scanf("%d", &choice);
			getchar();
			if (choice == 1 || choice == 2 || choice == 3 || choice == 4 || choice == 5 || choice == 6||choice==7) {
				break;
			}
			else {
				printf("Input error\n");
				continue;
			}
		}
		switch (choice) {
		case 1:Addward(wardlist);
			break;//添加病房信息
		case 2:Showward(wardlist); break;//显示病房信息
		case 3:Deleteward(wardlist); break;//删除病房信息
		case 4:Modifyward(wardlist); break;//修改病房信息
		case 5:Searchwardprintf(wardlist); break;//查询病房信息并输出
		case 6:savefile(wardlist,"bf.txt"); break;//保存到文件
		case 7: {
			for (int i = 0; i < 3; i++) {
				ward* a = wardlist[i].wardhead;
				
				while (a) {
					bed* b = a->bedlist;
					ward* a1 = a;
					while (b) {//释放床位
						bed* b1 = b;
						b = b->next;
						free(b1);
					}
					NurseIDNode* p = a->nurselist;
					while (p) {//释放护士编号链表
						NurseIDNode* p0 = p;
						p = p->next;
						free(p0);
					}
					a = a->next;
					free(a1);//释放堆区内存

				}
				wardlist[i].wardhead = NULL;
				
			}
			system("pause");
			system("cls");
			return;
			break;
		}//退出病房管理系统
		
		}
	}
}
void curesystem() {

	//init_system(&sys);//初始化函数
	//loadRecordsFromFile(&sys, "records.txt");//从文件中读取数据
	while (1) {
		cureShowmenu();
		int choice;
		while (1) {
			printf("Please enter the operation number:\n");
			scanf("%d", &choice);
			getchar();
			if (choice == 1 || choice == 2 || choice == 3 || choice == 4 || choice == 5 || choice == 6 || choice == 7 || choice == 8 || choice == 9) {
				break;
			}
			else {
				printf("Input error\n");
				continue;
			}
		}
		switch (choice) {
		case 1:add_record(&sys, wardlist); break;//添加诊疗记录
		case 2:printallrecordnode(&sys, wardlist); break;//显示并输出所有诊疗记录信息
		case 3:delete_record(&sys, wardlist, &Account); break;//删除诊疗记录
		case 4:modify_record(&sys, wardlist, &Account); break;//修改诊疗记录
		case 5:searchRecordprintf(&sys, wardlist, &Account); break; //通过创立时间或患者编号或医生编号三种方式查询对应诊疗记录并输出
		case 6:ChangeProcessStage(&sys, wardlist, &Account); break;//修改诊疗阶段
		case 7:ShowAllInHospitalPatients(&sys); break;// 显示所有住院患者信息 
		case 8:saveRecordsToFile(&sys,"records.txt");//将数据保存到文件中
			break;
		case 9: {
			RecordNode* head = sys.record_head;
			while (head) {
				RecordNode* p0 = head;
				head = head->next;
				free(p0);
			}
			sys.record_head = NULL;
			sys.record_count = 0;

		}
			  system("pause");
			  system("cls");
			  return;//退出诊疗管理系统
		}
	}
}