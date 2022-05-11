#include "Header.h"

void controlTiempos(Placa_t *this)
{
    this->systemFlags.inteTimer0 = 0; // Limpio flag interrupcion

    this->tiempos.t500useg++; // Incremento tiempos

    if (this->tiempos.t500useg == 20) // Parasron 10mseg?
    {
        this->tiempos.t500useg = 0;

        this->systemFlags.leerEntradas = 1; // Levanto flag para leer entradas

        this->tiempos.t100mseg++; // para 100mseg de tiempo

        // A partir de aca la base de tiempos se establece en 100mseg
        if (this->tiempos.t100mseg == 10)
        {
            this->tiempos.t100mseg = 0; // Pasaron 100mseg

            if (this->tiempos.delay)
            {
                this->tiempos.delay--;
                if (!this->tiempos.delay)
                {
                    this->flags.delay = 0;
                }
            }

            if (this->tiempos.delayReloj)
            {
                this->tiempos.delayReloj--;

                if (this->tiempos.delayReloj == 0)
                {
                    relojRampa = 0;  // Apago Rele
                    relojPorton = 0; // Apago Rele
                }
            }
        }
    }
}

#define MASCARAPUERTOB 0X1F
void leerEntradasB(Placa_t *this)
{
    uint8_t auxiliarLectura;
    uint8_t diferencia;

    if (!this->systemFlags.primeraLecturaB)
    {
        this->systemFlags.primeraLecturaB = 1;            // Hago la primer lectura
        this->entradaB.entradas = PORTB & MASCARAPUERTOB; // Enmascaro estado entradas y guardo en.entradas
    }

    auxiliarLectura = PORTB & MASCARAPUERTOB;               // Enmascaro
    diferencia = this->entradaB.entradas - auxiliarLectura; // comparo con la primer muestra

    if (diferencia == 0) // Si dio cero, son iguales
    {
        this->tiempos.antirebote++;         // Incremento antirebote
        if (this->tiempos.antirebote == 50) // Pasaron como 300 mseg
        {
            this->tiempos.antirebote = 0;
            this->entradaB.entradas = auxiliarLectura;

            if (Optos)
                this->auxEntradaB.entradas = ~auxiliarLectura; // Transfiero lectura a los flags en entrada
            else
                this->auxEntradaB.entradas = auxiliarLectura; // Transfiero lectura a los flags en entrada
        }
    }
    else
    {
        auxiliarLectura = 0;
        diferencia = 0;
        this->systemFlags.primeraLecturaB = 0;
    }
}
