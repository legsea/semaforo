// Definiciones----------

// PortA
#define releRampa1 RA0
#define releRampa2 RA1
#define relojPorton RA2
#define relojRampa RA3
#define releRampa3 RA4
#define NUA5 RA5
#define NUA6 RA6
#define NUA7 RA7

// PortB
#define switchRampa1 RB0     // In_1
#define switchRampa2 RB1     // In_2
#define switchPorton RB2     // In_3
#define señalSincronismo RB3 // In_4
#define switchRampa3 RB4     // In_5
#define NUB5 RB5
#define NUB6 RB6
#define NUB7 RB7

// Port C
#define NUC0 RC0
#define NUC1 RC1
#define NUC2 RC2
#define SCL RC3
#define SDA RC4
#define NUC5 RC5
#define TX RC6
#define RX RC7

// Macros

#define testbit(var, bit) ((var) & (1 << (bit)))
#define setbit(var, bit) ((var) |= (1 << (bit)))
#define clrbit(var, bit) ((var) &= ~(1 << bit))
