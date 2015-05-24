#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "gpio.h"
int main ()
{
    gpio_init();                        //  オマジナイ
    gpio_configure(25, GPIO_INPUT);     //  GPIO_25 を出力に設定
    gpio_configure_pull(25, GPIO_PULLUP);   //  プルアップ抵抗を有効化
    gpio_configure(18, GPIO_INPUT);     //  GPIO_18 を出力に設定
    gpio_configure_pull(18, GPIO_PULLDOWN); //  プルダウン抵抗を有効化
    while (1) {
        int val;
        val = gpio_read(25);            //  ピン電圧を読み込む
        printf("input(25): %d\n", val); //  0/1 をプリント
        val = gpio_read(18);            //  ピン電圧を読み込む
        printf("input(18): %d\n", val); //  0/1 をプリント
        usleep(500000);                 //  0.5秒待ち
    }
    return 0;
}

