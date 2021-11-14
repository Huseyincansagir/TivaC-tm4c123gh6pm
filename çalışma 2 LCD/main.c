

/**
 * main.c
 */
#include"inc/tm4c123gh6pm.h"

#include "LCD.h"

int main(void)
{
   lcdilkayarlar();
   lcdgotoxy(7, 1); //80h
   lcdyaz('a');


    return 0;
}
