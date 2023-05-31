// 请确保此文件使用 UTF-8 编码
#include <stdio.h>
#include <stdint.h>
#include <locale.h>
#include <windows.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

void toBin(uint16_t hex_number);
int isValidHexString(const char *str);
int main() {
    // 设置控制台输出字符集为UTF-8
    SetConsoleOutputCP(CP_UTF8);
    // 设置本地区域配置为UTF-8
    setlocale(LC_ALL, "UTF-8");

    // 需要解析的十六进制数据
    // 0001 1111 0001 1111
    int16_t hex_number = 0;
    while(1) {
        // 承接输入的十六进制字符串
        char input_buf[256];
        int valid_input = 0;
        printf("功能选择: 1、解析电压，2、解析电流，q退出程序");
        while (!valid_input)
        {
            printf("输入一个十六进制数 (例如：F424): ");
            // 读取整行输入，类型时字符串
            fgets(input_buf, sizeof(input_buf), stdin);
            if(input_buf[0] == 'q') {
                exit(0);
            }

            // 移除换行符，并检查输入的字符串是否为有效的十六进制数
            input_buf[strcspn(input_buf, "\n")] = 0;
            // 校验
            if (isValidHexString(input_buf))
            {
                // 校验通过的十六进制字符串赋值给hex_number
                sscanf(input_buf, "%x", &hex_number);
                valid_input = 1;
            }
            else
            {
                printf("错误：请输入正确的十六进制数！\n");
            }
        }
        printf("输入的十六进制数据二进制为：");
        toBin(hex_number);
        printf("输入的十六进制的十进制为: %d\n", hex_number);


        // 最高1代表负数
        // 截取高五位
        // uint16_t high = hex_number >> 11 & 0x1F;
        // 0000 0000 0000 1101
        // uint16_t无符号, int16_t 有符号
        // 1110111011101110
        // 1111 1111 1111 1101
        // 1111 1000 0000 0000
        // 1000000100100011
        // 0000000100100011
        int16_t high = (hex_number & 0x7FFF)  >> 11;
        // 1111 1111 1111 1101
        // 1000000000001111
        printf("高五位移动11位后的二进制数据：");
        toBin(high);
        // 判断
        int16_t high_hyd = high;
        if (hex_number < 0) {
            high_hyd = 16 - high;
            // 1111 1111 1111 1101
            // 1000000000001111
            printf("高五位移动11位后的二进制数据：");
            toBin(high_hyd);
            high_hyd *=  -1;
        }
        printf("高五位十进制数据: %d\n", high_hyd);

        // 1000 0111 1111 1111
        uint16_t low = hex_number & 0x07FF;
        printf("低十一位二进制:");
        toBin(low);
        // 低十一位
        printf("低十一位十进制数据: %d\n", low);
        double result = pow(2, high_hyd);
        printf("指数结果：%0.6f\n", result);
        printf("解析结果是：%0.6f\n", low * pow(2, high_hyd));
        hex_number = 0;
    }
    return 0;
}

// 打印数据的二进制
void toBin(uint16_t hex_number) {
    int bit_count = sizeof(uint16_t) * 8;

    // 从高位到低位依次打印二进制位
    for (int i = bit_count - 1; i >= 0; i--) {
        putchar((hex_number >> i) & 1 ? '1' : '0');
    }
    printf("\n");
}

// 校验输入的十六进制是否合法
int isValidHexString(const char *str)
{
    for (size_t i = 0; i < strlen(str); ++i)
    {
        char c = tolower(str[i]);
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')))
        {
            return 0;
        }
    }
    return 1;
}