#include "stdint.h"
#include "stdio.h"
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "inc/hw_types.h"
volatile long periyot=500000;  //Kesmenin ve alt programlarda kullanýlabilecek global deðiþken.

//////////////////////////////////////////////////////////////////////////////////////
void kesme(void)
{
    if((GPIO_PORTF_MIS_R & 0x01)==0x01)  //Interrupt switch 2 üzerinden baþlamaþtýr.  Periyotu azaltýr
             {

            if( periyot>50000)          //Min periyot=50000
              { periyot  = periyot - 50000;}

                 GPIO_PORTF_ICR_R=0x01;
             }
             else if((GPIO_PORTF_MIS_R & 0x10)==0x10)//Interrupt switch 1 üzerinden baþlamaþtýr.  Periyotu artýrýr
             {


                     periyot  = periyot + 50000;


                 GPIO_PORTF_ICR_R=0x10;
             }


}



//////////////////////////////////////////////////////////////////////////////////////////////


void main(void)
{
               /////GPPIO PORT AYARLARI/////
    SYSCTL_RCGCGPIO_R|=0x20;
     GPIO_PORTF_LOCK_R=0x4C4F434B;
     GPIO_PORTF_CR_R =0x1;
     GPIO_PORTF_DIR_R|=0x0E; //PortF 0 ve 4 input ve 123 output olarak ayarlandý.
     GPIO_PORTF_DEN_R|=0x1F; // Portlar aktif
     GPIO_PORTF_PUR_R = 0x11;// 0 ve 4 pull up. Butona basýnca topraða gider.


                                      //INTERRUPT AYARLARI//
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Note: To prevent false interrupts, the following steps should be taken when re-configuring//
    //GPIO edge and interrupt sense registers:                                                   //
    ///////////////////////////////////////////////////////////////////////////////////////////////

    // a. Mask the corresponding port by clearing the IME field in the GPIOIM register.
     GPIO_PORTF_IM_R&=~(1<<4)|~(1<<0);
     //b. Configure the IS field in the GPIOIS register and the IBE field in the GPIOIBE

     GPIO_PORTF_IS_R&=~(1<<4)|~(1<<0);

     GPIO_PORTF_IBE_R&=~(1<<4)|~(1<<0);

     GPIO_PORTF_IEV_R|=(1<<4)|(1<<0);
    // c. Clear the GPIORIS register.

     GPIO_PORTF_RIS_R=0X00;
     // d. Unmask the port by setting the IME field in the GPIOIM register.

     GPIO_PORTF_IM_R|=(1<<4)|(1<<0);





      NVIC_EN0_R=(1<<30);           //PortF enabled.


while(1)
    {

        GPIO_PORTF_DATA_R|=0x04;           //Pf2 Ledi yanar
        SysCtlDelay(periyot);              //x kadar bekle
        GPIO_PORTF_DATA_R &= ~(0x04);      //pf2 söndür
        SysCtlDelay(periyot);              //x kadar bekle

    }
}
