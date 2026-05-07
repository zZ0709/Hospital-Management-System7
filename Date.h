#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "account.h"
#include "ward.h"
#include "Struct.h"
#include "cure.h"
#include "Druginformation.h"
extern Date New_Date;

void Init_Date(Date* date, WARD wardlist[3], tdrug* druglist, DoctorNode* doclist,NurseNode *nurlist);
void Update_Date(Date* date, WARD wardlist[3], tdrug* druglist, DoctorNode* doclist, NurseNode* nurlist);
void Date_Check(Date* Sum_Date, Date date);