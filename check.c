#include "check.h"
// 辅助函数 1：清空输入流中的残留字符
void clear_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 辅助函数 2：安全清理 fgets 读取的字符串回车，并处理超长截断
void clean_fgets_string(char* str, int max_capacity) {
    int len = strlen(str);
    // 如果末尾有换行符，把它替换成 \0
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
    else if (len == max_capacity - 1) {
        // 如果没有换行符，且读满了缓冲区，说明用户输入超长了，清理残留
        clear_buffer();
    }
}

void get_alpha_string(const char* prompt, char* str, int max_len) {
    char temp[1024]; // 内部安全缓冲区

    while (1) {
        printf("%s (max %d chars): ", prompt, max_len);

        if (fgets(temp, sizeof(temp), stdin) == NULL) {
            clearerr(stdin);
            continue;
        }

        clean_fgets_string(temp, sizeof(temp));
        int len = strlen(temp);

        // 检查长度
        if (len == 0 || len > max_len) {
            printf("Error: String length must be between 1 and %d!\n", max_len);
            continue;
        }

        // 检查是否全字母
        bool is_valid = true;
        for (int i = 0; i < len; i++) {
            if (!isalpha(temp[i]) && temp[i] != ' ') {//允许空格
                is_valid = false;
                break;
            }
        }

        if (is_valid) {
            strcpy(str, temp); // 校验完美通过，安全拷贝给外部
            break;
        }
        else {
            printf("Error: Must contain only letters!\n");
        }
    }
}

int get_int_range(const char* prompt, int min, int max) {
    int current_val;

    while (1) {
        printf("%s [%d - %d]: ", prompt, min, max);

        // 尝试读取一个整数
        if (scanf("%d", &current_val) != 1) {
            printf("Error: Not a valid number!\n");
            clear_buffer(); // 读到字母或符号，清空错误输入
            continue;
        }

        clear_buffer(); // 成功读取数字后，顺手清掉后面的回车符，防止干扰下一次输入

        if (current_val >= min && current_val <= max) {
            return current_val; // 合法，直接返回
        }
        else {
            printf("Error: Value %d is out of range!\n", current_val);
        }
    }
}

void get_numeric_string_fixed(const char* prompt, char* str, int fixed_len) {
    char temp[1024];

    while (1) {
        printf("%s (exactly %d digits): ", prompt, fixed_len);

        if (fgets(temp, sizeof(temp), stdin) == NULL) {
            clearerr(stdin);
            continue;
        }

        clean_fgets_string(temp, sizeof(temp));
        int len = strlen(temp);

        // 检查长度
        if (len != fixed_len) {
            printf("Error: Must be exactly %d digits!\n", fixed_len);
            continue;
        }

        // 检查是否全数字
        bool digits_only = true;
        for (int i = 0; i < len; i++) {
            if (!isdigit(temp[i])) {
                digits_only = false;
                break;
            }
        }

        if (digits_only) {
            strcpy(str, temp);
            break;
        }
        else {
            printf("Error: Must be numeric only!\n");
        }
    }
}

int get_int_multiple_100(const char* prompt) {
    int current_val;

    while (1) {
        printf("%s: ", prompt);

        if (scanf("%d", &current_val) != 1) {
            printf("Error: Not a valid number!\n");
            clear_buffer();
            continue;
        }

        clear_buffer(); // 清除残留回车

        if (current_val != 0 && current_val % 100 == 0) {
            return current_val;
        }
        else {
            printf("Error: %d is not a valid multiple of 100!\n", current_val);
        }
    }
}

void get_char_medicalrecord(char record[10], const char initialrecord[10]) {
    int days,i=0;//保存住院天数
    int intrecord[9];//先以数组形式保存
    intrecord[0]=get_int_range("Please enter the registration form: 1. Outpatient 2. Emergency\n",1,2);
    intrecord[1] = get_int_range("Please enter whether you are hospitalized: 1. No 2. Yes\n", 1, 2);
    if (intrecord[1] == 1) {
        intrecord[2] = intrecord[3] = intrecord[4] = 0;
    }
    else {
        	if (intrecord[1] == '2' && (initialrecord[1] == '0' || initialrecord[1] == '1'))//医生首次录入，或者原本不住院现住院时住院人数加一
		{
            if(Searchwardbylxfreebeds(wardlist)){//#####################?
        days = get_int_range("Please enter the length of hospital stay:\n", 1, 365);
        intrecord[4] = days % 10;
        days /= 10;
        intrecord[3] = days % 10;
        days /= 10;
        record[2] = days % 10;
    }else{
        printf("Sorry, no available beds in the hospital. Please choose outpatient treatment.\n");
        intrecord[1] = 1;
        intrecord[2] = intrecord[3] = intrecord[4] = 0;
    }
    }
}
    intrecord[5] = get_int_range("Please enter the number of CT scans:\n", 0, 9);
    intrecord[6] = get_int_range("Please enter the number of blood draws :\n", 0, 9);
    intrecord[7] = get_int_range("Please enter the number of influsion times:\n", 0, 9);
    intrecord[8] = get_int_range("Please enter the number of surgeries performed:\n", 0, 9);
    for (i = 0;i < 9;i++)
    record[i] = record[i] +'0';
    record[9] = '\0';
    printf("The final diagnosis and treatment record number is:\n%s",record);
    printf("\n");
}

double get_double_range(const char* prompt, double min, double max) {
    double current_val;

    while (1) {
        printf("%s [%.2f - %.2f]: ", prompt, min, max);

        // 尝试读取一个双精度浮点数，使用 %lf
        if (scanf("%lf", &current_val) != 1) {
            printf("Error: Not a valid floating-point number!\n");
            clear_buffer(); // 读到非数字字符，清空输入缓冲区
            continue;
        }

        clear_buffer(); // 成功读取后，清掉尾随的回车符或多余字符

        if (current_val >= min && current_val <= max) {
            return current_val; }
        else {
            printf("Error: Value %.2f is out of range!\n", current_val);
        }
    }
}

// 获取用户输入的时间
Time input_time() {
    Time t;
    printf("Please enter 24-hour clock time:\n");

    t.hour = get_int_range("Hour", 0, 23);
    t.minute = get_int_range("Minute", 0, 59);

    return t;
}

// 如果 t1 晚于 t2 返回 1，否则返回 0
int is_later(Time t1, Time t2) {
    int total_mins1 = t1.hour * 60 + t1.minute;
    int total_mins2 = t2.hour * 60 + t2.minute;

    if (total_mins1 > total_mins2) {
        return 1;
    }
    else {
        return 0;
    }
}

#include <stdio.h>

// 清空输入缓冲区的辅助函数
