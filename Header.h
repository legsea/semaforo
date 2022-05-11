#ifndef _HEADER_H
#define _HEADER_H

#define Optos 0 // 1 esta con optos // 0 esta directa

#include <pic.h>
#include <stdint.h>
#include <pic16f886.h>
#include <stdio.h>

#include "config.h"
#include "16F886.h"

typedef union
{
    uint8_t genericFlags;
    struct
    {
        unsigned inteTimer0 : 1;
        unsigned leerEntradas : 1;
        unsigned primeraLecturaB : 1;
    };

} SystemFlags_t;

typedef union
{
    uint8_t allFlags;
    struct
    {
        unsigned delay : 1;
        unsigned delayReloj : 1;
    };

} Flags_t;

typedef union
{
    uint16_t twoBytes;
    struct
    {
        unsigned low : 8;
        unsigned high : 8;
    };

} Split_t;

typedef union
{
    uint8_t entradas;
    struct
    {
        unsigned rampa1 : 1; // LSB
        unsigned rampa2 : 1;
        unsigned porton : 1;
        unsigned sincronismo : 1;
        unsigned rampa3 : 1;
    };
} EntradaB_t;

typedef struct
{
    uint8_t t500useg;
    uint8_t t100mseg;
    uint8_t loco;
    uint8_t t1seg;
    uint8_t antirebote;
    uint8_t delay;
    uint8_t delayReloj;
} Timers_t;

typedef struct
{
    uint8_t estado;
    SystemFlags_t systemFlags;
    Timers_t tiempos;
    EntradaB_t entradaB;
    EntradaB_t auxEntradaB;
    Flags_t flags;
} Placa_t;

#define INICIO 10
#define STANBY 20
#define RAMPA1 30
#define RAMPA2 40
#define RAMPA3 45
#define PORTON 50
#define CONTANDO 60

#define OFF 1

#include "control.h"
#include "funciones.h"

#endif //  _HEADER_H
