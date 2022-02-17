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
uint8_t uni_ADC = 0;
uint8_t dec_ADC = 0;
uint8_t cen_ADC = 0;
uint8_t num = 0;
uint8_t trad = 0;

//------------Funciones sin retorno de variables----------------------
void setup(void);                                   // Función de setup
uint8_t tabla_numASCII(uint8_t a);

//-------------Funciones que retornan variables-----------------------

//----------------------Interrupciones--------------------------------
void __interrupt() isr(void){
    
    
}

//----------------------Main Loop--------------------------------
void main(void) {
    setup();
    Iniciar_LCD();                                  // Se inicializa la LCD en 8 bits
    Limpiar_pantallaLCD();                          // 
    set_cursor(1,0);
    Escribir_stringLCD("Hola");
    set_cursor(2,2);
    Escribir_stringLCD("Jose Santizo");
    __delay_ms(5000);
    Limpiar_pantallaLCD();
    while(1){
        set_cursor(1,0);                            // Setear cursor a primera línea                           
        Escribir_stringLCD("S1:    S2:   S3:");     // Escribir menú en primera línea
        
        //**********************************************************************
        // COMUNICACIÓN CON PRIMER ESCLAVO
        //**********************************************************************
        
        PORTCbits.RC2 = 0;                          // Se selecciona el esclavo 1
        __delay_ms(1);
        
        spiWrite(1);
        val_ADC = spiRead();                        // El valor del ADC traducido por el esclavo, es enviado al maestro
        
        __delay_ms(1);
        PORTCbits.RC2 = 1;
//        
        divisor_dec(val_ADC, ADC_dig);              // Se divide en dígitos hexadecimales, el valor del ADC 
        
        uni_ADC = tabla_numASCII(ADC_dig[0]);       // Traducir dígito de unidades a caracter ASCII
        dec_ADC = tabla_numASCII(ADC_dig[1]);       // Traducir dígito de decenas a caracter ASCII
        cen_ADC = tabla_numASCII(ADC_dig[2]);       // Traducir dígito de centenas a caracter ASCII
        
        
        //**********************************************************************
        // COMUNICACIÓN CON SEGUNDO ESCLAVO
        //**********************************************************************
        
        
        //**********************************************************************
        // IMPRESIÓN DE VALORES A LCD
        //**********************************************************************
        
        set_cursor(2,0);                            // Setear cursor a segunda línea
        Escribir_caracterLCD(uni_ADC);              // Imprimir valor de unidades de número de ADC
        Escribir_caracterLCD(dec_ADC);              // Imprimir valor de decenas de número de ADC
        Escribir_caracterLCD(cen_ADC);              // Imprimir valor de centenas de número de ADC
//        
    }
}

//----------------------Subrutinas--------------------------------
void setup(void){
    
    //Configuración de entradas y salidas
    ANSEL = 0;                                      // Pines digitales
    ANSELH = 0;
    
    TRISA = 0;                                      // PORTA como salida
    TRISB = 0;                                      // PORTB como salida
    TRISC = 0;
    TRISD = 0;                                      // PORTD como salida
    TRISE = 0;                                      // PORTE como salida
    
    PORTA = 0;                                      // Limpiar PORTA
    PORTD = 0;                                      // Limpiar PORTD
    PORTC = 0;
    PORTE = 0;                                      // Limpiar PORTE
    PORTB = 0;                                      // Limpiar PORTB
    
    //Configuración de Pin de slave select
    TRISC2 = 0;                                     // RC2 como pin para seleccionar esclavo
    PORTCbits.RC2 = 1;                              // RC2 se activa para que el pin RC5 del esclavo 1 lo niegue y esté deseleccionado el mismo
    
    //Configuración de oscilador
    initOsc(_4MHz);                                 // Oscilador a 4 mega hertz
    
    //Config de SPI (Configuración de maestro a 4 MHz, datos de entrada enviados a mitad de entrega de datos, polaridad en falling edge y clock rate en rising edge)
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
}


uint8_t tabla_numASCII(uint8_t a){
    switch(a){
        case 0:                                     // Si a = 0
            return 48;                              // devolver valor 48 (0 en ASCII)
            break;
        case 1:                                     // Si a = 1
            return 49;                              // devolver valor 49 (1 en ASCII)
            break;
        case 2:                                     // Si a = 2
            return 50;                              // devolver valor 50 (2 en ASCII)
            break;
        case 3:                                     // Si a = 3
            return 51;                              // devolver valor 51 (3 en ASCII)
            break;
        case 4:                                     // Si a = 4
            return 52;                              // devolver valor 52 (4 en ASCII)
            break;
        case 5:                                     // Si a = 5
            return 53;                              // devolver valor 53 (5 en ASCII)
            break;
        case 6:                                     // Si a = 6
            return 54;                              // devolver valor 54 (6 en ASCII)
            break;
        case 7:                                     // Si a = 7
            return 55;                              // devolver valor 55 (7 en ASCII)
            break;
        case 8:                                     // Si a = 8
            return 56;                              // devolver valor 56 (8 en ASCII)
            break;
        case 9:                                     // Si a = 9
            return 57;                              // devolver valor 57 (9 en ASCII)
            break;
        default:
            break;
    }
}
