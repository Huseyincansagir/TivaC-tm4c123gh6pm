#include "stdint.h"
#include "stdio.h"
#include"inc/tm4c123gh6pm.h"

char data[3]={65,66,67};

void init_i2c();
uint8_t Read(){
    I2C0_MSA_R= (0x55<<1)+0; // adres + write
    //I2C0_MDR_R='w';
    I2C0_MCS_R = 0x07; //stop start run
    while((I2C0_MCS_R &= 0x00000040)==1); //poll busy bit

    I2C0_MSA_R= (0x55<<1)+1; // adres + write
    I2C0_MCS_R=0x07; // haberleþme baþladý
    while((I2C0_MCS_R &= 0x00000040)==1); //poll busy bit

    return I2C0_MDR_R;
}

void Write(uint8_t data){
    I2C0_MSA_R= (0x55<<1)+0; // adres + write
    I2C0_MDR_R=data;
    I2C0_MCS_R = 0x07; //stop start run
    while((I2C0_MCS_R &= 0x00000040)==1); //poll busy bit
}
void WriteMulti(char datam[],int adet){
    I2C0_MSA_R= (0x55<<1)+0; // adres + write

    I2C0_MDR_R='K' ; // baþlama
    I2C0_MCS_R = 0x03; // start run
    while((I2C0_MCS_R &= 0x00000040)==1); //poll busy bit

    int i=0;

    while (i<adet){
        if ((adet==1) || (i==adet-1) ){
            I2C0_MDR_R=datam[i] ;
            I2C0_MCS_R = 0x07; //stop start run
            while((I2C0_MCS_R &= 0x00000040)==1); //poll busy bit
            break;

        }else {
            I2C0_MDR_R=datam[i] ; // yada i .cisi =0
            I2C0_MCS_R = 0x01; //stop start run
            while((I2C0_MCS_R &= 0x00000040)==1); //poll busy bit
            i++;
        }

    }
}
int main()
{
    // topraklar ortak
    init_i2c();
    WriteMulti(data,3);
    Write('A');
    uint8_t gelendeger=Read();
}

void init_i2c()
{
    // hiç bir saat ayarý yapmadýysak 16mhz
//    1. Enable the I2C clock using the RCGCI2C register in the System Control module (see page 348).
    SYSCTL_RCGCI2C_R|=0x01;

//    2. Enable the clock to the appropriate GPIO module via the RCGCGPIO register in the System
//    Control module (see page 340). To find out which GPIO port to enable, refer to Table
//    23-5 on page 1351.
    SYSCTL_RCGCGPIO_R|=0x02;

//    3. In the GPIO module, enable the appropriate pins for their alternate function using the
//    GPIOAFSEL register (see page 671). To determine which GPIOs to configure, see Table
//    23-4 on page 1344.
    GPIO_PORTB_DEN_R |=0x0c;  // pb2 ve pb3 aktif 0000 1100

    GPIO_PORTB_AFSEL_R |=0x0c;

//    4. Enable the I2CSDA pin for open-drain operation. See page 676
    GPIO_PORTB_ODR_R|=0x08;  // sdb için open drain yapýldý

    // 5. Configure the PMCn fields in the GPIOPCTL register to assign the I2C signals to the appropriate
   // pins. See page 688 and Table 23-5 on page 1351.

    GPIO_PORTB_PCTL_R|=0x03; // ??

//    6. Initialize the I2C Master by writing the I2CMCR register with a value of 0x0000.0010.

    I2C0_MCR_R|=0x0010;

//    7. Set the desired SCL clock speed of 100 Kbps by writing the I2CMTPR register with the correct
//    value. The value written to the I2CMTPR register represents the number of system clock periods
//    in one SCL clock period. The TPR value is determined by the following equation:
//    TPR = (System Clock/(2*(SCL_LP + SCL_HP)*SCL_CLK))-1;
//    TPR = (16MHz/(2*(6+4)*100000))-1;
//    TPR = 7
//    Write the I2CMTPR register with the value of 0x0000.0009.
    I2C0_MTPR_R=7; // 100 kbps



}
