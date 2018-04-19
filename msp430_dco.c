/*************************************************************************
	> File Name: dco.c
	> Author: 
	> Mail: 
	> Created Time: 2018年04月19日 星期四 14时06分31秒
    > DCO 数控振荡器调节原理和方法叙述：
    1. 按照手册msp430g2553这款单片机没有Rosc功能，也就是输入DC Generator直流生成器的输入电阻不受寄存器DCOR控制，为默认的某个输入值
    2. DC Generator受SCG0全局状态器的控制关闭输出或打开输出
    3. DC Generator受RSELx寄存器的控制输出一定幅值的直流量
    4. DCO数控振荡器接受一定幅值的直流量，该直流量决定了基础震荡频率的大小（粗调）
    5. DCO数控振荡器受DCOx寄存器的控制基于该值对基础震荡频率上调震荡频率（微调）
    6. DCO数控振荡器输出调节后的两个震荡信号fDCOn和fDCOn+1，这两个信号后者比前者频率大一个微调等级
    7. Modulator混频器将fDCOn和fDCOn+1两个信号进行按比例混合，比例基于MODx的值，使得混频后的输出信号满足小数分频的调节功能
    8. 混频后输出的信号受SELS子主频时钟源信号选择寄存器的控制，如果该值为0则选择混频后输出信号即DCO输出，如果该值为1则丢弃混频输出信号选择LFXT1震荡信号
    9. 被选择的输出信号送入Divider分频器，Divider分频器受DIVSx寄存器控制将输入信号进行1/2/4/8的分频
    10. 分频后的信号受SCG1全局状态器的控制选择是否输出到SMCLK总线
 ************************************************************************/

#include <msp430/msp430g2553.h>

int main(void){
    WDTCTL = WDTPW | WDTHOLD;
    /*
     * 非校验的1MHz
    DCOCTL |= DCO1 | MOD2 | MOD0;
    BCSCTL1 |= RSEL2 | RSEL1 | RSEL0;
    */
    // 校验的1MHz
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;

    // 设置SMCLK源为DCOCLK 设置分频为1
    BCSCTL2 &= ~(SELS | DIVS1 | DIVS0);
    return 0;
}
