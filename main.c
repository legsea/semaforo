/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief CPU Semaforo
 * @version 0.1
 * @date 2021-12-23
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "Header.h"

__CONFIG(0XEB04);
/* __CONFIG(FOSC_INTRC_NOCLKOUT &WDTE_OFF &PWRTE_ON &MCLRE_OFF &CP_ON &CPD_ON &BOREN_ON &IESO_OFF &FCMEN_ON &LVP_OFF);
__CONFIG(BOR4V_BOR40V &WRT_OFF);
 */

Placa_t controlCPU;

void main(void)
{

    configDevice();

    controlCPU.estado = INICIO;

    while (1)
    {

        if (controlCPU.systemFlags.inteTimer0)
            controlTiempos(&controlCPU);

        if (controlCPU.systemFlags.leerEntradas)
        {
            controlCPU.systemFlags.leerEntradas = 0;
            leerEntradasB(&controlCPU);
        }

        switch (controlCPU.estado)
        {
        case INICIO:
            initCPU(&controlCPU);
            break;

        case STANBY:
            esperaAccion(&controlCPU);
            break;

        case RAMPA1:
            rampaUno(&controlCPU);
            break;

        case RAMPA2:
            rampaDos(&controlCPU);
            break;

        case RAMPA3:
            rampaTres(&controlCPU);
            break;

        case PORTON:
            llamaPorton(&controlCPU);
            break;

        case CONTANDO:
            counting(&controlCPU);
            break;

        default:
            initCPU(&controlCPU);
            break;
        }
    }
}

void interrupt interrupciones(void)
{
    if (TMR0IE && TMR0IF)
    {
        TMR0IF = 0;
        controlCPU.systemFlags.inteTimer0 = 1;
        TMR0 = 5;
    }
}
