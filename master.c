/*
 * File:   Master_program.c
 * Author: MinhLeDinh
 */
#include <xc.h>
#define _XTAL_FREQ 4000000 
#define I2C_BaudRate 100000 

void I2C_Master_Init()
{
    SSPCON = 0x28;
    SSPCON2 = 0x00;
    SSPSTAT = 0x00;
    SSPADD = ((_XTAL_FREQ/4)/I2C_BaudRate) - 1;
    TRISC3 = 1;
    TRISC4 = 1;
}
void I2C_Wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2C_Start()
{
    I2C_Wait();
    SEN = 1;
}

void I2C_Stop()
{
    I2C_Wait();
    PEN = 1;
}

unsigned char I2C_Write(unsigned char data)
{
    I2C_Wait();
    SSPBUF = data;
    I2C_Wait();
    return ACKSTAT;
}

unsigned short I2C_Master_Read()
{
    unsigned short temp;
    I2C_Wait();
    RCEN = 1;
    I2C_Wait();
    temp = SSPBUF;
    I2C_Wait();
    ACKDT = 1;
    ACKEN = 1;
    return temp;
}

void UART_init(void)    // Use for display data to virtual terminal, remove if not use
{
    BRGH=1;     
    SPBRG=25;   // Baudrate 9600 bps
    SYNC=0;
    SPEN=1;
    TRISC6=1;   
    TXEN=1;    // Transmit enabled 
}
 
void UART_write_c(char data)
{
    while(!TRMT);
    TXREG = data;
}
 
void UART_write_s(char *text) {
   for(int i = 0; text[i] != '\0'; i++){
      UART_write_c(text[i]);
   }
}

void main()
{
    TRISB = 0xFF;   // Port B direction sets as input
    I2C_Master_Init();
    UART_init();
    while(1)
    {
      if(PORTBbits.RB7==0)
      {    
        while(PORTBbits.RB7==0);
        UART_write_s("Temp:");
        for(int i=0;i<2;i++)
        {
            I2C_Start(); 
            I2C_Write(0x41);    // Slave 2 address / read data from slave mode
            UART_write_c(I2C_Master_Read());    // Display received data in virtual terminal
            I2C_Stop();
        }
        UART_write_s(" C\r");
      }
      if(PORTBbits.RB6==0)
      {    
        while(PORTBbits.RB6==0);
        for(int j=0;j<7;j++)
        {
            I2C_Start(); 
            I2C_Write(0x51);    // Slave 1 address / read data from slave mode
            UART_write_c(I2C_Master_Read());    // Display received data in virtual terminal
            I2C_Stop();
        }
      }
    }
}
