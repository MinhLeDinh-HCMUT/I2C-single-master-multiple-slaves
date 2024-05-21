/*
 * File:   Slave1_program.c
 * Author: MinhLeDinh
 */
#include <xc.h>
#define _XTAL_FREQ 4000000 
int index=0,led_state=0;
short clearBuffer;
int datareturn[14]={89,69,76,76,79,87,13,71,82,69,69,78,32,13}; // YELLOW / GREEN text

void I2C_Slave_Init(unsigned char address)
{
    SSPADD = address; 
    SSPSTAT = 0x80;
    SSPCON = 0x36;
    SSPCON2 = 0x01;
    TRISC3 = 1; 
    TRISC4 = 1; 
    GIE = 1;    
    PEIE = 1;
    SSPIF = 0;
    SSPIE = 1;
}
 
void main(void)
{
    TRISB=0x00;
    PORTB=0x00;
    I2C_Slave_Init(0x50); // I2C address
    while(1)
    {
        PORTBbits.RB1=led_state;
        PORTBbits.RB2=!led_state;
    }
    return;
}

void __interrupt() I2C_Slave()
{ 
    if(SSPIF == 1)
    {
        SSPCONbits.CKP=0;
        if((SSPCONbits.SSPOV)||(SSPCONbits.WCOL))
        {
            clearBuffer = SSPBUF;     // Clear buffer
            SSPCONbits.SSPOV = 0;     // Clear overflow flag
            SSPCONbits.WCOL = 0;      // Clear collision bit
            SSPCONbits.CKP = 1;
        }
        if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW)
        {
            clearBuffer = SSPBUF;
            BF = 0;
            SSPBUF = datareturn[index];    // Give data to the buffer
            index+=1;
            if(index==7) led_state=1;    
            if(index==14) 
            {
                led_state=0;
                index=0;
            }
            SSPCONbits.CKP = 1;
            while(SSPSTATbits.BF);
        }
            SSPIF = 0;
    }
}

