#ifndef CHECK_H
#define CHECK_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "Struct.h"


void clear_buffer(void);

void clean_fgets_string(char, int);


// 1. 校验并获取字母字符串 (字母字符串，最大长度)
void get_alpha_string(const char* prompt,char* str, int max_len);

// 2. 校验并获取整数范围 (被检验数，最小值，最大值)
int get_int_range(const char* prompt, int min, int max);

// 3. 校验并获取固定长度数字串 (数字字符串 固定长度)
void get_numeric_string_fixed(const char* prompt, char* str, int fixed_len);

// 4. 校验并获取100的倍数 (被检验数)
int get_int_multiple_100(const char* prompt );

//5.检查诊疗记录每一位都合理
void get_char_medicalrecord(char[10]);

//6.检验并获取两位小数浮点数范围
double get_double_range(const char* prompt, double min, double max);

// 7.获取用户输入的时间
Time input_time();

// 8.如果 t1 晚于 t2 返回 1，否则返回 0
int is_later(Time t1, Time t2);


#endif#
