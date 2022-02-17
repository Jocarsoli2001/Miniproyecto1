/*
 * File:   ESCLAVO2.c
 * Author: Jos� Sanoli
 *
 * Created on 15 de febrero de 2022, 23:06
 */


//---------------------Bits de configuraci�n-------------------------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


//-----------------Librer�as utilizadas en c�digo-------------------- 
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//---------------------Librear�as creadas por usuario--------------------
#include "SPI.h"
#include "Oscilador.h"

//-----------------Definici�n de frecuencia de cristal---------------
#define _XTAL_FREQ 4000000

//-----------------------Constantes----------------------------------

//-----------------------Variables------------------------------------
char read;

//------------Funciones sin retorno de variables----------------------
void setup(void);                                   // Funci�n de setup

//-------------Funciones que retornan variables-----------------------

//----------------------Interrupciones--------------------------------
//void __interrupt() isr(void){
//    
//}

//----------------------Main Loop--------------------------------
void main(void) {
    setup();
    read = 0;
    while(1){
        
        //**********************************************************************
        // CONTADOR EN PUERTO B
        //**********************************************************************
        
        
        //**********************************************************************
        // LECTURA DE MSPP
        //**********************************************************************
        read = ReadMSSP();                          // Read = Lectura de SPI
        WriteMSSP(PORTB);  
        
        
    }
}

//----------------------Subrutinas--------------------------------
void setup(void){
    
    //Configuraci�n de entradas y salidas
    ANSEL = 0;                                 // Pines digitales
    ANSELH = 0;
    
    TRISA = 0;                                 // PORTA como entradas anal�gicas
    TRISB = 0;                                      // PORTB como salida
    TRISD = 0;                                      // PORTD como salida
    TRISE = 0;                                      // PORTE como salida
    
    PORTD = 0;                                      // Limpiar PORTD
    PORTE = 0;                                      // Limpiar PORTE
    PORTB = 0;                                      // Limpiar PORTB
    
    //Configuraci�n de oscilador
    initOsc(_4MHz);                                 // Oscilador a 4 mega hertz
    
    //Config de SPI (Configuraci�n de esclavo activada, datos de entrada enviados a mitad de entrega de datos, polaridad en falling edge y clock rate en rising edge)
    InitMSSP(SPI_SLAVE_SS_EN);
    
}
