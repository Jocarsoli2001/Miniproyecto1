/*
 * File:   SPI.c
 * Author: Jos� Sanoli
 *
 * Created on 7 de febrero de 2022, 17:07
 */

#include "SPI.h"

void InitMSSP(MSSP_Mode Modo){
    
    SSPSTAT = 0b00000000;
    SSPCON = 0b00000000;
    
    // Habilitar pines de transmisi�n
    // Modo I2C: 1 = Habilita los pines SDA y SCL para transmisi�n
    // Modo SPI: 1 = Habilita los pines SCK, SDO, SDI y SS para transmisi�n
    SSPCONbits.SSPEN = 0;                               // Se deshabilita brevemente para permitir "re-inicializaciones"
    //__delay_ms(10);
    SSPCONbits.SSPEN = 1;     
    
    // Configuraci�n de pines de transmisi�n
    TRISCbits.TRISC5 = 0;                               // Serial Data Out (SDO|RC5) = Output
    TRISCbits.TRISC4 = 1;                               // Serial Data In (SDI|RC4) = Input
    if (Modo < 0b00000110){                             // Si MSSP = SPI (Opciones para I2C son todas menores a 6)
        
        if (Modo < 0b00000100){                         // Si SPI = Master (Opciones para Master son todas menores a 4)
            TRISCbits.TRISC3 = 0;                       // Serial Clock (RC3) = Output
        }  
        else {                                          // Si SPI = Slave
            TRISCbits.TRISC3 = 1;                       // Serial Clock (RC3) = Input   
        }   
        
    }
    
    if (Modo == 0b00000100){ TRISAbits.TRISA5 = 1;}     // Si Slave Select = ON entonces SS (RA5) = Input

    // Modo de funcionamiento para m�dulo MSSP
    // Se hace un OR con la variable "Modo" para evitar sobrescribir datos de SSPCON
    SSPCON = SSPCON |  Modo;
    
    //PIE1bits.SSPIE = 0;                                 // Interrupciones por MSSP = OFF
    
    // Polaridad de reloj
    // NOTA: Efectos de configuraci�n var�an seg�n el modo (SPI o I2C)
    SSPCONbits.CKP = 0;                                 // Estado "Idle" del reloj = LOW
    
    // Configuraci�n de flanco de reloj
    SSPSTATbits.CKE = 0;                                // Dado que CKP = 0, los datos se transmiten luego de un flanco positivo 
    
    SSPSTATbits.SMP = 0;                                // Informaci�n sampleada al final del tiempo de salida
}

// ESCRITURA A BUS
void WriteMSSP(char Data){
    // Se escribe al registro SSPBUF
    // Si PIC = Master: Proceso inmediato
    // Si PIC = Slave: Proceso realizado hasta recibir flanco configurado
    SSPBUF = Data;                                                      
}

// LECTURA DE BUS
char ReadMSSP(){
    while(SSPSTATbits.BF == 0){}            // Programa espera mientras la bandera BF = 0 (Transmisi�n no completa)     
    return(SSPBUF);
}