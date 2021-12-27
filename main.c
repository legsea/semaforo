/**
 * @file main.c
 * @author Ing. Martin Leg
 * @brief controlCPU
 * @version 0.1
 * @date 2021-11-17
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "Header.h"

__CONFIG(FOSC_INTRC_NOCLKOUT &WDTE_OFF &PWRTE_ON &MCLRE_OFF &CP_ON &CPD_ON &BOREN_ON &IESO_OFF &FCMEN_ON &LVP_OFF);
__CONFIG(BOR4V_BOR40V &WRT_OFF);

Placa_t controlCPU;

void interrupt interrupciones(void)
{
    if (TMR0IE && TMR0IF)
    {
        TMR0IF = 0;
        controlCPU.systemFlags.inteTimer0 = 1;
        TMR0 = 5;
    }
}

void main(void)
{

    configDevice();

    while (1)
    {

        if (controlCPU.systemFlags.inteTimer0)
        {
            controlTiempos(&controlCPU);
        }

        if (controlCPU.systemFlags.leerEntradas)
        {
            controlCPU.systemFlags.leerEntradas = 0;
            leerEntradasB(&controlCPU);
        }
    }
}
