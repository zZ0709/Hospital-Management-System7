#include "account.h"

//============================前置操作（简化功能代码）==============================
bool Check_Double(const char* str) {//检查金额输入（二位小数）
	int len = strlen(str);
	int dotIndex = -1;
	int dotCount = 0;

	// 1. 检查是否为空或过短 (最小长度如 "0.01" 为 4)
	if (len < 4) return false;

	for (int i = 0; i < len; i++) {
		// 允许负号出现在开头
		if (i == 0 && str[i] == '-') continue;

		if (str[i] == '.') {
			dotCount++;
			dotIndex = i;
		}
		else if (!isdigit(str[i])) {
			// 如果不是数字也不是小数点，则非法
			return false;
		}
	}

	// 2. 必须有且仅有一个小数点
	if (dotCount != 1) return false;

	// 3. 核心校验：小数点后面必须正好有 2 位数字
	// 字符串长度 - 小数点下标 - 1 = 小数位数
	int decimalPlaces = len - dotIndex - 1;
	if (decimalPlaces == 2) {
		return true;
	}

	return false;
}
int Date_Compare(Date Start, Date End)//判断日期是否合法
{
	//先将日期转化为字符串
	char start_Str[8], char end_Str[8];
	int i = 7;
	while (i!=5)//录入日
	{
		start_Str[i] = Start.day % 10;
		end_Str[i] = End.day % 10;
		Start.day /= 10;
		End.day /= 10;
		i--;
	}
	while (i!=3)//录入月
	{
		start_Str[i] = Start.month % 10;
		end_Str[i] = End.month % 10;
		Start.month /= 10;
		End.month /= 10;
		i--;
	}
	while (i!=-1)//录入年份
	{
		start_Str[i] = Start.year % 10;
		end_Str[i] = End.year % 10;
		Start.year /= 10;
		End.year /= 10;
		i--;
	}
	//判断两个日期输入在时间上是否合法
	if (strcmp(start_Str, end_Str) == 1)
	{
		return 0;
	}
	else
	{
		if (strcmp(start_Str, end_Str) == 0)
		{
			return 1;
		}
	}
	return 2;
}
void Record_Print(Fund_Record *record)//某一条的资金记录打印
{
	//输入信息
	printf("Ordeere number:%d\n", record->order_Number);
	printf("Date:year %d month %d day %d\n", record->date.year, record->date.month, record->date.day);
	printf("Cash:%.2f\n", record->cash);
	printf("Type:");
	switch (record->type)
	{
	case 1:
		printf("in hospital\n");//住院
		break;
	case 2:
		printf("register\n");//挂号
		break;
	case 3:
		printf("diagnosis and treatment\n");//疗诊
		break;
	case 4:
		printf("salary\n");//工资发放
		break;
	case 5:
		printf("5.medicine purchase\n");//药品采购
		break;
	case 6:
		printf("6.medicine sale\n");//药品出售
		break;
	}
	printf("valid:%d\n", record->Is_Valid);
}
Date Date_Input()//日期输入
{
	Date ret;
	printf("please input the date\n");//年份输入
	printf("year(2000-2026):");
	ret.year = Int_Input(2000, 2026);
	printf("\n");
	printf("month:");//月份输入
	ret.month = Int_Input(1,12);
	printf("\n");
	printf("day:");//日输入
	ret.day = Int_Input(1, 31);
	printf("\n");
	return ret;
}
void Type_Choose()//金额类型选择
{
	printf("\n");
	printf("1.in hospital\n");//住院
	printf("2.register\n");//挂号
	printf("3.diagnosis and treatment\n");//疗诊
	printf("4.salary\n");//工资发放
	printf("5.medicine purchase\n");//药品采购
	printf("6.medicine sale\n");//药品出售
}
int Int_Input(int min,int max)//整型输入
{
	int ret;
	char Str[Str_Len];
	scanf("%s", Str);
	while (ret=!Check_Int(Str,min,max)) {
		printf("input error,please check out the range and try again\n");
		scanf("%s", Str);
	}
	return ret;
}
double Double_Input()//浮点数输入
{
	double ret=0;
	char Str[Str_Len];
	scanf("%s", Str);
	while (!Check_Double(Str)) {//以字符串形式输入浮点数
		printf("format error,please try again\n");
		scanf("%s", Str);
	}
	int i;
	for (i = (Str[i]=='-'?1:0);Str[i] != '.';i++)//将字符串转化为浮点类型
	{
		ret = ret * 10 + Str[i]-'0';//整数部分
	}
	double last=0;
	for (i++;Str[i];i++)//小数部分
	{
		last += last * 10 + Str[i] - '0';
	}
	ret += last / 100;
	if (Str[i] == '-');
	ret = -ret;
	return ret;
}
//============================检验输入的整型变量是否合法========================================
int Check_Int(char* str, int min, int max) {
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
//===================================功能函数==================================================
void Init_Account(Fund_System *Account)//初始化资金账户
{
	(*Account).sumAmount = Init_Amount;
	(*Account).Fund_List = (Fund_Record*)malloc(sizeof(Fund_Record));
	(*Account).Fund_List->next = NULL;
	Account->Record_Num = 0;
}
void Fund_Add(Fund_System *Account)//添加资金记录
{
	printf("please input the number of fundrecords(1-50)\n");//填入需要添加的资金记录的数量
	int FundRecord_Num=Int_Input(1, 50);
	while (FundRecord_Num) {//头插法建立链表，记录资金变动
		Fund_Record* p = (Fund_Record*)malloc(sizeof(Fund_Record));
		if (p == NULL) {
			printf("Failed to add,the space is insufficient\n");
			return;//防御性编程，防止开辟空间失败
		}
		p->next = Account->Fund_List->next;
		Account->Fund_List->next = p;
		//输入日期
		p->date = Date_Input();
		//输入金额和变动来源
		printf("please input the cash\n");
		p->cash = Double_Input();
		Account->sumAmount += p->cash;
		printf("please input the type\n");
		Type_Choose();
		p->type = Int_Input(1,6);
		p->Is_Valid = 1;//默认有效
		FundRecord_Num--;
		Account->Record_Num++;
		//自动记录序号
		p->order_Number = Account->Record_Num;
	}
	printf("add successfully\n");
	Account_Save_File(Account);//保存到文件中
	system("pause");//清屏
	system("cls");
}
void Auto_Fund(Fund_System *Account, int type, int cash, Date date)
{
	Fund_Record* p = (Fund_Record*)malloc(sizeof(Fund_Record));
	if (p == NULL) {
		printf("Failed to add,the space is insufficient\n");
		return;//防御性编程，防止开辟空间失败
	}
	p->next = Account->Fund_List->next;
	Account->Fund_List->next = p;
	//日期
	p->date = date;
	//金额和变动来源
	p->cash = cash;
	Account->sumAmount += p->cash;
	p->type = type;
	p->Is_Valid = 1;//默认有效
	Account->Record_Num++;
	//自动记录序号
	p->order_Number = Account->Record_Num;
	Account_Save_File(Account);//保存到文件中
}
void Fund_Change(Fund_System *Account)//记录修改
{
	while (1) {
		printf("please input way of search\n\n");//选择查找资金记录的方法
		printf("*************************************\n");
		printf("** 1.acccroding the ordernumber    **\n");
		printf("** 2.accroding the date            **\n");
		printf("*************************************\n\n");
		int Index1 = Int_Input(1, 2);
		switch (Index1)
		{
		case 1://跟据序号查找
			printf("please input the ordernumber\n");
			int order_Number = Int_Input(1, Account->Record_Num);
			Fund_Record* p = Account->Fund_List->next;
			while (p) {//打印找到的记录的信息
				if (p->order_Number == order_Number && p->Is_Valid)
				{
					Record_Print(p);
					break;
				}
				p = p->next;
			}
			if (!p) {
				printf("Don't find\n");
				break;
			}
			printf("please choose the information need to change\n");
			printf("1.date\n");
			printf("2.cash\n");
			printf("3.type\n");
			//输入修改后的信息
			printf("please input the new date\n");
			p->date = Date_Input();
			printf("please input the new cash\n");
			p->cash = Double_Input();
			printf("please input the new type\n");
			Type_Choose();
			p->type = Int_Input(1, 6);
			printf("change successfully");
			break;
		case 2://跟据日期查找
			//输入日期
			printf("please input the date\n");
			Date date = Date_Input();
			bool flag1 = 0, flag2 = 0;
			Fund_Record* p = Account->Fund_List->next;
			while (p)//查找打印所有符合日期的信息
			{
				if (p->date.year == date.year && p->date.month == date.month && p->date.day == date.day && p->Is_Valid) {
					Record_Print(p);
					flag1 = 1;
					printf("1.change or 2.not");//选择是否修改
					if (Int_Input(1, 2) == 1)//选择修改
					{
						flag2 = 1;
						//输入修改后的信息
						printf("please input the new date\n");
						p->date = Date_Input();
						printf("please input the new cash\n");
						p->cash = Double_Input();
						printf("please input the new type\n");
						Type_Choose();
						p->type = Int_Input(1, 6);
					}
				}
				p = p->next;
			}
			if (!flag1)//查找失败
			{
				printf("Don't find\n");
				break;
			}
			if (flag2)//修改成功
			{
				printf("change successfully\n");
			}
			break;
		}
		Account_Save_File(Account);//保存到文件中
		system("pause");//清屏
		system("cls");
		printf("1.continue or 2.leave\n");//选择是否继续修改
		if (Int_Input(1, 2) == 2)
		{
			//清屏
			system("cls");
			break;
		}
	}
}
void Fund_Delete(Fund_System *Account)//记录删除
{
	while (1) {
		printf("please input way of search\n\n");//选择查找资金记录的方法
		printf("*************************************\n");
		printf("** 1.acccroding the ordernumber    **\n");
		printf("** 2.accroding the date            **\n");
		printf("*************************************\n\n");
		int Index1 = Int_Input(1, 2);
		switch (Index1)
		{
		case 1://跟据序号查找
			printf("please input the ordernumber\n");
			int order_Number = Int_Input(1, Account->Record_Num);
			Fund_Record* p = Account->Fund_List->next, * q = Account->Fund_List;
			bool flag = 0;
			while (p) {//打印找到的记录的信息
				if (p->order_Number == order_Number && p->Is_Valid)//对每一个符合的记录判断是否删除
				{
					Record_Print(p);
					printf("1.delete or 2.not");//确认是否删除
					if(Int_Input(1,2)==1)//选择删除
					{
						flag = 1;
						p->Is_Valid = 0;
					}
					break;
				}
				q = p;
				p = p->next;
			}
			if (!p) {
				printf("Don't find or the record isn't valid\n");
				break;
			}
			//修改操作
			if (!flag)
			{
				printf("delete successfully");
			}
			break;
		case 2://跟据日期查找
			//输入日期
			printf("please input the date\n");
			Date date = Date_Input();
			bool flag1 = 0, flag2 = 0;
			Fund_Record* p = Account->Fund_List->next, * q = Account->Fund_List;
			while (p)//查找打印所有符合日期的信息
			{
				if (p->date.year == date.year && p->date.month == date.month && p->date.day == date.day && p->Is_Valid) {
					Record_Print(p);
					flag1 = 1;
					printf("1.change or 2.not");//选择是否删除
					if (Int_Input(1, 2) == 1)//选择删除
					{
						flag2 = 1;
						p->Is_Valid = 0;
						continue;
					}
				}
				q = p;
				p = p->next;
			}
			if (!flag1)//查找失败
			{
				printf("Don't find or the record isn't valid\n");
				break;
			}
			if (flag2)//删除成功
			{
				printf("delete successfully\n");
			}
			break;
		}
		Account_Save_File(Account);//保存到文件中
		system("pause");//清屏
		system("cls");
		printf("1.continue or 2.leave\n");//选择是否继续删除
		if (Int_Input(1, 2) == 2)
		{
			system("cls");
			break;
		}
	}
}
void Foud_Search(Fund_System *Account)//资金记录查找
{
	while (1) {
		printf("please input way of search\n\n");//选择查找资金记录的方法
		printf("*************************************\n");
		printf("** 1.acccroding the ordernumber    **\n");
		printf("** 2.accroding the date            **\n");
		printf("*************************************\n\n");
		int Index1 = Int_Input(1, 2);
		switch (Index1)
		{
		case 1://跟据序号查找
			printf("please input the ordernumber\n");
			int order_Number = Int_Input(1, Account->Record_Num);
			Fund_Record* p = Account->Fund_List->next;
			while (p) {//打印找到的记录的信息
				if (p->order_Number == order_Number)
				{
					Record_Print(p);
					break;
				}
				p = p->next;
			}
			if (!p)
			{
				printf("Don't find\n");
			}
			break;
		case 2://跟据日期查找
			//输入日期
			printf("please input the date\n");
			Date date = Date_Input();
			Fund_Record* p = Account->Fund_List->next;
			bool flag = 0;
			while (p)//查找打印所有符合日期的信息
			{
				if (p->date.year == date.year && p->date.month == date.month && p->date.day == date.day) {
					Record_Print(p);//打印
					flag = 1;
				}
				p = p->next;
			}
			if (!flag)
			{
				printf("Don't find\n");
			}
			break;
		}
		system("pause");//清屏
		system("cls");
		printf("1.continue or 2.leave\n");//选择是否继续查找
		if (Int_Input(1, 2) == 2)
		{
			system("cls");
			break;
		}
	}
}
void Print_Allrecord(Fund_System *Account)//打印某段时间内的所有记录
{
	//选择起止时间
	printf("please input the range you want to print\n");
	printf("the start:");
	Date date_Start = Date_Input();
	printf("\n");
	printf("the end:");
	Date date_End = Date_Input();
	Fund_Record* p = Account->Fund_List->next;
	//打印操作
	while (p)
	{
		if (Date_Compare(date_Start, p->date) && Date_Compare(p->date, date_End))
		{//判断是否在规定日期内
			Record_Print(p);//打印
		}
		p = p->next;
	}
	printf("print successfully\n");
}
int Account_Save_File(Fund_System *Account)//文件保存
{
	FILE* fStup;
	fStup = fopen(Account_Filename, "w");
	Fund_Record* p = Account->Fund_List->next;//将链表数据写入文件
	while (p) {
		fprintf(fStup, "%d. date:year %d month %d day %d\tcase:%.2f\ttype:%d\tvalid:%d\n",
			p->order_Number,p->date.year,p->date.month,p->date.day,p->cash,p->type,p->Is_Valid);
		p = p->next;
	}
	fclose(fStup);//关闭文件
	printf("saved successfully\n");
	return 1;
}
int Account_Save_Read(Fund_System *Account)//文件读取，将文件内容存到链表里
{
	FILE* fStup;
	fStup = fopen(Account_Filename, "r");
	if (fStup == NULL) {
		printf("read error,the code whil be force to stop\n");
		return 0;//防御性编程，防止打开文件失败导致程序运行出错
	}
	//从文件中读取信息
	Fund_Record record;
	while (fscanf(fStup, "%d. date:year %d month %d day %d\tcase:%.2f\ttype:%d\tvalid:%d\n",
		record.order_Number, record.date.year, record.date.month, record.date.day, record.cash, record.type,record.Is_Valid) != EOF) {
		Fund_Record* p = (Fund_Record*)malloc(sizeof(Fund_Record));
		if (p == NULL) {
			printf("you don't have enough space\n");
			return 0;//防御性编程，防止开辟空间失败
		}
		p->next = Account->Fund_List->next;//将文件中读取到的信息转移到链表中
		Account->Fund_List->next = p;
		p->order_Number = record.order_Number;
		p->date = record.date;
		p->cash = record.cash;
		p->type = record.type;
	}
	fclose(fStup);//读取完毕
	printf("read successfully\n");
	return 1;
}