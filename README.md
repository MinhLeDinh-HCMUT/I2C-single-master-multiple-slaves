# Single master - multiple slaves I2C communication on PIC microcontroller
## PIC microcontroller that can be used
- PIC16F877
- PIC16F877A
- PIC16F887
## IDE
- MPLAB X IDE (XC8 Compiler)
## Task
- Utilize Special Function Registers (SFR), do not use built-in functions.
- The button on pin RB6 is associated with the Slave 1, where as the button on pin RB7 is associated with the Slave 2.
- When the button on RB6 pin of the Master is pressed, the leds' state on the Slave 1 changed. If the green led is on, the yellow led is off and vice versa.
- The led' status is sent back to the Master and displayed on the virtual terminal. 
- When the button on RB6 pin of the Master is pressed, the ADC value of the LM35 temperature sensor is sent from the second Slave to the Master.
- The yelow led on the Slave 2 is used as an indicator of successful transmission. The temperature value is then shown on the virtual terminal.
## Simulation video:
- Link: https://drive.google.com/file/d/1CQJ3V6JcGY7mu1GlnrNdNdldFCkgBXFW/view?usp=sharing
