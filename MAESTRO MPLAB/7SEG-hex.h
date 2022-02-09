#ifndef ADC7seg_H
#define	ADC7seg_H

#include <xc.h>                         // include processor files - each processor file is guarded.  

#include <stdint.h>
#include <stdio.h>

//-------------------------Variables-----------------------------------------------


//-------------------------Función para obtener valor de ADC-----------------------
int tabla_hex(int a);                       // Función de traducción a display de 7 segmentos

#endif	/* ADC7seg_H */