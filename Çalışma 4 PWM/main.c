#include "stdint.h"
#include"inc/tm4c123gh6pm.h"


void Delay_ms(int time_ms)//16 mhz için
{
  int i, j;
  for(i = 0 ; i < time_ms; i++)
      for(j = 0; j < 3180; j++)
          {}  /* excute NOP for 1ms */
}

int main(void)
{
    volatile unsigned long delay;
//    1. Enable the PWM clock by writing a value of 0x0010.0000 to the RCGC0 register in the System
//    Control module (see page 456).
    SYSCTL_RCGC0_R|=0X00100000; // SYSCTL_RCGC0_PWM0;
    delay = SYSCTL_RCGC0_R;
//    2. Enable the clock to the appropriate GPIO module via the RCGC2 register in the System Control
//    module (see page 464) pb6 ve pb7 idi

    SYSCTL_RCGC2_R|=0x00000002 ; //SYSCTL_RCGC2_GPIOB;
    delay = SYSCTL_RCGC2_R;

//    3. In the GPIO module, enable the appropriate pins for their alternate function using the
//    GPIOAFSEL register. To determine which GPIOs to configure, see Table 23-4 on page 1344

    GPIO_PORTB_AFSEL_R= 0xc0; // pb6 ve pb7 alternatif fonk actýk
    GPIO_PORTB_DEN_R |= 0xc0;
    // direction outpurr
    //GPIO_PORTB_DIR_R|=0xc0;
    // analog fonk kapatýrsan iyi olur
    GPIO_PORTB_AMSEL_R &= 0x00;

//    4. Configure the PMCn fields in the GPIOPCTL register to assign the PWM signals to the appropriate
//    pins (see page 688 and Table 23-5 on page 1351).
    GPIO_PORTB_PCTL_R |= 0x44000000;
    //GPIO_PORTB_PCTL_R|=GPIO_PCTL_PB6_M0PWM0|GPIO_PCTL_PB7_M0PWM1 ;

//    5. Configure the Run-Mode Clock Configuration (RCC) register in the System Control module
//    to use the PWM divide (USEPWMDIV) and set the divider (PWMDIV) to divide by 2 (000).

        SYSCTL_RCC_R |=0x00100000; //SYSCTL_RCC_USEPWMDIV //USEPWMDIV
        SYSCTL_RCC_R &= ~0x000E0000; // e 17-18-19 bit bölgesi 0 landý
      // divider 2
      // 8 mhz düþtü

//      6. Configure the PWM generator for countdown mode with immediate updates to the parameters.
//      Write the PWM0CTL register with a value of 0x0000.0000.
//      Write the PWM0GENA register with a value of 0x0000.008C.
//      Write the PWM0GENB register with a value of 0x0000.080C.
      PWM0_0_CTL_R = 0x00000000; //PWM0_CTL_R=0x00;
      PWM0_0_GENA_R |= 0x0000008C;
      PWM0_0_GENB_R |= 0x0000080C;

      //7.
      PWM0_0_LOAD_R = 320-1;

      //8
      int a=320-1;
      PWM0_0_CMPA_R = a;//240-1;// %25 DUTY

      //9
      PWM0_0_CMPB_R = 80-1; // %75 PB7

      // 10 STARTT
      PWM0_0_CTL_R = 0x00000001; // PWM0_CTL_R=0x01;

      // 11 ENABLE OUTPUT
      PWM0_ENABLE_R=0x03;

      while(1){
          PWM0_0_CMPA_R = a;
          a--;
          Delay_ms(10);
          if (a<0){
              a=320;
          }

      }

      return 0;
}
