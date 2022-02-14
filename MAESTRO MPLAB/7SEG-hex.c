/*
 * File:   7SEG-hex.c
 * Author: José Sanoli
 *
 * Created on 7 de febrero de 2022, 18:42
 */


#include <xc.h>

#include "7SEG-hex.h"

int tabla_hex(int a){
    switch (a){                                     // Ingresar valor de "a" a switch case
        case 0:                                     // Si a = 0
            return 0b00111111;                      // devolver valor 0b00111111
            break;
        case 1:                                     // Si a = 1
            return 0b00000110;                      // devolver valor 0b00000110 
            break;
        case 2:                                     // Si a = 2
            return 0b01011011;                      // devolver valor 0b01011011
            break;
        case 3:                                     // Si a = 3
            return 0b01001111;                      // devolver valor 0b01001111
            break;
        case 4:                                     // Si a = 4
            return 0b01100110;                      // devolver valor 0b01100110
            break;
        case 5:                                     // Si a = 5
            return 0b01101101;                      // devolver valor 0b01101101
            break;
        case 6:                                     // Si a = 6
            return 0b01111101;                      // devolver valor 0b01111101
            break;
        case 7:                                     // Si a = 7
            return 0b00000111;                      // devolver valor 0b01111101
            break;
        case 8:                                     // Si a = 8
            return 0b01111111;                      // devolver valor 0b01111111
            break;
        case 9:                                     // Si a = 9
            return 0b01101111;                      // devolver valor 0b01101111
            break;
        case 10:                                    // Si a = 10 (letra A)
            return 0b01110111;                      // devolver valor 0b01110111
            break;
        case 11:                                    // Si a = 11 (letra B)
            return 0b01111100;                      // devolver valor 0b01111100
            break;
        case 12:                                    // Si a = 12 (letra C)
            return 0b00111001;                      // devolver valor 0b00111001
            break;
        case 13:                                    // Si a = 13 (letra D)
            return 0b01011110;                      // devolver valor 0b01011110
            break;
        case 14:                                    // Si a = 14 (letra E)
            return 0b01111001;                      // devolver valor 0b01111001
            break;
        case 15:                                    // Si a = 15 (letra F)
            return 0b01110001;                      // devolver valor 0b01110001
            break;
        default:
            break;
            
    }
}

void divisor_hex(uint8_t a, char dig[]){            // Divide un número para obtener sus dígitos hexadecimales
    for(int i = 0; i<3 ; i++){                      // De i = 0 hasta i = 2
        dig[i] = a % 16;                            // array[i] = cont_vol mod 16(retornar residuo). Devuelve digito por dígito de un número hexadecimal.
        a = (a - dig[i])/16;                        // a = valor sin último digito.
    }
}

void divisor_dec(uint8_t b, char dig1[]){            // Divide un número para obtener sus dígitos decimales
    for(int n = 0; n<3 ; n++){                      // De i = 0 hasta i = 2
        dig1[n] = b % 10;                            // array[i] = cont_vol mod 10(retornar residuo). Devuelve digito por dígito de un número decimal.
        b = (b - dig1[n])/10;                        // a = valor sin último digito.
    }
}
