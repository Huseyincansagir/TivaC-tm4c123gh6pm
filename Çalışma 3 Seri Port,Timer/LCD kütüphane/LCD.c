
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "LCD.h"

void otuzhexgonder()
{
    SysCtlDelay(50000);
    GPIOPinWrite(LCDPORT, d4|d5|d6|d7, 0x30);
    GPIOPinWrite(LCDPORT, E, 2); // rs=0
    SysCtlDelay(200);
    GPIOPinWrite(LCDPORT, E, 0); // rs=0
}

void lcdilkayarlar(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); // portb aktif
    GPIOPinTypeGPIOOutput(LCDPORT, 0xFF); // butun pinler output

    GPIOPinWrite(LCDPORT, rs, 0); // rs=0
    otuzhexgonder();
    otuzhexgonder();
    otuzhexgonder();

    SysCtlDelay(50000);

    ////////////////////////////////////////////////////
    // buray� 07.11.2020 de ekledim
    GPIOPinWrite(LCDPORT, d4 | d5 | d6 | d7,  0x20 );
    GPIOPinWrite(LCDPORT, E, 0x02);
    SysCtlDelay(200);
    GPIOPinWrite(LCDPORT, E, 0x00);
    SysCtlDelay(50000);
    ////////////////////////////////////////////////////

    // dl=0 4 bit
    //lcdfonksiyonayarla(4,2,) bu fonksiyonu yaz
    // normalde LcdKomut(0x28); // dl=0 4 bit --- N=1 2 sat�r --- F=0 5x7 lcd
    // olmas� gerekirdi.

    LcdKomut(0x06); // i/d=1 , s=0
    LcdKomut(0x0C); // D=1, C=1, B=1
    LcdKomut(0x01); // ekran sil

}

void LcdKomut (unsigned char c)
{
    GPIOPinWrite(LCDPORT, rs, 0); // rs=0
    GPIOPinWrite(LCDPORT, d4|d5|d6|d7, (c & 0xF0));
    GPIOPinWrite(LCDPORT, E, 2); // rs=0
    SysCtlDelay(2000);
    GPIOPinWrite(LCDPORT, E, 0); // rs=0

    SysCtlDelay(50000); // yeni de�i�tirdim

    GPIOPinWrite(LCDPORT, rs, 0); // rs=0
    GPIOPinWrite(LCDPORT, d4|d5|d6|d7, (c & 0x0F)<<4);
    GPIOPinWrite(LCDPORT, E, 2); // rs=0
    SysCtlDelay(2000);
    GPIOPinWrite(LCDPORT, E, 0); // rs=0

    SysCtlDelay(50000); // yeni ekledim
}

extern void LcdTemizle(void)
{
    LcdKomut(0x01);
}

void lcdfonksiyonayarla( unsigned char dl,unsigned char N,unsigned char F) // dl,N,F
{
// dl i�in ya 4 yada 8
// N i�in 1 i�in 1 sat�ra, 2 gelirse 2 sat�r
// F i�in 7 gelise 5x7 fontu
   // e�er 10 gelirse de 5x10 fonutunu

if (dl==1)
{
    if (N==1)
    {
        if (F==1)
        {
            LcdKomut(0x3C); //0011 1100
        }
        else  //F=0
        {
            LcdKomut(38); //0011 1000
        }

    }
    else //N=0
    {
        if (F==1)
                  {
                      LcdKomut(34); // 0011 0100
                  }
                  else
                  {
                      LcdKomut(30); // 0011 0000
                  }
    }

}
else //dl=0
{
    if (N==1)
           {
               if (F==1)
               {
                   LcdKomut(0x2C); //0010 1100
               }
               else  //F=0
               {
                   LcdKomut(28); // 0010 1000
               }

           }
           else //N=0
           {
               if (F==1)
                 {
                     LcdKomut(24); // 0010 0100
                 }
                 else   //F=0
                 {
                     LcdKomut(20); // 0010 0000
                 }
           }
}

}


void lcdyaz(unsigned char c)
{
    GPIOPinWrite(LCDPORT, rs, 0x01); // rs=1
    GPIOPinWrite(LCDPORT, d4|d5|d6|d7, (c & 0xF0));
    GPIOPinWrite(LCDPORT, E, 0x02); // rs=1
    SysCtlDelay(2000);
    GPIOPinWrite(LCDPORT, E, 0); // rs=1

    SysCtlDelay(50000);

    GPIOPinWrite(LCDPORT, rs, 0x01); // rs=1
    GPIOPinWrite(LCDPORT, d4|d5|d6|d7, (c & 0x0F)<<4);
    GPIOPinWrite(LCDPORT, E, 0x02); // rs=1
    SysCtlDelay(2000);
    GPIOPinWrite(LCDPORT, E, 0); // rs=1

    SysCtlDelay(50000);

}

void lcdyaziyaz( char* s){

        while(*s)
                lcdyaz(*s++);
}

void lcdgotoxy(unsigned int x, unsigned int y)
{

    /*
    ////////////////////////////////////////////////
    //80 81 82 83 84 85 86 87 88 89 8A 8B 8C 8D 8F//
    //C0 C1 C2 C3 C4 C5 C6 C7 C8 C9 CA CB CC CD CF//
    ////////////////////////////////////////////////

 Y  //////////////////////////////////////////////////
 1  ///1  2  3  4  5  6  7  8 9 10 11 12 13 14 15 16//X
 2  ///1  2  3  4  5  6  7  8 9 10 11 12 13 14 15 16//X
    //////////////////////////////////////////////////
    */


x=x-1; // kursoru ayarlad�ktan sonra bir sonraki kursora ge�ti�i i�in bir eksi�i yap�ld� ki istenilen konuma karakter koyulsun.
    int b=0x80;

    if (y==2)    //2. sat�rda ise 2. sat�r�n 1. h�cresinin adresi toplanmak �zere ayarland�
    {
        b=0xC0;
    }

    else if (y==1)   //1. sat�rda ise 1. sat�r�n 1. h�cresinin adresi toplanmak �zere ayarland�
        {
           b=0x80;
        }
    b=b+x;  //x ekseninde kayd�rma ayarland�.
    LcdKomut(b); // lcd ye
}



