// Definiciones----------

//PortA
#define ReleUp			RA0
#define ReleDown    	RA1
#define ReleEmergencia  RA2
#define NUA3	        RA3
#define NUA4    		RA4			
#define NUA5    		RA5
#define NUA6            RA6
#define NUA7            RA7

// PortB
#define BotonPiso0		RB0	// In_1
#define BotonPiso1		RB1 // In_2
#define BotonPiso2	 	RB2 // In_3
#define Piso1  		    RB3 // In_4
#define Piso2			RB4	// In_5
#define NUB5			RB5
#define	NUB6			RB6
#define	NUB7			RB7

// Port C
#define	Piso3  		        RC0
#define BotonEmergencia		RC1
#define In_8				RC2
#define SCL					RC3
#define SDA					RC4
#define NUC5				RC5
#define TX					RC6
#define	RX					RC7

// Macros

#define testbit(var,bit) ((var)&(1<<(bit)))
#define setbit(var,bit)	 ((var)|=(1<<(bit)))
#define clrbit(var,bit)	 ((var)&=~(1<<bit)) 



