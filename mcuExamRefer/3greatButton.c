/*************************************************************************
	> File Name: 3greatButton.c
	> Author: Zhu Brocade Soar
	> Mail: zhujinteng2012@163.com
	> Created Time: 2018年06月24日 星期日 23时43分00秒
    1) 程序开始运行后，看门狗每16ms变成饿狗咬人(由于设置了看门狗中断)
    2) 咬人时进行中断调用
    3) 调用中断向量指向的WDT_ISR()中断服务函数
    4) 中断服务函数调用事件检测函数，
    5) 事件检测比较上一个中断发生时(16ms前)和当前的P1IN对应位置的输入于是酌情调用事件处理函数
    6) 事件处理函数翻转P1.0和P1.6的输出
 ************************************************************************/

#include <msp430/msp430g2553.h>

void P1_IODect(void);
void P13_Onclick(void);
void GPIO_init(void);
void WDT_init(void);

int main(void){
    WDTCTL = WDTPW + WDTHOLD;               // 关狗
    GPIO_init();
    WDT_init();
    _enable_interrupts();
    _bis_SR_register(LPM3_bits);
    return 0;
}

void GPIO_init(void){
    P1DIR |= BIT0 + BIT6;                   // P1.0和P1.6输出
    P1OUT |= BIT0;                          // P1.0 初值
    P1OUT &= ~BIT6;                         // P1.6 初值
    P1OUT |= BIT3;                          // P1.3 输入
    P1REN |= BIT3;                          // P1.3 使能上拉电阻
    return;
}

void WDT_init(void){
    WDTCTL = WDT_ADLY_16;                   // 设置狗饿时间为16ms
                                            // 等价于
                                            // WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS1
    IE1 |= WDTIE;                           // 使能看门狗中断
    return;
}

// 定义中断服务函数
/* Windows - CCS - 默认编译器 情况下如下
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void){
}
*/
void __attribute__ ((interrupt (WDT_VECTOR))) WDT_ISR(void){
    P1_IODect();                    // 事件检测函数
    return;
}

// 定义事件检测函数
void P1_IODect(void){
    // 这里的KEY_Now 和KEY_Past 只做标志位，只做0和1用
    static unsigned char KEY_Now = 0;       // 静态变量在return时被保留
    unsigned char KEY_Past = 0;
    KEY_Past = KEY_Now;
    // 0000 1000 & 0000 1000
    // =  0000 1000 (非0)
    if(P1IN & BIT3) KEY_Now = 1;
    else KEY_Now = 0;
    if((KEY_Past == 1) && (KEY_Now == 0)) P13_Onclick();
}

// 定义事件处理函数
void P13_Onclick(void){
    // 翻转P1.0和P1.6输出
    P1OUT ^= BIT0;
    P1OUT ^= BIT6;
    // 等价于 P1OUT ^= BIT0 + BIT6;
    return;
}

