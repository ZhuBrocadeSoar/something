/*************************************************************************
	> File Name: 4timeAToPwm.c
	> Author: Zhu Brocade Soar
	> Mail: zhujinteng2012@163.com
	> Created Time: 2018年06月25日 星期一 16时09分41秒
    1) 从msp430g2553.h中得知:
        a) Timer_A 计时器模块共有 Timer0_A3 和 Timer1_A3两个
        b) 我们研究的是Timer0_A3
        c) 例如TA0CCTL0或如TA0CCR0是Timer0_A3模块的寄存器的实际名字
        例如TACCTL0或如TACCR0或如CCTL0或如CCR0等是寄存器名的别名
        d) 书上提到的TAx应该指Timer0_A3的三个通道的输出(可在引脚复用功能表中找到)
    2) 书中PWM库虽说不能用但是实际上把用到的函数抠出来放到主函数同文件下也是一样的
    3) 把库抠出来会使得通用性和维护性很差
    4) Timer_A 有8种输出模式通过OUTMODx控制，用于生成PWM信号输出的有:
        a) 模式7 超前PWM，这是书上PWM-LED亮度控制例程所用的PWM信号，该模式
        该模式，通过CCR0设置输出PWM信号的频率，TA0输出不是所需的PWM信号
        通过CCR1/2设置输出PWM信号的占空比，TA1/2输出对应占空比和频率的PWM信号
        CCR1/2与CCR0的比值决定占空比
        b) 模式3 滞后PWM，这与模式7控制方法相同，但相位不同
        c) 模式2和模式6 带死区的双路PWM，这是一种特殊的形式，一般两路协同工作。控制方法同模式3/7。
        d) 模式4 三路50%占空比相移方波，该模式占空比不可控恒定为50%，
        该模式，通过CCR0设置信号频率，TA0输出该频率50%占空比的方波基波，
        通过CCR1/2设置输出方波相对TA0输出方波相位超前量，TA1/2输出对应超前量的方波
    5) 下述例程只控制一路超前PWM信号
 ************************************************************************/

#include <msp430/msp430g2553.h>

// 申明
void PWM_init(void);
void PWM_permill(int duty);
void GPIO_init(void);
void WDT_init(void);
void WDT_ISR(void);
void P1_IODect(void);
void P13_Onclick(void);

int main(void){
    WDTCTL = WDTPW + WDTHOLD;               // 关狗
    PWM_init();                             // PWM功能(Timer0_A3)初始化
    GPIO_init();                            // 通用端口(按钮和LED)初始化
    WDT_init();
    _enable_interrupts();                   // 使能中断
    _bis_SR_register(LPM3_bits);            // 进入模式LPM3
    return 0;
}

// 定义PWM初始化函数
// 将Timer0_A3设置为超前PWM输出
void PWM_init(void){
    // 设定时钟源
    // TASSEL_0 TACLK
    // TASSEL_1 ACLK
    // TASSEL_2 SMCLK
    // TASSEL_3 INCLK
    TA0CTL |= TASSEL_1;                     // ACLK
    // 设定分频
    // ID_0 1
    // ID_1 2
    // ID_2 4
    // ID_3 8
    TA0CTL |= ID_0;                         // 分频 1
    // 设置计数模式
    // MC_0 Stop 不计数
    // MC_1 Up to CCR0 增计数
    // MC_2 Continous Up 连续增计数
    // MC_3 Up/Down 增减计数
    // 设置通道1输出模式，通道0和通道2不使用无需设置输出模式
    // OUTMODx 输出模式x
    TA0CCTL1 = OUTMOD_7;                    // 超前PWM
    // 引脚输出设置统一放在GPIO_init()中
    // 设置PWM频率(该值实际上是计数次数(也就是周期)，对应周期是x个输入计时器的时钟周期)
    TA0CCR0 = 500;                          // 取值范围: 0~65535(uint_16, 16位无符号整形)
    return;
}

// 定义通道1(超前PWM)占空比设定函数
// 输入参数 duty 取值范围 0~1000 单位千分之一
void PWM_permill(unsigned int duty){
    if(duty > 1000){
        duty = 1000;
    }
    TA0CCR1 = duty * TA0CCR0 / 1000;
    return;
}

// 其他PWM模式占空比设定类似，自行参考课本P97 P98例程

// 定义通用端口初始化函数
void GPIO_init(void){
    // 设定按钮
    P1DIR &= ~BIT3;                         // 输入
    P1REN |= BIT3;                          // 使能上下拉电阻 
    P1OUT |= BIT3;                          // 上拉电阻
    // 设定TA0.1(P1.6)
    P1DIR |= BIT6;                          // 输出
    P1SEL |= BIT6;                          // 功能选择(作为TA0.1，即课本提到的TA1)
    // P1SEL &= ~BIT?; 手动设置TA0.2对应的引脚的功能选择，具体看引脚功能表或者引脚原理图，课本给出的例程对应单片机没有这个引脚。
    return;
}

// 定义看门狗初始化函数
void WDT_init(void){
    WDTCTL = WDT_ADLY_16;
    IE1 |= WDTIE;
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
    static unsigned int Bright = 0;         // 静态变量作为状态机
    Bright = (Bright + 40) % 400;           // 超过40%占空比变化不明显，在40%处回环
    PWM_permill(Bright);                    // 改变占空比
    return;
}

