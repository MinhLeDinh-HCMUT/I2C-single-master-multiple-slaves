/*
 * File:   Slave2_program.c
 * Author: MinhLeDinh
 */
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define _XTAL_FREQ 4000000 

int index=0,temp=0;
short clearBuffer;
uint16_t adc_res=0;
char temp_str[16];

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

void ADC_Init()
{
   ADCON0=0b10000001;   
   ADCON1=0b10001001; 
}

void main(void)
{
    TRISB=0x00;
    PORTB=0x00;
    I2C_Slave_Init(0x40); // I2C address
    ADC_Init();
    while(1)
    {
    __delay_us(50);     // Delay for acquisition time (min=20us)
    GO_DONE = 1;    // Start A/D conversion
    while(ADCON0bits.GO_DONE);
    adc_res = ((ADRESH << 8) + ADRESL);   // 10-bit right justified result
    temp = adc_res/2.046;
    sprintf(temp_str,"%d",temp);    // Conversion to string
    }
    return;
}


void __interrupt() I2C_Slave()
{ 
    if(SSPIF == 1)
    {
        if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW)
        { 
            clearBuffer = SSPBUF;
            BF = 0;
            SSPBUF = temp_str[index];    // Give data to the buffer
            index+=1;           
            SSPCONbits.CKP = 1;
            while(SSPSTATbits.BF);
            if(index==2) 
            {
                index=0; 
                PORTBbits.RB0=1;    // Sending signal
                __delay_ms(200);
                PORTBbits.RB0=0;
            }
        }
            SSPIF = 0;
    }
}

