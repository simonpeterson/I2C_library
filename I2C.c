#include "xc.h"
#include <p24Fxxxx.h>
#include <p24FJ64GA002.h>
#include "delay_int.h"
void lcd_cmd(char Package){
    //These nops can be used for debugging purposes. decomment the nops here and also at the bottom of the
    //function and add breakpoints to be able to view the I2CxSTAT register
    /*
    asm("nop");
   send me  asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    */
    //start the send sequence, wait for 
    I2C2CONbits.SEN = 1;
    while(I2C2CONbits.SEN == 1); //wait for SEN flag to go low to start
    I2C2TRN = 0b01111100;        //sent the slave address with R/W = 0 for write
    while(I2C2STATbits.TRSTAT == 1); //wait for ack
    I2C2TRN = 0b00000000;        //control byte of all 0s for writes
    while(I2C2STATbits.TRSTAT == 1); //wait for ack
    I2C2TRN = Package;               //send the data
    while(I2C2STATbits.TRSTAT == 1); //watit for ack
    I2C2CONbits.PEN = 1;         //terminate the transmission and wait for confirmation
    while(I2C2CONbits.PEN == 1);
    TMR1 = 0;                    
    T1CONbits.TON = 1;
    while(!_T1IF);
    _T1IF = 0;
    delay(1);   //1 ms delay
    /*
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
     * */
}
void lcd_setup(void){
    //this function sets up the LCD to be ready to be written to
    I2C2CONbits.I2CEN = 0;    //disable peripheal
    I2C2BRG = 157;            //set the baud rate to be 100 khz, IC2BRG = (100k/16mhz-16mhz/10^7)-1)
    I2C2CONbits.I2CEN = 1;    //enable peripheral
    delay(50);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00111001); // function set, extended instruction mode
    lcd_cmd(0b00010100); // interval osc
    lcd_cmd(0b01110000); // contrast C3-C0
    lcd_cmd(0b01011110); // Ion, Bon, C5-C4
    lcd_cmd(0b01101100); // follower control
    delay(200);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00001100); // Display On
    lcd_cmd(0b00000001); // Clear Display
    delay(2);
    
}
void lcd_printChar(char Package) {
    //prints a character to the display
    I2C2CONbits.SEN = 1; //Initiate Start condition
    while(I2C2CONbits.SEN == 1) // SEN will clear when Start Bit is complete
    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit (0 for write)
    while(I2C2STATbits.TRSTAT == 1);
    I2C2TRN = 0b01000000; // 8-bits consisting of control byte /w RS=1, CO = 0
    while(I2C2STATbits.TRSTAT == 1);
    I2C2TRN = Package; // 8-bits consisting of the data byte
    while(I2C2STATbits.TRSTAT == 1);
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN == 1); // PEN will clear when Stop bit is complete
}
void heartbeat(void){
    //heartbeat function for debugging purposes
    while(1){
        _RA0 = 1;
        delay(400);
        _RA0  = 0;
        delay(400);
    }   
}
void lcd_setCursor(char x, char y){
    unsigned char toWrite = 0x40 * y + x;    //see formula in lab manual
    toWrite += 0x80;                       //add leading one to make MSB be 1
    I2C2CONbits.SEN = 1;                     //Initiate Start condition
    while(I2C2CONbits.SEN == 1)              // SEN will clear when Start Bit is complete
    I2C2TRN = 0b01111100;                    // 8-bits consisting of the slave address and the R/nW bit
    while(I2C2STATbits.TRSTAT == 1);
    I2C2TRN = 0b00000000;                    // control byte CO = 0, RS = 0
    while(I2C2STATbits.TRSTAT == 1);
    I2C2TRN = toWrite;                       // 8-bits consisting of the data byte
    while(I2C2STATbits.TRSTAT == 1);
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN == 1); // PEN will clear when Stop bit is complete
}
void lcd_printStr(const char * s){
    //
    I2C2CONbits.SEN = 1; //Initiate Start condition
    while(I2C2CONbits.SEN == 1) // SEN will clear when Start Bit is complete
    I2C2TRN = 0b01111100;            // 8-bits consisting of the slave address and the R/nW bit
    while(I2C2STATbits.TRSTAT == 1); // SEN will clear when Start Bit is complete
    while(*(s+1) != '\0'){     //if character isn't null
        I2C2TRN = 0b11000000;           //continutous writes
        while(I2C2STATbits.TRSTAT == 1);
        I2C2TRN = *s;                   //send the character
        while(I2C2STATbits.TRSTAT == 1); 
        s = s+1;                        //increment the counter
    }
    I2C2TRN = 0b01000000;           //last write
    while(I2C2STATbits.TRSTAT == 1);
    I2C2TRN = *s; 
    while(I2C2STATbits.TRSTAT == 1); 
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN == 1); // PEN will clear when Stop bit is complete
    delay(200);
        
}