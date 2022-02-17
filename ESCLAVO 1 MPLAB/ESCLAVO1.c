/*
 * File:   ESCLAVO1.c
 * Author: José Sanoli
 *
 * Created on 15 de febrero de 2022, 23:06
 */


//---------------------Bits de configuración-------------------------------
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


//-----------------Librerías utilizadas en código-------------------- 
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//---------------------Librearías creadas por usuario--------------------
#include "SPI.h"
#include "Oscilador.h"
#include "ADC.h"

//-----------------Definición de frecuencia de cristal---------------
#define _XTAL_FREQ 4000000

//-----------------------Constantes----------------------------------

//-----------------------Variables------------------------------------


//------------Funciones sin retorno de variables----------------------
void setup(void);                                   // Función de setup

//-------------Funciones que retornan variables-----------------------

//----------------------Interrupciones--------------------------------
void __interrupt() isr(void){
    if(SSPIF == 1){                                 // Interrupción de MSSP
        spiWrite(cont1);                            // Leer valor de ADRESH (cont1) y mandarlo por SPI
        SSPIF = 0;
    }
    if(PIR1bits.ADIF){                              // Interrupción de ADC
        ADC();                                      // Guarda valor de ADRESH en cont1 o cont2, dependiendo del canal seleccionado
        PIR1bits.ADIF = 0;                          // Apagar bandera de interrupción de ADC
    }
}

//----------------------Main Loop--------------------------------
void main(void) {
    setup();
    ADCON0bits.GO = 1;                             // Iniciar conversión de ADC
    while(1){
        //**********************************************************************
        // CONVERSIÓN DE POTENCIÓMETRO DE ADC (VER ADC.C)
        //**********************************************************************
        conversion();
        
    }
}

//----------------------Subrutinas--------------------------------
void setup(void){
    
    //Configuración de entradas y salidas
    ANSEL = 0b0001;                                 // Pines digitales
    ANSELH = 0;
    
    TRISA = 0b0001;                                 // PORTA como entradas analógicas
    TRISB = 0;                                      // PORTB como salida
    TRISD = 0;                                      // PORTD como salida
    TRISC = 0;
    TRISE = 0;                                      // PORTE como salida
    
    PORTD = 0;                                      // Limpiar PORTD
    PORTC = 0;
    PORTE = 0;                                      // Limpiar PORTE
    PORTB = 0;                                      // Limpiar PORTB
    
    //Habilitar slave select
    TRISAbits.TRISA5 = 1;                           // Slave Select
    
    //Configuración de oscilador
    initOsc(_4MHz);                                 // Oscilador a 4 mega hertz
    
    //Configuración de ADC para canal analógico seleccionado
    config_ADC(_CH0);
    
    //Config de SPI (Configuración de esclavo activada, datos de entrada enviados a mitad de entrega de datos, polaridad en falling edge y clock rate en rising edge)
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    //Configuración de interrupciones
    INTCONbits.GIE = 1;                             // Habilitamos interrupciones
    INTCONbits.PEIE = 1;                            // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;                             // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;                             // Habilitamos interrupción MSSP
    PIR1bits.ADIF = 0;                              // Limpiar bandera de interrupción del ADC
    PIE1bits.ADIE = 1;                              // Interrupción ADC = enabled
    
}

