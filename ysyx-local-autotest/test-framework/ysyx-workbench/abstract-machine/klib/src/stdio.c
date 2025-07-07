#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>
#include <limits.h> // 包含 LONG_MIN

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

void int_to_str(int num, char *str) {
    int i = 0;
    int is_negative = 0;

    // 处理负数
    // 对最小负数的处理存在一定问题,尝试的解决方式:
    if (num == -2147483648) {
        strcpy(str,"-2147483648");
        return;
    }
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    // 将每一位数字逆序存储到字符数组中
    do {
        str[i++] = num % 10 + '0'; // 将数字转换为字符
        num /= 10;
    } while (num);

    // 如果是负数，加上负号
    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0'; // 字符串结尾
    // 反转字符串
    int j = 0, k = i - 1;
    while (j < k) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
        j++;
        k--;
    }
}

void long_to_str(long num, char *str) {
    int i = 0;
    int is_negative = 0;

    // 处理负数
    if (num == LONG_MIN) {  // 特殊处理最小负数
        strcpy(str, "-9223372036854775808");
        return;
    }
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    // 将每一位数字逆序存储到字符数组中
    do {
        str[i++] = num % 10 + '0'; // 将数字转换为字符
        num /= 10;
    } while (num);

    // 如果是负数，加上负号
    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0'; // 字符串结尾

    // 反转字符串
    int j = 0, k = i - 1;
    while (j < k) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
        j++;
        k--;
    }
}


/* @brief : 将十六进制转换为字符串
 * @return: None
 */
void hex_to_string(unsigned int hex_num, char* result) {
    const char hex_digits[] = "0123456789ABCDEF";
    int i = 0;
    // Process each digit
    do {
        result[i++] = hex_digits[hex_num % 16];
        hex_num /= 16;
    } while (hex_num > 0);
    // Null terminate the string
    result[i] = '\0';
    // Reverse the string
    for (int j = 0; j < i / 2; ++j) {
        char temp = result[j];
        result[j] = result[i - j - 1];
        result[i - j - 1] = temp;
    }
}

// 通过putch实现的，但是nemu中好像不允许使用putch,我使用时会报错
int printf(const char *fmt, ...) {
    char out[1000] = ""; // printf buffer
    va_list ap;
    va_start(ap, fmt);
    uint64_t char_num = 0;
    int d;
    uint32_t x;
    char *s;
    char temp[64];

    while (*fmt != '\0') {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case '%':
                    if (char_num < sizeof(out) - 1) {
                        out[char_num++] = '%';
                    }
                    fmt++;
                    break;
                case 'd':  // Match format specifier for int
                    d = va_arg(ap, int);
                    int_to_str(d, temp);
                    if (char_num + strlen(temp) < sizeof(out)) {
                        strcat(out, temp);
                        char_num += strlen(temp);
                    }
                    fmt++;
                    break;
                case 'l':
                    if (*(fmt + 1) == 'd') {
                        fmt ++;
                        long ld = va_arg(ap,long int);
                        long_to_str (ld, temp);
                        if (char_num + strlen(temp) < sizeof(out)) {
                            strcat(out, temp);
                            char_num += strlen(temp);
                        }
                        fmt++;
                    }else if(*(fmt + 1) == 'x') {
                        assert(1);
                    }
                    break;
                case 'x':  // Match format specifier for unsigned int (hex)
                    x = va_arg(ap, unsigned int);
                    hex_to_string(x, temp);
                    if (char_num + strlen(temp) < sizeof(out)) {
                        strcat(out, temp);
                        char_num += strlen(temp);
                    }
                    fmt++;
                    break;
                case 'c':  // Match format specifier for string
                    char c = (char)va_arg(ap, int);
                    if (char_num < sizeof(out)) {
                        out[char_num++] = c;
                        out[char_num] = '\n';
                    }
                    fmt++;
                    break;
                case 's':  // Match format specifier for string
                    s = va_arg(ap, char *);
                    if (char_num + strlen(s) < sizeof(out)) {
                        strcat(out, s);
                        char_num += strlen(s);
                    }
                    fmt++;
                    break;
                default:
                    // Unsupported format specifier, can add error handling here
                    break;
            }
        } else {
            if (char_num < sizeof(out) - 1) {
                out[char_num++] = *fmt;
            }
            fmt++;
        }
    }
    out[char_num] = '\0';
    va_end(ap);

    for (uint64_t i = 0; i < char_num; i++) {
        putch(out[i]);
    }

    return char_num;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    uint64_t char_num = 0;

    int d;
    uint32_t x;
    char *s;
    char temp[64] = "0";
    while (*fmt != '\0') {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case '%':
                    out[char_num++] = '%';
                    fmt++;
                    break;

                case 'd':  // 匹配格式符为int型参数
                    d = va_arg(ap, int);
                    int_to_str(d, temp);
                    out[char_num] = '\0';
                    strcat(out,temp);
                    char_num = strlen(out);
                    fmt++;
                    break;
                case 'x':
                    x = va_arg(ap, unsigned int);
                    hex_to_string(x, temp);
                    out[char_num] = '\0';
                    strcat(out,temp);
                    char_num = strlen(out);
                    fmt++;
                    break;
                case 's':
                    s = va_arg(ap,char *);
                    out[char_num] = '\0';
                    strcat(out,s);
                    char_num = strlen(out);
                    fmt++;
                    break;
                default: ;
            }
        }else {
            out[char_num] = *fmt;
            char_num++;
            fmt++;
        }
    }
    out[char_num] = '\0';
    va_end(ap);
    return char_num;
  /*panic("Not implemented");*/
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
