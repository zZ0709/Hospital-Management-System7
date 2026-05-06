#include"Dept.h"

Dept* dept_list = NULL;    // 科室链表头指针

//初始化6个科室的链表
Dept* init_dept_list() {
	Dept* head = NULL;
	Dept* tail = NULL;
	for (int i = 0; i < MAX_DEPT; i++) {
		Dept* node = create_dept_node();
		if (node == NULL) {
			// 内存分配失败，清理已分配的节点
			while (head != NULL) {
				Dept* temp = head;
				head = head->next;
				free(temp);
			}
			return NULL;
		}
		if (head == NULL) {
			head = node;
			tail = node;
		}
		else {
			tail->next = node;
			tail = node;
		}
	}
	return head;
}

//  创建科室节点
Dept* create_dept_node() {
	Dept* node = (Dept*)malloc(sizeof(Dept));
	if (node == NULL) {
		printf("Memory allocation failed!\n");
		return NULL;
	}
	node->Dept_doctor_head = NULL;
	node->doct_count = 0;
	node->next = NULL;
	return node;
}

//把该医生存入对应科室的医生表中
void add_doctor_to_dept(Dept* dept_list, DoctorNode* doctor) {
	if (dept_list == NULL ) {
		printf("Invalid department list node.\n");
		return;
	}
	
	int dept_index = doctor->dept - 1; // 科室编号从1开始，数组索引从0开始
	Dept* dept = dept_list;
	for (int i = 0; i < dept_index; i++) {
		if (dept == NULL) {
			printf("Department index out of range.\n");
			return;
		}
		dept = dept->next;
	}

	if (dept == NULL) {
		printf("Department not found.\n");
		return;
	}

	Doctor_Id* doctor_id_node = Create_Doctor_id_Node();

	// 将医生节点添加到科室的医生链表末尾
	if (dept->Dept_doctor_head == NULL) {
		dept->Dept_doctor_head = doctor_id_node;
	}
	else {
		Doctor_Id* p = dept->Dept_doctor_head;
		while (p->next != NULL) {
			p = p->next;
		}
		p->next = doctor_id_node;
	}
	dept->doct_count++;
}

//删除一个科室的医生表中的一个医生节点
void delete_doctor_from_dept(Dept* dept_list, DoctorNode* doctor) {
	if (dept_list == NULL || doctor == NULL) {
		printf("Invalid department list or doctor node.\n");
		return;
	}

	int dept_index = doctor->dept - 1; // 科室编号从1开始，数组索引从0开始
	Dept* dept = dept_list;
	for (int i = 0; i < dept_index; i++) {
		if (dept == NULL) {
			printf("Department index out of range.\n");
			return;
		}
		dept = dept->next;
	}
	if (dept == NULL) {
		printf("Department not found.\n");
		return;
	}

	Doctor_Id* p = dept->Dept_doctor_head;
	Doctor_Id* prev = NULL;
	while (p != NULL) {
		if (strcmp(p->id, doctor->id) == 0) {
			if (prev == NULL) {
				dept->Dept_doctor_head = p->next;
			}
			else {
				prev->next = p->next;
			}
			dept->doct_count--;
			return;
		}
		prev = p;
		p = p->next;
	}
	printf("Doctor not found in department.\n");
}

//释放科室链表
void free_dept_list(Dept* head) {
	Dept* p = head;
	while (p != NULL) {
		Dept* temp = p;
		p = p->next;
		Doctor_Id* temp_id = temp->Dept_doctor_head;
		while (temp_id != NULL) {
			Doctor_Id* temp_id_next = temp_id->next;
			free(temp_id);
			temp_id = temp_id_next;
		}
		free(temp);
	}
}