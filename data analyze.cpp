#include "data analyze.h"
#include "ward.h"
#include "Doctor.h"
#include "Struct.h"
#include "Dept.h"
#include "date.h"
#include "account.h"


void Ward_Analyze(WARD wardlist[3])//分析病房使用情况来规划病房
{
	double Ave_Use;//记录病房平均使用情况
	int Sum_Use;//记录总使用情况
	int Sum_Time=(wardlist[1].wardhead->bedlist->dayQueue.rear- wardlist[1].wardhead->bedlist->dayQueue.front+30)%30;//记录时间跨度
	bed* bedp;
	for (int i = 0;i < 3;i++)//遍历每个病房，每个病床，得到最新时间
	{
		Sum_Use = 0;
		ward* p = wardlist[i].wardhead->next;
		while (p)
		{
			int Cnt = 1;
			bed* q = p->bedlist->next;
			while (q)
			{
				for (int i = 0;i < 31;i++)
				{
					if (p->bedlist->dayQueue.is_Use[i].is_use)
					{
						Sum_Use++;
					}
				}
				Sum_Use -= p->bedlist->dayQueue.is_Use[p->bedlist->dayQueue.rear].is_use;
				q = q->next;
			}
			Ave_Use = (double)Sum_Use / (Sum_Time * wardlist[i].wardhead->totalbeds);
			if (Ave_Use < 0.7)
			{
				int cnt = 0;
				while (Ave_Use < 0.7 && wardlist[i].wardhead->totalbeds-cnt)
				{
					cnt++;
					Ave_Use = (double)Sum_Use / (Sum_Time * (wardlist[i].wardhead->totalbeds - cnt));
				}
				switch (i)
				{
					case 1:
						printf("general ward:%d need to decrease %d wards\n", Cnt, cnt);
						break;
					case 2:
						printf("emergency ward:%d need to decrease %d wards\n", Cnt, cnt);
						break;
					case 3:
						printf("ICU ward:%d need to decrease %d wards\n", Cnt, cnt);
						break;
				}
			}
			if (Ave_Use > 0.9)
			{
				int cnt = 0;
				while (Ave_Use > 0.9)
				{
					cnt++;
					Ave_Use = (double)Sum_Use / (Sum_Time * (wardlist[i].wardhead->totalbeds + cnt));
				}
				switch (i)
				{
				case 1:
					printf("general ward:%d need to increase %d wards\n", Cnt, cnt);
					break;
				case 2:
					printf("emergency ward:%d need to increase %d wards\n", Cnt, cnt);
					break;
				case 3:
					printf("ICU ward:%d need to increase %d wards\n", Cnt, cnt);
					break;
				}
			}
			p = p->next;
			Cnt++;
		}
	}
	
}
void Doctor_Analyze(Dept* dept_list)//医生分析
{
	double Ave_Work;//记录病房平均使用情况
	int Sum_Work;//记录总使用情况
	int Sum_Time=1;//记录时间跨度
	Dept* depp = dept_list;
	Doctor_Id* p = dept_list->Dept_doctor_head;
	while (p)
	{
		Sum_Time++;
		p = p->next;
	}
	int Cnt = 1;
	while (depp)
	{
		Sum_Work = 0;
		Doctor_Id* docp=depp->Dept_doctor_head;

		while (docp)
		{
			ScheduleNode_Doctor* q = get_Doctor_id(docp->id)->schedule_head;
			while (q)
			{
				Sum_Work += q->treat_count;
				q = q->next;
			}
			docp = docp->next;
		}
		Ave_Work = (double)Sum_Work / (depp->doct_count * Sum_Time);
		if (Ave_Work < 5.0)
		{
			int cnt = 0;
			while (Ave_Work < 5.0 && depp->doct_count - cnt)
			{
				cnt++;
				Ave_Work = (double)Sum_Work / (depp->doct_count * Sum_Time - cnt);
			}
			switch (Cnt)
			{
				//1.内科 internal  2. 外科 surgery  3.儿科 child  4. 妇科 gynecology  5. 急诊 emergency  6. 全科 general
			case 1:
				printf("internal\n");
				break;
			case 2:
				printf("surgery\n");
				break;
			case 3:
				printf("child\n");
				break;
			case 4:
				printf("gynecology\n");
				break;
			case 5:
				printf("emergency\n");
				break;
			case 6:
				printf("general\n");
				break;
			}
			printf("need to decrease %d doctors", cnt);
		}
		if (Ave_Work > 8.0)
		{
			int cnt = 0;
			while (Ave_Work > 8.0)
			{
				cnt++;
				Ave_Work = (double)Sum_Work / (depp->doct_count * Sum_Time + cnt);
			}
			switch (Cnt)
			{
				//1.内科 internal  2. 外科 surgery  3.儿科 child  4. 妇科 gynecology  5. 急诊 emergency  6. 全科 general
			case 1:
				printf("internal\n");
				break;
			case 2:
				printf("surgery\n");
				break;
			case 3:
				printf("child\n");
				break;
			case 4:
				printf("gynecology\n");
				break;
			case 5:
				printf("emergency\n");
				break;
			case 6:
				printf("general\n");
				break;
			}
			printf("need to increase %d doctors", cnt);
		}
		depp = depp->next;
		Cnt++;
	}

}

void Nurse_Analyze(NurseNode* nurlist)
{
	double Ave_Work;//记录病房平均使用情况
	int Sum_Work=0;//记录总使用情况
	int Sum_Time = 1;//记录时间跨度
	NurseNode* nurp = nurlist;
	int Cnt = 0;
	while (nurp)
	{
		ScheduleNode_Nurse* p = nurp->schedule_head;
		while (p)
		{
			Sum_Work += p->Is_work;
			p = p->next;
		}
		nurp = nurp->next;
		Cnt++;
	}
	Ave_Work = (double)Sum_Work / (Sum_Time * Cnt);
	if (Ave_Work<0.7)
	{
		int cnt = 0;
		while (Ave_Work < 0.7 && Sum_Time * Cnt - cnt)
		{
			Ave_Work = (double)Sum_Work / (Sum_Time * Cnt - cnt);
			cnt++;
		}
		printf("advide to decrease %d nurses\n",cnt);
	}
	if (Ave_Work > 0.9)
	{
		int cnt = 0;
		while (Ave_Work > 0.9 )
		{
			Ave_Work = (double)Sum_Work / (Sum_Time * Cnt + cnt);
			cnt++;
		}
		printf("advide to increase %d nurses\n", cnt);
	}
}
void Drug_Analyze(pdrug druglist)
{
	tdrug* drugp = druglist->next_drug;
	while (drugp)
	{
		int Sum_change=0;//记录总使用情况
		int Sum_Time;
		if (drugp->valid_days >= 30)
		{
			Sum_Time = 30;
		}
		else
		{
			Sum_Time = drugp->valid_days;
		}
		for (int i = 0;i < 30;i++)
		{
			Sum_change += (drugp->history[i].inbound - drugp->history[i].outbound);
		}
		if (Sum_change > 0 && drugp->drug_inventory / Sum_change < 20)
		{
			int advice;
			advice = Sum_change * 20 - drugp->drug_inventory;
			printf("need purchase %s:%d\n", drugp->drug_name, advice);
		}
		drugp = drugp->next_drug;
	}
}
void Account_analyze(Fund_System *account, NurseNode*nurlist,Doctor*doclist)
{
	int sum_nur=0;
	int sum_doc = 0;
	NurseNode* nurp = nurlist;
	DoctorNode* docp = doclist;
	while (nurp)
	{
		sum_nur++;
		nurp = nurp->next;
	}
	while (docp)
	{
		sum_doc++;
		docp = docp->next;
	}
	if (account->sumAmount < 100000.00)
	{
		double salary_nur = nurlist->salary;
		double salary_doc = doclist->salary;
		if (salary_nur > 3500.00 && salary_doc>4500.00)
		{
			double release1 = (account->sumAmount - 100000.00) / (sum_nur + sum_doc);
			if (salary_nur - 3500.00 > release1 && salary_doc - 4500.00 > release1)
			{
				printf("advice to decrease every doctor and nurse salary:%.2d\n", release1);
			}
			else
			{
				if ((salary_nur - 3500.00) * sum_nur + (salary_doc - 4500.00) * sum_doc < account->sumAmount - 100000.00)
				{
					printf("advice decrease every doctor salary:%.2d\n", salary_doc - 4500.00);
					printf("advice decrease every nurse salary:%.2d\n", salary_nur - 3500.00);
				}
				else
				{
					double release2=((salary_nur - 3500.00)* sum_nur + (salary_doc - 4500.00) * sum_doc - account->sumAmount + 100000.00) / (sum_nur + sum_doc);
					printf("advice decrease every doctor salary:%.2d\n", salary_doc - 4500.00 -release2);
					printf("advice decrease every nurse salary:%.2d\n", salary_nur - 3500.00 - release2);
				}
			}
		}
	}
	if (account->sumAmount > 400000.00)
	{
		double increase1=account->sumAmount-500000.00;
		if (increase1 > 0)
		{
			double increase2;
			if (increase1 / (sum_doc + sum_nur) > 2000)
			{
				increase2 = 2000.00;
			}
			else
			{
				increase2 = increase1 / (sum_doc + sum_nur);
			}
			printf("advice increase every doctor and nurse salary%.2d",increase2);
		}
	}
}