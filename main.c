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

    // 电流或者温度
    int16_t hex_number = 0;

    // 电压是无符号十六进制
    uint16_t voltage = 0;
    // 功能选择
    int type = 0;

    // 承接输入的十六进制字符串
    char input_buf[256];
    int valid_input = 0;
    // 功能选择成功标志位
    char funFlag = 0;
    while(1) {
        printf("功能选择: 1: 解析电流，温度，2: 解析输出电压，3：清屏，q退出程序 \n");
        while (1) {
            
            if (scanf("%d", &type) == 1) {
                switch(type) {
                    case 1: 
                    case 2: funFlag = 1; break;
                    case 3: system("cls"); printf("功能选择: 1: 解析电流，温度，2: 解析输出电压，3：清屏，q退出程序 \n");break;
                    default: printf("请选择功能1或者2！\n");
                }
                // 选择正常功能跳出循环
                if (funFlag) {
                    break;
                }
            } else {
                char temp = getchar();
                if (temp == 'q' || temp == 'Q') {
                    return 0;
                }
                printf("输入错误，请重新输入！\n");
            }
        }
        // 清空输入流，防止影响十六进制的输入，fgets和scanf读取的输入流是同一个
        getchar();
        while (!valid_input)
        {
            printf("输入一个十六进制数 (例如：F424): ");
            // 读取整行输入，类型是字符串
            fgets(input_buf, sizeof(input_buf), stdin);

            // 移除换行符，并检查输入的字符串是否为有效的十六进制数
            input_buf[strcspn(input_buf, "\n")] = 0;
            // 校验
            if (isValidHexString(input_buf))
            {
                // 校验通过的十六进制字符串赋值给voltage或者hex_number
                if (type == 2) {
                    sscanf(input_buf, "%x", &voltage);
                } else {
                    sscanf(input_buf, "%x", &hex_number);
                }
                valid_input = 1;
            }
            else
            {
                printf("错误：请输入正确的十六进制数！\n");
            }
        }
        if (type == 2) {
            printf("电压：%0.2f \n", voltage / pow(2, 12));
            hex_number = 0;
            voltage = 0;
            type = 0;
            valid_input = 0;
            funFlag = 0;
            continue;
        }

        // printf("输入的十六进制数据二进制为：");
        // toBin(hex_number);
        // printf("输入的十六进制的十进制为: %d\n", hex_number);


        // 最高1代表负数
        // 截取高五位
        // uint16_t无符号, int16_t 有符号
        int16_t high = (hex_number & 0x7FFF)  >> 11;
        // printf("高五位移动11位后的二进制数据：");
        // toBin(high);
        // 判断4096
        int16_t high_hyd = high;
        if (hex_number < 0) {
            high_hyd = 16 - high;
            // printf("高五位移动11位后的二进制数据：");
            // toBin(high_hyd);
            high_hyd *=  -1;
        }
        // printf("高五位十进制数据: %d\n", high_hyd);

        // 1000 0111 1111 11111
        uint16_t low = hex_number & 0x07FF;
        // printf("低十一位二进制:");
        // toBin(low);
        // 低十一位
        // printf("低十一位十进制数据: %d\n", low);
        double result = pow(2, high_hyd);
        // printf("指数结果：%0.6f\n", result);
        // printf("解析结果是：%0.6f\n", low * result);

        printf("输出数据%0.2f\n", low * result);
        // 重置
        hex_number = 0;
        voltage = 0;
        type = 0;
        valid_input = 0;
        funFlag = 0;
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