

#ifndef LCD_H_
#define LCD_H_

#define LCDPORT                          GPIO_PORTB_BASE
#define rs                               GPIO_PIN_0 // pb0
#define E                                GPIO_PIN_1 // pb1
#define d4                               GPIO_PIN_4 // lcd db4=pb4
#define d5                               GPIO_PIN_5 // lcd db5=pb5
#define d6                               GPIO_PIN_6 // lcd db6=pb6
#define d7                               GPIO_PIN_7 // lcd db7=pb7

void LcdTemizle(void);
void LcdKomut (unsigned char);
void lcdyaz(unsigned char);
void lcdgotoxy(unsigned int, unsigned int);
void lcdfonksiyonayarla( unsigned char,unsigned char,unsigned char); // dl,N,F
void lcdilkayarlar(void);
void lcdyaziyaz(char*);


#endif /* LCD_H_ */
