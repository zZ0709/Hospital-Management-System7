#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "UI.h"
#include "account.h"
#include "data analyze.h"
#include "accountUI.h"
#include "druginformation.h"
#include "patientinformation.h"

int main()
{
	Init_Account(&Account);
	Identi_Select();
	while (1)
	{
		int Index1 = Int_Input(1,3);//输入确定身份
		switch (Index1)
		{
		case 1:
			//Doctor_UI();
			break;
		case 2:
			//Nurse_UI();
			break;
		case 3:
			//Manager_UI();
			break;
		}
		printf("exit the program or not\n");//是否退出软件
		printf("1.yes\n");
		printf("2.no\n");
		int Index2=Int_Input(1,2);
		if (Index2 == 1)
		{
			exit(0);
		}
		system("pause");
		system("cls");
	}

	return 0;
}