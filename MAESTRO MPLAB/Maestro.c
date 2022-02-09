/*
 * File:   Maestro.c
 * Author: José Sanoli
 *
 * Created on 7 de febrero de 2022, 17:05
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
#include "LCD.h"
#include "Oscilador.h"
#include "7SEG-hex.h"

//-----------------Definición de frecuencia de cristal---------------
#define _XTAL_FREQ 4000000

//-----------------------Constantes----------------------------------

//-----------------------Variables------------------------------------
uint8_t val_ADC = 0;
char ADC_dig[];

//------------Funciones sin retorno de variables----------------------
void setup(void);                                   // Función de setup
void divisor_hex(uint8_t a, char dig[]);            // Función para dividir valores en dígitos y guardarlos en array


//-------------Funciones que retornan variables-----------------------

//----------------------Interrupciones--------------------------------
void __interrupt() isr(void){
    
    
}

//----------------------Main Loop--------------------------------
void main(void) {
    setup();
    while(1){
        //**********************************************************************
        // COMUNICACIÓN CON PRIMER ESCLAVO
        //**********************************************************************
        
        PORTCbits.RC2 = 0;                          // Se selecciona el esclavo 1
        __delay_ms(1);
        
        val_ADC = spiRead();                        // El valor del ADC traducido por el esclavo, es enviado al maestro
        divisor_hex(val_ADC, ADC_dig);              // Dividir por dígitos la variable de val_ADC para 
        
        
    }
}

//----------------------Subrutinas--------------------------------
void setup(void){
    
    //Configuración de entradas y salidas
    ANSEL = 0;                                      // Pines digitales
    ANSELH = 0;
    
    TRISA = 0;                                      // PORTA como salida
    TRISB = 0;                                      // PORTB como salida
    TRISD = 0;                                      // PORTD como salida
    TRISE = 0;                                      // PORTE como salida
    
    PORTA = 0;                                      // Limpiar PORTA
    PORTD = 0;                                      // Limpiar PORTD
    PORTE = 0;                                      // Limpiar PORTE
    PORTB = 0;                                      // Limpiar PORTB
    
    //Configuración de Pin de slave select
    TRISC2 = 0;                                     // RC2 como pin para seleccionar esclavo
    PORTCbits.RC2 = 1;                              // RC2 se activa para que el pin RC5 del esclavo 1 lo niegue y esté deseleccionado el mismo
    
    //Configuración de oscilador
    initOsc(_4MHz);                                 // Oscilador a 8 mega hertz
    
    //Config de SPI (Configuración de maestro a 4 MHz, datos de entrada enviados a mitad de entrega de datos, polaridad en falling edge y clock rate en rising edge)
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
}

void divisor_hex(uint8_t a, char dig[]){
    for(int i = 0; i<3 ; i++){                      // De i = 0 hasta i = 2
        dig[i] = a % 16;                            // array[i] = cont_vol mod 16(retornar residuo). Devuelve digito por dígito de un número hexadecimal.
        a = (a - dig[i])/16;                        // b = valor sin último digito.
    }
}

