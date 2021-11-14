#include "stdint.h"
#include "stdio.h"
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"

void Yanson(unsigned long x)
{


    GPIO_PORTF_DATA_R|=0x04; //Pf2 Ledi yanar
    SysCtlDelay(x);              //x kadar bekle
    GPIO_PORTF_DATA_R &= ~(0x04);//pf2 s�nd�r
    SysCtlDelay(x);              //x kadar bekle


}
void main(void)
{
    SYSCTL_RCGCGPIO_R|=0x20;
     GPIO_PORTF_LOCK_R=0x4C4F434B;
     GPIO_PORTF_CR_R =0x1;
     GPIO_PORTF_DIR_R|=0x0E; //PortF 0 ve 4 input ve 123 output olarak ayarland�.
     GPIO_PORTF_DEN_R|=0x1F; // Portlar aktif
     GPIO_PORTF_PUR_R = 0x11;// 0 ve 4 pull up. Butona bas�nca topra�a gider.
unsigned int x=250000;


while(1)
    {
    Yanson(x);



        while(!(GPIO_PORTF_DATA_R&0x10) )  //Parantez i�i d��meye bas�lm��sa 0 olur. B�ylece while d�ng�s� aktifle�ir
        {



            Yanson(x);  // Butona bas�ld���nda ledin s�nememsi i�in



                   if((GPIO_PORTF_DATA_R&0x10) )

                    {
                         x=x+25000;

                    }
        }
        while( !(GPIO_PORTF_DATA_R&0x01) && x>25000) //2500 minimum yan�p s�nme periyot de�eri
        {

            Yanson(x);  // Butona bas�ld���nda ledin s�nememsi i�in

            if( (GPIO_PORTF_DATA_R&0x01))

                      {
                         x=x-25000;

                      }

        }

    }
}
