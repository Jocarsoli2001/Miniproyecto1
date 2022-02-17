/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef __SPI_H
#define	__SPI_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic16f887.h>
    
typedef enum{
    SPI_MASTER_FOSC4    = 0b00000000,   // Master SPI con Clock = FOSC/4
    SPI_MASTER_FOSC16   = 0b00000001,   // Master SPI con Clock = FOSC/16
    SPI_MASTER_FOSC64   = 0b00000010,   // Master SPI con Clock = FOSC/64
    SPI_MASTER_TMR2     = 0b00000011,   // Master SPI con Clock = TMR2/2
    SPI_SLAVE_SS_EN     = 0b00000100,   // Slave SPI con Clock = SCK | SS pin = ON
    SPI_SLAVE_SS_DIS    = 0b00000101,   // Slave SPI con Clock = SCK | SS pin = OFF
    I2C_SLAVE_7BIT_AD   = 0b00000110,   // Slave I2C con dirección de 7 bits
    I2C_SLAVE_10BIT_AD  = 0b00000111,   // Slave I2C con dirección de 10 bits
    I2C_MASTER_FOSC     = 0b00001000    // Master I2C con Clock = FOSC/4(SSPADD+1)
}MSSP_Mode;


void InitMSSP(MSSP_Mode Modo);
void WriteMSSP(char Data);
char ReadMSSP();

#endif	/* SPI_H */

