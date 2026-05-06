#include<stdio.h>
#include"ward.h"
#include"cure.h"
#include"account.h"
WARD wardlist[3];//定义全局变量
int NurseCount(NurseIDNode* head) {
	int c = 0;
	while (head) {
		c++;
		head = head->next;
	}
	return c;
}

void PrintNurseList(ward* a) {
	if (!a || !a->nurselist) {
		printf("None\n");
		return;
	}
	NurseIDNode* n = a->nurselist;
	while (n) {
		NurseNode* nurse = get_Nurse_id(n->nurseid);
		if (nurse) printf("%s(%s) ", n->nurseid, nurse->name);
		else printf("%s(unknown) ", n->nurseid);
		n = n->next;
	}
}

void FreeNurseList(NurseIDNode* head) {
	while (head) {
		NurseIDNode* tmp = head;
		head = head->next;
		free(tmp);
	}
}

void AddNurseToWard(ward* w) {
	char nid[20];
	printf("Enter nurse ID to add: ");
	fgets(nid, 20, stdin);
	nid[strcspn(nid, "\n")] = '\0';
	if (strlen(nid) == 0) return;

	if (get_Nurse_id(nid) == NULL) {
		printf("Nurse not found!\n");
		return;
	}

	NurseIDNode* cur = w->nurselist;
	while (cur) {
		if (strcmp(cur->nurseid, nid) == 0) {
			printf("Nurse already assigned to this ward!\n");
			return;
		}
		cur = cur->next;
	}

	NurseIDNode* node = (NurseIDNode*)malloc(sizeof(NurseIDNode));
	if (!node) {
		printf("Memory allocation failed!\n");
		return;
	}
	strcpy(node->nurseid, nid);
	node->next = w->nurselist;
	w->nurselist = node;
	w->nursenum++;//护士总数增加
	printf("Nurse %s added successfully.\n", nid);
}

void RemoveNurseFromWard(ward* w) {//利用二级指针无需判断头节点
	char nid[20];
	printf("Enter nurse ID to remove: ");
	fgets(nid, 20, stdin);
	nid[strcspn(nid, "\n")] = '\0';
	if (strlen(nid) == 0) return;

	NurseIDNode** cur = &w->nurselist;
	while (*cur) {
		if (strcmp((*cur)->nurseid, nid) == 0) {
			NurseIDNode* tmp = *cur;
			*cur = (*cur)->next;
			free(tmp);
			w->nursenum--;//护士总数减少1
			printf("Nurse %s removed successfully.\n", nid);
			return;
		}
		cur = &(*cur)->next;
	}
	
	printf("Nurse not found in this ward!\n");
}

bed* Createbed(char *str) {//创建床位
	bed* b;
	 b = (bed*)malloc(sizeof(bed));
	 if (b == NULL) {
		 printf("Fatal Error: Memory allocation failed\n");
		 exit(EXIT_FAILURE);  // 或 return NULL;强制退出程序

	 }
	strcpy(b->bedID, str);
	b->isfree = 1;
	b->patient = NULL;
	b->next = NULL;//已经将尾部节点设为NULL，因此尾插时不用再将下一节点设为NULL
	InitQueue(&b->dayQueue);//初始化队列
	return b;
}
ward* prevfind(ward* p, WARD *wardlist,int i) {
	ward* a = wardlist[i].wardhead;
	if (a == p||a==NULL||p==NULL) {
		return NULL;
	}
	while (a) {
		if (a->next == p) {
			return a;
		}
		a = a->next;
	}
	return NULL;
}
void Openfile(WARD *wardlist,const char *filename) {//从文件中读取病房信息
	if (!wardlist || !filename) return;
	if (strlen(filename) == 0) {
		printf("Empty filename\n");
		return;
	}
	FILE* file;
		file = fopen(filename, "r");
		if (!file) {
			printf("Failed to open the file!\n");
			perror("Reason for failure\n");
		}
		
	
	ward* w; int count = 0;
	while (1) {
			w = (ward*)malloc(sizeof(ward));
			if (w== NULL) {
				printf("Fatal Error: Memory allocation failed\n");
				exit(EXIT_FAILURE);  // 或 return NULL;
			}
			
		int ret = fscanf(file, "%s %d %d %d %d %d", w->roomID, &w->wardtype, &w->department, &w->totalbeds, &w->freebeds, &w->nursenum);
		if (ret != 6) {
			free(w);
			break;
		}
		// 读取护士编号链表
		w->nurselist = NULL;
		for (int i = 0; i < w->nursenum; i++) {
			char nid[20];
			if (fscanf(file, "%19s", nid) != 1) break;
			NurseIDNode* node = (NurseIDNode*)malloc(sizeof(NurseIDNode));
			if (!node) break;
			strcpy(node->nurseid, nid);
			node->next = w->nurselist;//头插法
			w->nurselist = node;
		}
		int wardsum = wardlist[0].wardnum + wardlist[1].wardnum + wardlist[2].wardnum;
		if (wardsum >= wardmax) {
			printf("Ward limit reached\n");
			system("pause");
			return;
		   }
		
		switch (w->wardtype) {
		case 1:wardlist[0].wardnum++; break;
		case 2:wardlist[1].wardnum++; break;
		case 3:wardlist[2].wardnum++; break;

		}
		//建立病房链表
		if (wardlist[w->wardtype-1].wardhead == NULL) {//头节点是否有数据
			wardlist[w->wardtype - 1].wardhead = w;
			w->next = NULL;
		}
		else {
			ward* p = wardlist[w->wardtype - 1].wardhead;
			while (p->next != NULL) {//尾插法遍历节点
				p = p->next;
			}
			p->next = w;
			w->next = NULL;
		}
		w->bedlist = NULL;//建立床位链表，并使用尾插法插入节点
		for (int i = 0; i < w->totalbeds; i++) {
			char buf[20];
			sprintf(buf, "%d", i);
			bed* b = Createbed(buf);
			if (w->bedlist == NULL) {//尚未有节点
				w->bedlist = b;
			}
			else {
				bed* p = w->bedlist;
				while (p->next != NULL) {//遍历到尾部节点
					p = p->next;
				}
				p->next = b;
			}
		}count++;
	}

	fclose(file);
	if (count == 0)
		printf("No valid records loaded from %s\n", filename);
	else
		printf("Load successfully, %d records loaded from %s\n", count, filename);
	
}
void Addward(WARD *wardlist) {//添加病房
	ward* w;
	
		w = (ward*)malloc(sizeof(ward));
		if (w== NULL) {
			printf("Fatal Error: Memory allocation failed\n");
			exit(EXIT_FAILURE);  // 或 return NULL;
		}
	
	char str[20];
	int wardsum = wardlist[0].wardnum + wardlist[1].wardnum + wardlist[2].wardnum;
	if (wardsum >= wardmax) {
		printf("The total number of wards has reached the maximum!\n");
		system("pause");
		system("cls");
		return;
	}
	
	while (1) {//选择病房类型
		printf("Please enter the ward type:1.general ward,2.emergency ward,3.ICU ward\n");
		if (scanf("%d", &w->wardtype) == 1 && (w->wardtype == 1 || w->wardtype == 2 || w->wardtype == 3)) {

			if (w->wardtype == 1) { wardlist[0].wardnum++; }
			if (w->wardtype == 2) { wardlist[1].wardnum++; }
			if (w->wardtype == 3) { wardlist[2].wardnum++; }
			break;
		}
		else {
			
			printf("Input error\n ");
			while (getchar() != '\n');
			continue;
		}
	}
	while (1) {//输入相关科室
		printf("Please enter the department associated with the ward,just enter the number\n");
		printf("1.internal 2.sugery 3.child 4.gune 5.emergency 6.full\n");
		if (scanf("%d", &w->department)) {
			int ret = w->department;
			switch (ret) {
			case 1:break;
			case 2:break;
			case 3:break;
			case 4:break;
			case 5:break;
			case 6:break;
			default:printf("Input error\n"); continue;
			}
			break;
		}
		else {
			printf("Input error\n");
			while (getchar() != '\n');
			continue;
		}

	}
	// 遍历该类型链表，找同科室的最大序号（后两位）
	int max_seq = 0;
	ward* cur = wardlist[w->wardtype - 1].wardhead;
	while (cur) {
		// 只比较同科室的病房（roomID 第一位是科室编号）
		if (cur->roomID[0] - '0' == w->department) {
			int seq = Check2(cur->roomID) % 100;  // 取后两位序号
			if (seq > max_seq) max_seq = seq;
		}
		cur = cur->next;
	}

	// 新编号 = 科室*1000 + 类型*100 + (最大序号+1)
	int id = w->department * 1000 + w->wardtype * 100 + (max_seq + 1);
	sprintf(w->roomID, "%d", id);

	
	switch (w->wardtype) {//总床数
	case 3:w->totalbeds = icubeds; break;
	case 2:w->totalbeds = jzbeds; break;
	case 1:w->totalbeds = ptbeds; break;
	}
	w->freebeds = w->totalbeds;//空闲床数
	
	char temp[20];
	char keyward[20];
	w->nursenum = 0;//初始化护士
	w->nurselist = NULL;
	//病房链表
	if (wardlist[w->wardtype-1].wardhead == NULL) {//头节点是否有数据
		wardlist[w->wardtype-1].wardhead = w;
		w->next = NULL;
	}
	else {
		ward* p = wardlist[w->wardtype - 1].wardhead;
		while (p->next != NULL) {//遍历节点尾插法
			p = p->next;
		}
		p->next = w;
		w->next = NULL;
	}
	w->bedlist = NULL;//床位链表，并使用尾插法插入节点
	char c;
	for (int i = 0; i < w->totalbeds; i++) {
		char buf[20];
		sprintf(buf, "%d", i);
		bed* b = Createbed(buf);
		if (w->bedlist == NULL) {//尚未有节点
			w->bedlist = b;
		}
		else {
			bed* p = w->bedlist;
			while (p->next != NULL) {//遍历到尾部节点
				p = p->next;
			}
			p->next = b;
		}
	}
	printf("Ward added successfully\n");
	system("pause");
	system("cls");
}
int Check1(char* str, int min, int max) {
	if (str == NULL || *str == '\0') {//判断字符串是否为空
		return 0;
	}
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] < '0' || str[i]>'9') {//判断字符串是否为纯数字
			return 0;
		}
	}
	int length = strlen(str);
	if (length > 19) {//检测输入字符串是否超过数组长度限制
		return 0;
	}
	int num = 0;
	for (int i = 0; str[i] != '\0'; i++) {
		num = num * 10 + (str[i] - '0');//将字符串转化为整型
	}
	if (num >= min && num <= max) { return num; }//判断是否在范围内
	return 0;
}
int Check2(char* str) {
	if (str == NULL || *str == '\0') {//判断字符串是否为空
		return 0;
	}
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] < '0' || str[i]>'9') {//判断字符串是否为纯数字
			return 0;
		}
	}
	int length = strlen(str);
	if (length > 19) {//检测输入字符串是否超过数组长度限制
		return 0;
	}
	int num = 0;
	for (int i = 0; str[i] != '\0'; i++) {
		num = num * 10 + (str[i] - '0');//将字符串转化为整型
	}

	return num;
}


void Showward(WARD* wardlist) {//显示所有病房信息
	int count = 0; ward* p;
	for (int i = 0; i < 3; i++) {
		p = wardlist[i].wardhead;
		if (p == NULL) { count++; }
	}
	if (count == 3) {
		printf("No data available\n");
	}
	for (int i = 0; i < 3; i++) {
		p = wardlist[i].wardhead;
		while (p) {
			printfwarddata(p);
			p = p->next;
		}
		system("pause");
		system("cls");
	}
}
void printfwarddata(ward* a) {
	printf("wardID:%s ", a->roomID);
	printf("wardtype:%d ", a->wardtype);
	printf("totalbeds:%d ", a->totalbeds);
	printf("freebeds:%d ", a->freebeds);
	printf("department:%d \n", a->department);
	printf("nursenum:%d \n", a->nursenum);
	printf("Nurses: ");
	PrintNurseList(a);
	printf("\n");
}
void SearchwardbyID(WARD *wardlist) {//通过病房编号查询
	char id[20];
	while (1) {
		printf("Please enter the wardID that you want to search\n");
		fgets(id, 20, stdin);
		id[strcspn(id, "\n")] = '\0';
		if (strlen(id) == 19) {
			printf("Yout input is too long,please enter again\n");
			continue;
		}break;
	}
	int count = 0;
	for (int i = 0; i < 3; i++) {
		ward* p = wardlist[i].wardhead;
		while (p) {
			if (strcmp(id, p->roomID) == 0) {
				printfwarddata(p); count++;//输出对应病房信息
				break;
			}
			p = p->next;
		}
	}
	if (!count) {
		printf("Ward not found\n");//未找到对应病房
	}
}//通过病房类型查询
	void Searchwardbytype(WARD * wardlist) {
	char str[20]; int select;
	while (1) {
		printf("Please choose the wardtype that you want to search\n");//选择病房类型
		printf("1.general ward 2.emergency ward 3.ICU ward\n");
		printf("Please just enter the number\n");
		if (scanf("%d", &select)) { //判断输入的是否为1到3的数字
			
			switch (select) {
			case 1:break;
			case 2:break;
			case 3:break;
			default:printf("Input error\n"); continue;
			}break;
		}
		else {
			printf("Input error\n");
			while (getchar() != '\n');
			continue;
		}

	}
	int count=0;
	for (int i = 0; i < 3; i++) {

		ward* p = wardlist[i].wardhead;
		while (p) {
			if (p->wardtype==select) {
				printfwarddata(p); count++;//输出对应病房信息
				break;
			}
			p = p->next;
		}
	}
	if (!count) {
		printf("Ward not found\n");//未找到对应病房
	}
}
void Searchwardbykeshi(WARD *wardlist) {
	char str[20]; int select;
	while (1) {
		printf("Please choose the department that you want to search\n");//选择病房相关科室
		printf("1.internal 2.sugery 3.child 4.gune 5.emergency 6.full\n");
		printf("Please just enter the number\n");
		if (scanf("%d", &select)) { //判断输入的是否为1到6的数字
			
			switch (select) {
			case 1:break;
			case 2:break;
			case 3:break;
			case 4:break;
			case 5:break;
			case 6:break;
			default:printf("Input error\n"); continue;
			}break;
		}
		else {
			printf("Input error\n");
			while (getchar() != '\n');
			continue;
		}

	}
	int count = 0;
	for (int i = 0; i < 3; i++) {
		ward* p = wardlist[i].wardhead;
		while (p) {
			if (p->department == select) {
				printfwarddata(p); count++;//输出对应病房信息
				break;
			}
			p = p->next;
		}
	}
	if (!count) {
		printf("Ward not found\n");//未找到对应病房
	}
	
}
void  Searchwardbyfreebedsall(WARD *wardlist) {//查询有空床的病房
	int count = 0;
	for (int i = 0; i < 3; i++) {
		ward* a = wardlist[i].wardhead;
		while (a) {
			if (a->freebeds != 0) {
				count++;
				if (count == 1) {
					printf("Ward information with freebeds\n");
				}
				printfwarddata(a); 
			}a = a->next;
		}
	}
	if (!count) {
		printf("Ward not found\n");//未找到对应病房
	
	}
	
}
void Searchwardbynursename(WARD* wardlist) {
	char nurse_name[20];
	printf("Please enter the nurse's name: ");
	fgets(nurse_name, 20, stdin);
	nurse_name[strcspn(nurse_name, "\n")] = '\0';

	int count = 0;
	for (int i = 0; i < 3; i++) {
		ward* p = wardlist[i].wardhead;
		while (p) {
			bool found = false;
			NurseIDNode* n = p->nurselist;
			while (n) {
				NurseNode* nurse_info = get_Nurse_id(n->nurseid);
				if (nurse_info && strcmp(nurse_info->name, nurse_name) == 0) {
					found = true;
					break;
				}
				n = n->next;
			}
			if (found) {
				printfwarddata(p);
				count++;
			}
			p = p->next;
		}
	}
	if (!count) {
		printf("Ward not found\n");
	}
}

void Searchwardbynurseid(WARD* wardlist) {//查询对应护士编号的病房
	char nurseid[20];
	while (1) {
		printf("Please enter the nurse ID:\n");
		fgets(nurseid, 20, stdin);
		nurseid[strcspn(nurseid, "\n")] = '\0';
		if (strlen(nurseid) == 19) {
			printf("Input too long, please enter again\n");
			continue;
		}
		break;
	}

	int count = 0;
	for (int i = 0; i < 3; i++) {
		ward* p = wardlist[i].wardhead;
		while (p) {
			NurseIDNode* n = p->nurselist;
			while (n) {
				if (strcmp(n->nurseid, nurseid) == 0) {
					printfwarddata(p);
					count++;
					break; // 找到即可跳出内层链表，避免同一病房重复打印
				}
				n = n->next;
			}
			p = p->next;
		}
	}
	if (!count) {
		printf("Ward not found\n");
	}
}

ward * Searchwardbylxfreebeds(WARD* wardlist) {//查询对应科室病房是否有空床
	printf("Please enter the department you want to inquire about bed availablity\n");
	printf("1.internal 2.sugery 3.child 4.gune 5.emergency 6.full:\n");
	int choice = Int_Input(1, 6);//选择想查询的对应科室
	int count = 0; int deid; ward* p;
	for (int i = 0; i < 3; i++) {
		ward* a = wardlist[i].wardhead;
		while (a) {
			 deid = Check2(a->roomID) / 1000;
			if (a->freebeds != 0&&deid==choice) {
				count++;
				if (count == 1) {
					printf("Ward information with freebeds\n");
					 p = a;
				}
				printfwarddata(a);
			}a = a->next;
		}
	}
	if (!count) {
		printf("Ward not found\n");//未找到对应病房
		return NULL;
	}
	return p;
}
void Showmenu() {//菜单
	printf("Ward Management System\n");//病房管理系统
	printf("1.Add ward information\n");//添加病房信息
	printf("2.Show ward information\n");//显示所有病房信息
	printf("3.Delete ward information\n");//删除病房信息
	printf("4.Modify ward information\n");//修改病房信息
	printf("5.Query Ward information\n");//查询病房信息
	printf("6.Save to file\n");//将数据保存到文件中
	printf("7.Exit ward management system\n");//退出病房管理系统
}
void Deleteward(WARD *wardlist) {//删除病房信息
	int count = 0;
	for (int i = 0; i < 3; i++) {
		ward* a = wardlist[i].wardhead;
		if (a == NULL) { count++; }
	}
	if (count == 3) {
		printf("No ward information available ,cannot delete\n");//目前没有病房
	}
	char id[20]; int ret;
	while (1) {//判断输入是否合法
		printf("Please enter the wardID that you want to delete\n");
		fgets(id, 20, stdin);
		id[strcspn(id, "\n")] = '\0';
		if (strlen(id) == 19) {
			printf("Yout input is too long,please enter again\n");
			continue;
		}break;
	}
	ward* a = Searchward(wardlist, id);
	if (a == NULL) {
		printf("Ward not found\n");//病房不存在
		system("pause");
		system("cls");   return;
	}
	ward* b = prevfind(a, wardlist,a->wardtype-1);
	if (b == NULL) {
		wardlist[a->wardtype-1].wardhead = a->next;
	}
	else { b->next = a->next; }//删除该节点
	bed* bed1 = a->bedlist;
	while (bed1) {
		bed* bednext = bed1->next;
		free(bed1);
		bed1 = bednext;
	}
	wardlist[a->wardtype - 1].wardnum--;//病房总数减少1
	

	NurseIDNode* p = a->nurselist;
	while (p) {   //清空护士编号链表
		NurseIDNode* p1 = p;
		p = p->next;
		free(p1);
	}
	free(a);
	printf("Delete successfully\n");
	system("pause");
	system("cls");
}
void Modifyward(WARD* wardlist) {
	int count = 0;
	for (int i = 0; i < 3; i++) {
		ward* a = wardlist[i].wardhead;
		if (a == NULL) { count++; }
	}
	if (count == 3) {
		printf("No ward information available, cannot modify\n");  // 改文字
		return;  // 建议直接返回，不要继续执行
	}

	char id[20];
	while (1) {
		printf("Please enter the wardID that you want to modify\n");  // 改文字
		fgets(id, 20, stdin);
		id[strcspn(id, "\n")] = '\0';
		if (strlen(id) == 19) {
			printf("Your input is too long, please enter again\n");  // 拼写修正
			continue;
		}
		break;
	}

	ward* a = Searchward(wardlist, id);
	if (a == NULL) {
		printf("Ward not found\n");
		system("pause");
		system("cls");
		return;
	}

	// ========== 有人住院则禁止修改 ==========
	if (a->freebeds != a->totalbeds) {
		printf("Cannot modify ward: there are still patients hospitalized (%d occupied beds).\n",
			a->totalbeds - a->freebeds);
		system("pause");
		system("cls");
		return;
	}

	// 保存旧类型，用于后续链表摘除
	int typebefore = a->wardtype;
	int totalbefore = a->totalbeds;

	// 1. 输入新病房类型
	while (1) {
		printf("Please enter the ward type: 1.general ward, 2.emergency ward, 3.ICU ward\n");
		if (scanf("%d", &a->wardtype) == 1 && (a->wardtype >= 1 && a->wardtype <= 3)) {
			break;
		}
		else {
			printf("Input error\n");
			while (getchar() != '\n');
			continue;
		}
	}

	// 2. 根据新类型设置新床位数（因为已确认无人住院，空闲床重置）
	switch (a->wardtype) {
	case 3: a->totalbeds = icubeds; break;
	case 2: a->totalbeds = jzbeds; break;
	case 1: a->totalbeds = ptbeds; break;
	}
	a->freebeds = a->totalbeds;  // 全部空闲

	// 3. 从旧类型链表摘除
	ward* prev = prevfind(a, wardlist, typebefore - 1);
	if (prev == NULL)
		wardlist[typebefore - 1].wardhead = a->next;
	else
		prev->next = a->next;

	// 4. 插入新类型链表尾部
	ward* tail = wardlist[a->wardtype - 1].wardhead;
	if (tail == NULL) {
		wardlist[a->wardtype - 1].wardhead = a;
		a->next = NULL;
	}
	else {
		while (tail->next != NULL) tail = tail->next;
		tail->next = a;
		a->next = NULL;
	}

	// 5. 更新计数
	wardlist[typebefore - 1].wardnum--;   // 旧类型减1
	wardlist[a->wardtype - 1].wardnum++;  // 新类型加1

	// 6. 释放旧床位链表，按新床位数重建
	bed* bed1 = a->bedlist;
	while (bed1) {
		bed* bednext = bed1->next;
		free(bed1);
		bed1 = bednext;
	}
	a->bedlist = NULL;
	for (int i = 0; i < a->totalbeds; i++) {
		char buf[20];
		sprintf(buf, "%d", i);
		bed* b = Createbed(buf);
		if (a->bedlist == NULL) {
			a->bedlist = b;
		}
		else {
			bed* p = a->bedlist;
			while (p->next != NULL) {
				p = p->next;
			}
			p->next = b;
		}
	}

	// 7. 输入科室
	while (1) {
		printf("Please enter the department associated with the ward, just enter the number\n");
		printf("1.internal 2.sugery 3.child 4.gune 5.emergency 6.full\n");
		if (scanf("%d", &a->department)) {
			if (a->department >= 1 && a->department <= 6) break;
			else printf("Input error\n");
		}
		else {
			printf("Input error\n");
			while (getchar() != '\n');
		}
	}

	// 8. 重新生成 roomID（基于新类型和新科室）
	int max_seq = 0;
	ward* cur = wardlist[a->wardtype - 1].wardhead;
	while (cur) {
		if (cur != a && cur->roomID[0] - '0' == a->department) {  // cur != a 排除自己
			int seq = Check2(cur->roomID) % 100;
			if (seq > max_seq) max_seq = seq;
		}
		cur = cur->next;
	}
	int new_id = a->department * 1000 + a->wardtype * 100 + (max_seq + 1);
	sprintf(a->roomID, "%d", new_id);

	// 护士管理子菜单（支持多个护士）
	int nchoice;
	do {
		printf("\n--- Nurse Management for Ward %s ---\n", a->roomID);
		PrintNurseList(a);
		printf("\n1. Add nurse\n");
		printf("2. Remove nurse\n");
		printf("0. Finish\n");
		printf("Choice: ");
		if (scanf("%d", &nchoice) != 1) {
			while (getchar() != '\n');
			nchoice = -1;
		}
		while (getchar() != '\n');
		switch (nchoice) {
		case 1: AddNurseToWard(a); break;
		case 2: RemoveNurseFromWard(a); break;
		case 0: break;
		default: printf("Input error\n");
		}
	} while (nchoice != 0);

	printf("Modify successfully\n");
	system("pause");
	system("cls");
}

void Searchwardprintf(WARD *wardlist) {//查找特定病房并输出信息
	char str[20]; int select;
	while (1) {
		printf("Please choose a way to search the ward\n");//选择特定的查询方式
		printf("1.wardID 2.wardtype 3.department 4.freebedsall 5.nursename 6nurseid 7.freebedsdepartment\n");
		printf("Please just enter the number\n");
		if (scanf("%d", &select)) { //判断输入的是否为1到6的数字
			
			switch (select) {
			case 1:break;
			case 2:break;
			case 3:break;
			case 4:break;
			case 5:break;
			case 6:break;
			case 7:break;
			default:printf("Input error\n"); continue;
			}break;
		}
		else {
			printf("Input error\n");
			while (getchar() != '\n');
			continue;
		}
	}
	getchar();
	switch (select) {     //分各情况分别检索
	case 1:SearchwardbyID(wardlist); break;
	case 2:Searchwardbytype(wardlist); break;
	case 3: Searchwardbykeshi(wardlist); break;
	case 4:Searchwardbyfreebedsall(wardlist); break;
	case 5:Searchwardbynursename(wardlist); break;
	case 6:Searchwardbynurseid(wardlist); break;
	case 7:Searchwardbylxfreebeds(wardlist); break;

	}
	system("pause");
	system("cls");
}
ward* Searchward(WARD *wardlist, char *str) {//通过编号查询对应节点
	int count = 0;
	for (int i = 0; i < 3; i++) {
		ward* p = wardlist[i].wardhead;
		while (p) {
			if (strcmp(str, p->roomID) == 0) {
				count++;
				return p;
			}p = p->next;
		}
	}
	if (!count) {
		return NULL;
	}

}
int  Searchwardint(WARD* wardlist, char* str) {//通过编号查询对应节点查到返回1，未查到返回0
	int count = 0;
	for (int i = 0; i < 3; i++) {
		ward* p = wardlist[i].wardhead;
		while (p) {
			if (strcmp(str, p->roomID) == 0) {
				count++;
				return 1;
			}p = p->next;
		}
	}
	if (!count) {
		return 0;
	}

}
void savefile(WARD *wardlist,const char *filename) {//将数据保存到文件中
	if (!wardlist || !filename) return;
	if (strlen(filename) == 0) {
		printf("Empty filename\n");
		return;
	}
	FILE* file;
	
		file = fopen(filename, "w");
		if (!file) {
			printf("Failed to open the file!\n");
			perror("Reason for failure\n");
		}
		
	
	int count=0;
	for (int i = 0; i < 3; i++) {
		ward* a = wardlist[i].wardhead;
		if (a == NULL) {
			count++;
		}
	}
	if (count == 3) {
		printf("No data to save\n");
		fclose(file);
		system("pause");
		system("cls");
		return;

	} int count1 = 0;
	ward* a;
	for (int i = 0; i < 3; i++) {
		 a = wardlist[i].wardhead;
		while (a) {
			fprintf(file, "%s %d %d %d %d %d", a->roomID, a->wardtype, a->department, a->totalbeds, a->freebeds, a->nursenum);
			if (a->nursenum != 0) {
				NurseIDNode* n = a->nurselist;
				while (n) {
					fprintf(file, " %s", n->nurseid);
					n = n->next;
				}
			}
			fprintf(file, "\n");
			a = a->next; count1++;
		}
	}
	
	fclose(file);
	if (count1 == 0)
		printf("No valid records to save\n");
	else
		printf("Save successfully, %d records saved to %s\n", count1, filename);
	system("pause");
	system("cls");
}
int IDtwo(char *str) {//返回编号后两位
	int num = Check2(str);
	return num % 100;
}
void initial(WARD* wardlist) {
	for (int i = 0; i < 3; i++) {
		wardlist[i].wardhead = NULL;//初始化
		wardlist[i].wardnum = 0;
	}
}
// 1. 初始化队列
void InitQueue(bed_que* q) {
	q->front = q->rear = 0;  // 空队列时，头指针和尾指针重合
}

// 2. 判断队列是否为空
bool QueueEmpty(bed_que* q) {
	return q->front == q->rear;  // 头指针=尾指针 即为空
}

// 3. 判断队列是否为满
bool QueueFull(bed_que* q) {
	// 循环队列：牺牲一个空间，(尾指针+1)%长度 == 头指针 即为满
	return (q->rear + 1) % 31 == q->front;
}

// 4. 入队操作（向队列尾部添加元素）
// 参数：q-队列指针，elem-要入队的元素
bool EnQueue(bed_que* q, bed_point elem) {
	if (QueueFull(q)) {
		printf("Queue full,auto-dequeue the last entry first!\n");
		bed_point tmp;
		DeQueue(q, &tmp);
	}// 满了自动移除最早的一天
	q->is_Use[q->rear] = elem;  // 将元素放入队尾位置
	q->rear = (q->rear + 1) % 31;  // 尾指针后移（循环处理）
	return true;
}

// 5. 出队操作（从队列头部取出元素）
// 参数：q-队列指针，elem-用于存储出队元素的地址
bool DeQueue(bed_que* q, bed_point* elem) {
	if (QueueEmpty(q)) {
		printf("Queue is empty,cannot dequeue!\n");
		return false;
	}

	*elem = q->is_Use[q->front];  // 取出队头元素
	q->front = (q->front + 1) % 31;  // 头指针后移（循环处理）
	return true;
}

// 6. 获取队列当前元素个数
int QueueLength(bed_que* q) {
	// 循环队列长度计算公式：(尾指针 - 头指针 + 长度) % 长度
	return (q->rear - q->front + 31) % 31;
}

// 7. 获取队头元素（不出队）
bool GetHead(bed_que* q, bed_point* elem) {
	if (QueueEmpty(q)) {
		printf("Queue is empty！\n");
		return false;
	}

	*elem = q->is_Use[q->front];
	return true;
}
bed* findfirstfreebeds(ward* p) {//寻找有空床病房中第一个空床
	bed* a = p->bedlist;//对应床头节点
	while (a) {
		if (a->isfree == true) {
			return a;
		}
		a = a->next;
	}

}
// 传入：床位链表头 + 今天的日期
void UpdateAllBedsQueue(WARD *wardlist, Date today)
{
	if (wardlist == NULL) {
		printf("No data found\n");
		return;
}
	for (int i = 0; i < 3; i++) {
		ward* p = wardlist[i].wardhead;
		
		// 遍历每一张床
		while (p != NULL) {
			bed* b = p->bedlist;
			while (b != NULL)
			{
				// 1. 构造当天的床位状态
				bed_point point;
				point.date = today;              // 当天日期
				point.is_use = !b->isfree;       // 空=false，占用=true

				// 2. 加入这张床自己的队列
				EnQueue(&b->dayQueue, point);

				// 下一张床
				b = b->next;
			}p = p->next;
		}
	}
}