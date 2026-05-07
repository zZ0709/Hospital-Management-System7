#include "account.h"
#include "ward.h"
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include "Date.h"
#include "Struct.h"
#include "Doctor.h"
#include "Dept.h"
#include "Global.h"
#include "cure.h"
#include "DrugInformation.h"

void Init_Date(Date* date, WARD wardlist[3], tdrug* druglist,DoctorNode *doclist, NurseNode* nurlist)
{
	date = NULL;
	for (int i = 0;i < 3;i++)//遍历每个病房，每个病床，得到最新时间
	{
		ward* p = wardlist[i].wardhead->next;
		while (p)
		{
			bed* q = p->bedlist->next;
			while (q)
			{
				if (!QueueEmpty(&(q->dayQueue)) && (date == NULL || Date_Compare(*date,q->dayQueue.is_Use[(q->dayQueue.rear + 30) % 31].date)==2))
				{
					*date = q->dayQueue.is_Use[(q->dayQueue.rear + 30) % 31].date;
				}
				q = q->next;
			}
			p = p->next;
		}
	}
	tdrug* drugp = druglist->next_drug;
	while (drugp)
	{
		if (drugp->valid_days && Date_Compare(*date, drugp->history[drugp->current_day_idx].date) == 2)
		{
			*date = drugp->history[drugp->current_day_idx].date;
		}
		drugp = drugp->next_drug;
	}
	DoctorNode* docp = doclist;
	while (docp)
	{
		if (docp->schedule_head && Date_Compare(*date, docp->schedule_tail->date)==2)
		{
			*date = docp->schedule_tail->date;
		}
		docp = docp->next;
	}
	NurseNode* nurp = nurlist;
	while (nurp)
	{
		if (nurp->schedule_head && Date_Compare(*date, nurp->schedule_tail->date) == 2)
		{
			*date = nurp->schedule_tail->date;
		}
		nurp = nurp->next;
	}
}
void Update_Date(Date* date, WARD wardlist[3], tdrug* druglist, DoctorNode* doclist, NurseNode* nurlist)
{
	for (int i = 0;i < 3;i++)//遍历每个病房，每个病床，得到最新时间
	{
		ward* p = wardlist[i].wardhead->next;
		while (p)
		{
			bed* q = p->bedlist->next;
			while (q)
			{
				if (Date_Compare(*date, q->dayQueue.is_Use[(q->dayQueue.rear + 30) % 31].date) == 0)
				{
					bed_point bed;
					bed.is_use = 0;
					bed.date = *date;
					EnQueue(&q->dayQueue,bed);
				}
				q = q->next;
			}
			p = p->next;
		}
	}
	tdrug* drugp = druglist->next_drug;
	Auto_initial_today_drug(*date);
	DoctorNode* docp = doclist;
	while (docp)
	{
		if (Date_Compare(*date, docp->schedule_tail->date) == 0)
		{
			add_schedule_Doc(docp, *date,1);
		}
		docp = docp->next;
	}
	NurseNode* nurp = nurlist;
	while (nurp)
	{
		if (nurp->schedule_head && Date_Compare(*date, nurp->schedule_tail->date) == 0)
		{
			add_schedule_Nurse(nurp,*date,1);
		}
		nurp = nurp->next;
	}
}

void Date_Check(Date* Sum_Date,Date date)
{
	if (Date_Compare(date, *Sum_Date) == 0)
	{
		*Sum_Date = date;
		Update_Date(&New_Date,wardlist[3],drug_head, doctor_head, nurse_head);
	}
}