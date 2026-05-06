#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Tool.h"
#include"Doctor.h"
#include"Nurse.h"
#include"Project.h"
#include"Global.h"
#include"Dept.h"

int the_main_of_Doc_and_Nur() {

    dept_list = init_dept_list();
    // 启动时自动加载文件
    load_doctors();
    load_nurses();
    load_doc_schedules();
  	load_nurse_schedules();

	// 初始化科室链表
    int main_choice;

    while (1) {
        printf("\n");
        printf(" -------------------------------------------\n");
        printf("|       Hospital Staff Mgmt System          |\n");
        printf("|-------------------------------------------|\n");
        printf("|  Doctors: %-3d          Nurses: %-3d      |\n", doctor_count, nurse_count);
        printf("|-------------------------------------------|\n");
        printf("|  1. Doctor Management                     |\n");
        printf("|  2. Nurse Management                      |\n");
        printf("|  3. Save Data to File (Stay)              |\n");
        printf("|  4. Save and Exit                         |\n");
        printf("|  0. Exit without Saving                   |\n");
        printf(" -------------------------------------------\n");
        printf("Please select: ");

        // 输入保护
        if ((main_choice = Single_number_input()) == -1) {
            continue;
        }

        else if (main_choice == 0) {
            free_doctor_list();
            free_nurse_list();
            printf("System exited without saving changes. Goodbye!\n");
            break;
        }
        else if (main_choice == 1) {
            int doc_choice;
            while (1) {
                printf("\n----- Doctor Management -----\n");
                printf("1. Add Doctor\n");
                printf("2. Delete Doctor\n");
                printf("3. Modify Doctor\n");
                printf("4. Query Doctor\n");
                printf("0. Return to Previous\n");
                printf("Choice: ");

                if ((doc_choice = Single_number_input()) == -1) {
                    continue;
                }
               

                if (doc_choice == 0) break;
                switch (doc_choice) {
                case 1: add_doctor(); break;
                case 2: delete_doctor(); break;
                case 3: modify_doctor(); break;
                case 4: query_doctor(); break;
                default: printf("Invalid selection\n");
                }
            }
        }
        else if (main_choice == 2) {
            int nur_choice;
            while (1) {
                printf("\n----- Nurse Management -----\n");
                printf("1. Add Nurse\n");
                printf("2. Delete Nurse\n");
                printf("3. Modify Nurse\n");
                printf("4. Query Nurse\n");
                printf("0. Return to Previous\n");
                
                if ((nur_choice = Single_number_input()) == -1) {
                    continue;
                }

                if (nur_choice == 0) break;
                switch (nur_choice) {
                case 1: add_nurse(); break;
                case 2: delete_nurse(); break;
                case 3: modify_nurse(); break;
                case 4: query_nurse(); break;
                }
            }
        }
        else if (main_choice == 3) {
            save_doctors();
            save_nurses();
            save_doc_schedules();
			save_nurse_schedules();
            printf("--- Notice: All data synchronized to local files ---\n");
        }
        else if (main_choice == 4) {
            save_doctors();
            save_nurses();
            save_doc_schedules();
			save_nurse_schedules();
            free_doctor_list();
            free_dept_list(dept_list);
            free_nurse_list();
            printf("Data saved, memory cleared, safely exited. Goodbye!\n");
            break;
        }
        else {
            printf("Invalid selection, please try again.\n");
        }
    }
    return 0;
}