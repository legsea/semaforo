#include "Header.h"

void initCPU(Placa_t *controlCPU)
{
    releRampa1 = 0;
    releRampa2 = 0;
    releRampa3 = 0;
    relojPorton = 0;
    relojRampa = 0;
    controlCPU->estado = STANBY;
}

void esperaAccion(Placa_t *controlCPU)
{
    if (controlCPU->entradaB.rampa1)
        controlCPU->estado = RAMPA1;

    if (controlCPU->entradaB.rampa2)
        controlCPU->estado = RAMPA2;

    if (controlCPU->entradaB.rampa3)
        controlCPU->estado = RAMPA3;

    if (controlCPU->entradaB.porton)
        controlCPU->estado = PORTON;
}

void rampaUno(Placa_t *controlCPU)
{
    releRampa1 = 1;
    relojRampa = 1;
    controlCPU->tiempos.delayReloj = 10;
    controlCPU->tiempos.delay = 20;
    controlCPU->flags.delay = 1;
    controlCPU->estado = CONTANDO;
}

void rampaDos(Placa_t *controlCPU)
{
    releRampa2 = 1;
    relojRampa = 1;
    controlCPU->tiempos.delayReloj = 10;
    controlCPU->tiempos.delay = 20;
    controlCPU->flags.delay = 1;
    controlCPU->estado = CONTANDO;
}

void rampaTres(Placa_t *controlCPU)
{
    releRampa3 = 1;
    relojRampa = 1;
    controlCPU->tiempos.delayReloj = 10;
    controlCPU->tiempos.delay = 20;
    controlCPU->flags.delay = 1;
    controlCPU->estado = CONTANDO;
}

void llamaPorton(Placa_t *controlCPU)
{
    relojPorton = 1;
    releRampa1 = 1;
    releRampa2 = 1;
    releRampa3 = 1;
    controlCPU->tiempos.delayReloj = 10;
    controlCPU->tiempos.delay = 20;
    controlCPU->flags.delay = 1;
    controlCPU->estado = CONTANDO;
}

void counting(Placa_t *controlCPU)
{
    if (controlCPU->flags.delay == 0)
    {
        if (controlCPU->entradaB.sincronismo == OFF)
        {
            releRampa1 = 0;
            releRampa2 = 0;
            releRampa3 = 0;
            relojPorton = 0;
            relojRampa = 0;
            controlCPU->estado = STANBY;
        }
    }
}