/*************************************************************************
	> File Name: 5uartHello.c
	> Author: Zhu Brocade Soar
	> Mail: zhujinteng2012@163.com
	> Created Time: 2018年06月25日 星期一 19时10分23秒
 ************************************************************************/

#include <msp430/msp430g2553.h>

char RX_BUFF;

// 申明
void GPIO_init(void);
void UART_init(void);
void UART_RX_ISR(void);
void UART_RX_Dect(void);
void UART_OnRX(void);

int main(void){
    WDTCTL = WDTPW + WDTHOLD;                           // 关狗
    GPIO_init();
    UART_init();
    _enable_interrupts();
    _bis_SR_register(LPM3_bits);
    return 0;
}

// 定义通用端口初始化函数
void GPIO_init(void){
    return;
}

// 定义UART初始化函数
void UART_init(void){
    // 设置SMCLK为1MHz
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
    BCSCTL2 &= ~(SELS + DIVS1 + DIVS0);
    // 开始设置UART
    // UCSWRST 软复位
    UCA0CTL1 |= UCSWRST;                    // 设置前开启软复位
    // 设置时钟源
    // UCSSEL_1 ACLK
    // UCSSEL_2 SMCLK 
    UCA0CTL1 = UCSSEL_2 + UCSWRST;          // SMCLK(1MHz)
    // 设置字符帧格式
    // 通信模式
    UCA0CTL0 &= ~UCSYNC;                    // 0:UART-Mode 1:SPI-Mode 
    // 停止位
    UCA0CTL0 |= UCSPB;                      // 0:one 1:two
    // 数据位
    UCA0CTL0 &= ~UC7BIT;                    // 0:8-bits 1:7-bits
    // 大/小端序
    UCA0CTL0 &= ~UCMSB;                     // 0:LSB 1:MSB
    // 校验使能
    UCA0CTL0 &= ~UCPEN;                     // 0:disable 1:enable
    // 校验位
    UCA0CTL0 |= UCPAR;                      // 0:odd 1:even
    // 设置波特率 9600
    UCA0MCTL = UCBRS_1 + UCBRF_0 ;          // 查表设置
    UCA0BR0 = 104;                          // 查表设置
    // 设置完毕
    UCA0CTL1 &= ~UCSWRST;                   // 设置完关闭软复位
    return;
}


// 定义中断服务函数
/* Windows - CCS - 默认编译器 情况下如下
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void){
}
*/
// 定义UART_RX中断服务函数
void __attribute__ ((interrupt (USCIAB0RX_VECTOR))) UART_RX_ISR(void){
    UART_RX_Dect();
    IFG2 &= ~UCA0RXIFG;                     // 手动清除中断标志
    return;
}

// 定义UART_RX事件检测函数（检测USCI_A0还是USCI_B0）
void UART_RX_Dect(void){
    // xxxx xxx1 & 0000 0001
    // =  0000 0001
    if((IFG2 & UCA0RXIFG) == UCA0RXIFG) UART_OnRx();
    return;
}

// 定义UART_RX事件处理函数（保存接收到的字符到RX_BUFF变量
void UART_OnRX(void){
    RX_BUFF = UCA0RXBUF;
    return;
}

