#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<stdbool.h>
#include<math.h>
#include "ward.h"
#include "Druginformation.h"

void Ward_Analyze(WARD wardlist[3]);
void Doctor_Analyze(Dept* dept_list);
void Nurse_Analyze(NurseNode* nurlist);
void Drug_Analyze(pdrug druglist);
void Account_analyze(Fund_System account, NurseNode* nurlist, Doctor* doclist);