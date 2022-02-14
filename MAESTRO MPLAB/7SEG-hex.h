#ifndef ADC7seg_H
#define	ADC7seg_H

#include <xc.h>                             // include processor files - each processor file is guarded.  

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------------------Variables-----------------------------------------------


//-------------------------Función para obtener valor de ADC-----------------------
int tabla_hex(int a);                       // Función de traducción a display de 7 segmentos
void divisor_hex(uint8_t a, char dig[]);    // Divisor para números hexadecimales
void divisor_dec(uint8_t b, char dig1[]);    // Divisor para números Decimales
    

#endif	/* ADC7seg_H */