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
#include "UART.h"

//-----------------Definición de frecuencia de cristal---------------
#define _XTAL_FREQ 4000000

#define SS1 PORTCbits.RC2                           // Slave select 1 = pin RC2
#define SS2 PORTCbits.RC1                           // Slave select 2 = pin RC1
#define SS3 PORTCbits.RC0                           // Slave select 3 = pin RC0

//-----------------------Constantes----------------------------------

//-----------------------Variables------------------------------------
int val_ADC;
int Contador;
char Temp;

//USART
char i = 0;
int dato_escrito[];
const char msg[] = "S1:    S2:   S3:\n\r";
int n = sizeof(msg);
int direc = 0;
char data[];

// Banderas
int Bandera_s1 = 1;
int Bandera_s2 = 0;
int Bandera_s3 = 0;

// Variables para ADC
char ADC_dig[];
char dig_ADC = 0;
char uni_ADC = 0;
char dec_ADC = 0;
char cen_ADC = 0;

// Variables para Contador
char Cont_dig[];
char dig_Cont = 0;
char uni_Cont = 0;
char dec_Cont = 0;
char cen_Cont = 0;

// Variables para temperatura
char Temp_dig[];
char dig_Temp = 0;
char uni_Temp = 0;
char dec_Temp = 0;
char cen_Temp = 0;

//------------Funciones sin retorno de variables----------------------
void setup(void);                                   // Función de setup
char tabla_numASCII(char a);
void imprimir_cadena(void);
void TXREG_char (uint8_t *direccion,uint8_t uni1, uint8_t dec1, uint8_t cen1, 
                 uint8_t e1,uint8_t uni2, uint8_t dec2, uint8_t cen2, uint8_t e2, uint8_t uni3,
                 uint8_t dec3, uint8_t cen3, uint8_t e3, uint8_t *data);

//-------------Funciones que retornan variables-----------------------

//----------------------Interrupciones--------------------------------
void __interrupt() isr(void){
    
    
}

//----------------------Main Loop--------------------------------
void main(void) {
    setup();
    val_ADC = 0;                                        // Setear variable que guardará el valor del ADC en 0
    Contador = 0;                                       // Setear variable que guardará el valor del contador en 0
    Temp = 0;                                           // Setear variable que guardará el valor de la temperatura en 0
    SS1 = 1;
    SS2 = 1;
    SS3 = 1;
    
    while(1){
        set_cursor(1,0);                                // Setear cursor a primera línea                           
        Escribir_stringLCD("S1:    S2:   S3:");         // Escribir menú en primera línea
        
        //**********************************************************************
        // COMUNICACIÓN USART
        //**********************************************************************
        imprimir_cadena();
        
        //**********************************************************************
        // COMUNICACIÓN Y ESCRITURA DE VALORES DE PRIMER ESCLAVO EN LCD
        //**********************************************************************
        SS2 = 1;
        SS3 = 1;
        SS1 = 0; 
        // Se selecciona el esclavo 1
        __delay_us(5);

        WriteMSSP(1);

        //NOTA: El valor del SPI funciona solamente cuando se guarda en una variable tipo int.
        val_ADC = ReadMSSP();                           // El valor del ADC traducido por el esclavo, es enviado al maestro

        __delay_us(5);

        SS1 = 1;
        SS2 = 1;
        SS3 = 1;
            
        
        //**********************************************************************
        // COMUNICACIÓN CON SEGUNDO ESCLAVO
        //**********************************************************************
        SS1 = 1;
        SS2 = 0;
        SS3 = 1;

        __delay_us(5);

        WriteMSSP(1);

        //NOTA: El valor del SPI funciona solamente cuando se guarda en una variable tipo int.
        Contador = ReadMSSP();                           // El valor del ADC traducido por el esclavo, es enviado al maestro

        __delay_us(5);

        SS2 = 1;
        SS2 = 1;
        SS3 = 1;
        
        
        //**********************************************************************
        // COMUNICACIÓN CON TERCER ESCLAVO
        //**********************************************************************
        SS1 = 1;
        SS2 = 1;
        SS3 = 0;

        __delay_us(5);

        WriteMSSP(1);

        //NOTA: El valor del SPI funciona solamente cuando se guarda en una variable tipo int.
        Temp = ReadMSSP();                              // El valor del ADC traducido por el esclavo, es enviado al maestro

        __delay_us(5);

        SS2 = 1;
        SS2 = 1;
        SS3 = 1;

         
        //**********************************************************************
        // PREPARACIÓN DE DATOS PARA ESCRITURA EN LCD
        //**********************************************************************
        
        divisor_dec(Contador, Cont_dig);                // Se divide en dígitos decimales, el valor del Contador 
        
        uni_Cont = tabla_numASCII(Cont_dig[2]);         // Traducir dígito de unidades a caracter ASCII
        dec_Cont = tabla_numASCII(Cont_dig[1]);         // Traducir dígito de decenas a caracter ASCII
        cen_Cont = tabla_numASCII(Cont_dig[0]);         // Traducir dígito de centenas a caracter ASCII
        
        
        divisor_dec(val_ADC, ADC_dig);                  // Se divide en dígitos hexadecimales, el valor del ADC 
        
        uni_ADC = tabla_numASCII(ADC_dig[2]);           // Traducir dígito de unidades a caracter ASCII
        dec_ADC = tabla_numASCII(ADC_dig[1]);           // Traducir dígito de decenas a caracter ASCII
        cen_ADC = tabla_numASCII(ADC_dig[0]);           // Traducir dígito de centenas a caracter ASCII
        
        
        divisor_dec(Temp, Temp_dig);                    // Se divide en dígitos hexadecimales, el valor de la temperatura
        
        uni_Temp = tabla_numASCII(Temp_dig[2]);         // Traducir dígito de unidades a caracter ASCII
        dec_Temp = tabla_numASCII(Temp_dig[1]);         // Traducir dígito de decenas a caracter ASCII
        cen_Temp = tabla_numASCII(Temp_dig[0]);         // Traducir dígito de decenas a caracter ASCII
        
        
        
        //**********************************************************************
        // ESCRITURA EN LCD
        //**********************************************************************
        
        set_cursor(2,7);                                // Setear cursor a segunda línea
        Escribir_caracterLCD(uni_Cont);                 // Imprimir valor de unidades de número de ADC
        Escribir_caracterLCD(dec_Cont);                 // Imprimir valor de decenas de número de ADC
        Escribir_caracterLCD(cen_Cont);                 // Imprimir valor de centenas de número de ADC
        
        set_cursor(2,0);                                // Setear cursor a segunda línea
        Escribir_caracterLCD(uni_ADC);                  // Imprimir valor de unidades de número de ADC
        Escribir_caracterLCD(dec_ADC);                  // Imprimir valor de decenas de número de ADC
        Escribir_caracterLCD(cen_ADC);                  // Imprimir valor de centenas de número de ADC
        
        set_cursor(2,13);                               // Setear cursor a segunda línea
        Escribir_caracterLCD(uni_Temp);                 // Imprimir valor de unidades de número de ADC
        Escribir_caracterLCD(dec_Temp);                 // Imprimir valor de decenas de número de ADC
        Escribir_caracterLCD(cen_Temp);                 // Imprimir valor de decenas de número de ADC

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
    PORTC = 0;
    
    //Configuración de Pin de slave select
    TRISC2 = 0;                                     // RC2 como pin para seleccionar esclavo
    PORTCbits.RC2 = 1;                              // RC2 se activa para que el pin RC5 del esclavo 1 lo niegue y esté deseleccionado el mismo
    
    TRISC1 = 0;
    PORTCbits.RC1 = 1;
    
    TRISC0 = 0;
    PORTCbits.RC0 = 1;
    
    //Configuración de oscilador
    initOsc(_4MHz);                                 // Oscilador a 4 mega hertz
    
    //Config de SPI (Configuración de maestro a 4 MHz, datos de entrada enviados a mitad de entrega de datos, polaridad en falling edge y clock rate en rising edge)
    InitMSSP(SPI_MASTER_FOSC4);
    
    // Configuración LCD
    Iniciar_LCD();                                    // Se inicializa la LCD en 8 bits
    Limpiar_pantallaLCD();
    set_cursor(1,0);
    Escribir_stringLCD("Hola");
    set_cursor(2,2);
    Escribir_stringLCD("Jose Santizo");
    __delay_ms(5000);
    Limpiar_pantallaLCD();
    
    //Configuración de TX y RX
    Config_USART(9600,4);
    
}


char tabla_numASCII(char a){
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

void imprimir_cadena(void){
    if(PIR1bits.TXIF){               
        for (i = 0; i<= n; i++){                    // For de i = 0 hasta el largo de la variable msg
            TXREG = data;                           // TXREG = datos

            TXREG_char(direc,cen_ADC,dec_ADC,uni_ADC,"   ",cen_Cont,dec_Cont,uni_Cont,"   ",cen_Temp,dec_Temp,uni_Temp,"C",data);

            direc = direc++;
        }
    }
}

void TXREG_char (uint8_t *direccion,uint8_t uni1, uint8_t dec1, uint8_t cen1, 
                        uint8_t e1,uint8_t uni2, uint8_t dec2, uint8_t cen2, uint8_t e2, uint8_t uni3,
                        uint8_t dec3, uint8_t cen3, uint8_t e3, uint8_t *data){
    switch(*direccion){
        case 0:
            *data = msg;
            break;
        case 1:
            *data = uni1;
            break;
        case 2:
            *data = dec1;
            break;
        case 3:
            *data = cen1;
            break;
        case 4:
            *data = e1;
            break;
        case 5:
            *data = uni2;
            break;
        case 6:
            *data = dec2;
            break;
        case 7:
            *data = cen2;
            break;
        case 8: 
            *data = e2;
            break;
        case 9:
            *data = uni3;
            break;
        case 10: 
            *data = dec3;
            break;
        case 11:
            *data = cen3;
            break;
        case 12:
            *data = e3;
            break;
        default:
            *data = 13;
            break;
    }
}