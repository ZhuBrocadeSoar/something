/*************************************************************************
	> File Name: 2intFromIo.c
	> Author: Zhu Brocade Soar
	> Mail: zhujinteng2012@163.com
	> Created Time: 2018年06月24日 星期日 22时53分01秒
    1) 当按钮按下时，P1.3从高电平向低电平跳变，这个跳变匹配到
    2) P1.3监听的下降沿中断，
    3) CPU先将P1IFG置位，然后进行中断调用
    4) 调用中断向量指向的PORT1_ISR()中断服务函数
    5) 中断服务函数调用事件检测函数，
    6) 事件检测函数检测到P1IN相应位置的输入和P1IFG相应位置匹配于是调用事件处理函数
    7) 事件处理函数翻转P1.0和P1.6的输出
 ************************************************************************/

#include <msp430/msp430g2553.h>

// 申明不定义
void P1_IODect(void);
void P13_Onclick(void);
void GPIO_init(void);

// 主函数
int main(void){
    WDTCTL = WDTPW + WDTHOLD;       // 关狗
    GPIO_init();                    // 端口初始化
    _enable_interrupts();           // 使能中断
    _bis_SR_register(LPM3_bits);    // 进入模式LPM3
    return 0;
}

// 定义端口初始化函数
void GPIO_init(void){
    // 设置P1.3为外部中断输入
    P1DIR &= ~BIT3;                 // 输入
    P1REN |= BIT3;                  // 上下拉电阻使能
    P1OUT |= BIT3;                  // 上拉
    P1IE |= BIT3;                   // 中断使能
    P1IES |= BIT3;                  // 下降沿中断
    // 设置P1.0和P1.6为输出
    P1DIR |= BIT0 + BIT6;
    P1OUT |= BIT0;
    P1OUT &= ~BIT6;
    return;
}

// 定义中断服务函数
/* Windows - CCS - 默认编译器 情况下如下
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void){
}
*/
void __attribute__ ((interrupt (PORT1_VECTOR))) PORT1_ISR(void){
    P1_IODect();                    // 事件检测函数
    P1IFG = 0;                      // 手动清除中断标志
    return;
}

// 定义事件检测函数 
void P1_IODect(void){
    unsigned int Push_Key = 0;
    //      0000 1000 & xxxx 1xxx
    //      =  0000 1000
    Push_Key = P1IFG & (~P1DIR);
    __delay_cycles(10000);
    // xxxx 0xxx & 0000 1000
    // =  0000 0000
    if((P1IN & Push_Key) == 0){
        switch(Push_Key){
            case BIT3: P13_Onclick(); break;
            default: break;
        }
    }
    return;
}

// 定义事件处理函数
void P13_Onclick(void){
    // 翻转P1.0和P1.6输出
    P1OUT ^= BIT0;
    P1OUT ^= BIT6;
    // 等价于 P1OUT ^= BIT0 + BIT6;
    return;
}

