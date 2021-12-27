#include "Header.h"

void configDevice(void)
{
    configPorts();
    configInterrupts();
    configTimers();
    configADC();
    configCCP();
    configComparadores();
    configOscilador();
}

void configPorts()
{
    // Puerto A

    PORTA = 0x00;
    TRISA = 0b00000000;
    ANSEL = 0x00;
    ANSELH = 0x00;

    // Puerto B
    PORTB = 0x00;
    TRISB = 0b10011111;
    nRBPU = 1;
    if (Optos == 1)
        nRBPU = 0; //Pull ups: 1=Disable/0=Enable */

    WPUB = 0xFF;

    // Puerto C

    PORTC = 0x00;
    TRISC = 0b00000111;
}

void configInterrupts(void)
{
    GIE = 1;  // Interrupciones globales
    PEIE = 0; // Interrupciones de perifericos
    T0IE = 1; // Interrupciones por Timer0
    INTE = 0; // Interrupciones por flanco
    RBIE = 0; // Interrupcion por RBIE
    T0IF = 0;
    INTF = 0;
    RBIF = 0;

    INTEDG = 1; // Interrupt on Rising edge
}

void configCCP(void)
{
    // Configuracion de CCP
    CCP1IE = 0; // Interrupciones por CCP (desactivado)
    CCP2IE = 0;
    CCP1CON = 0x00;
    CCP2CON = 0x00;
}

void configTimers(void)
{
    // Configuracion de Timer
    // Timer0
    T0CS = 0; // Contador en el pin T0CKI = 1; Reloj interno = 0;
    T0SE = 1; // Transicion de bajo a alto = 0;
    PSA = 0;  // Pre escaler asignado a: 0=TMR0/1=WDT;
    PS2 = 0;
    PS1 = 0; // Pre escaler 1:4
    PS0 = 1;
    TMR0 = 5; //Para interrupciones cada 500useg@8Mhz // 255-5=250*4=1000*.5useg=500useg

    // Timer1
    T1CKPS1 = 1; // Pre escaler 1:8
    T1CKPS0 = 1; // Pre escaler 1:8
    T1OSCEN = 0; //
    nT1SYNC = 1; //
    TMR1CS = 0;  // Timer0 como reloj interno
    TMR1IE = 0;  // Interrupciones por Timer1
    TMR1ON = 0;  // Timer 1 On/Off
    TMR1L = 0x00;
    TMR1H = 0x00;

    // Timer2
    TMR2IE = 0; // Interrupciones por Timer2
    TOUTPS3 = 0;
    TOUTPS2 = 0;
    TOUTPS1 = 0; //Bits de Post escaler
    TOUTPS0 = 1;
    TMR2ON = 0; //Enciendo el timer2
    T2CKPS1 = 1;
    T2CKPS0 = 1; // Pre escaler
    TMR2 = 0;
}

void configComparadores(void)
{
    // Comparadores

    CM1CON0 = 0x00;
    CM2CON0 = 0x00;
}

void configOscilador(void)
{
    OSCCON = 0b01110000; //Pic trabajando con Osc. Interno a 8 Mhz
}

void configADC(void)
{
    // ADC

    ADCON0 = 0b10000000; //
    ADCON1 = 0b10000000;

    VCFG1 = 0;
    VCFG0 = 0;

    CHS0 = 0;
    CHS1 = 0;
    CHS2 = 0;
    CHS3 = 0;
}