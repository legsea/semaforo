/*
	10/01/2019 08:53:08

	- Inicio Proyecto:
	
		- PIC16F886	(28/02/2019 9:14:19 - Para Nuevo Hardware)
		- Oscilador Interno 4Mhz

L�gica para los Pass Box:

Estado normal: 
Con el equipo energizado, la puertas estar�n enclavadas constantemente mediante
los electro imanes (luces y ventilador apagado).

El equipo tendr� 2 botones principales y 1 PARA TEST:

Bot�n verde: Cuando se lo presiona en el lado A, este libera la puerta de ese
lado, y cuando esta es cerrada (luego de ingresar el material), enciende el
ventilador por 30 segundos. Hay un bot�n de cada lado del equipo.

Bot�n rojo: Sin liberar las puertas, hace un ciclo de 2 min haciendo funcionar
el ventilador y la l�mpara UV. Hay un bot�n de cada lado del equipo.
 

La operatoria normal ser� la siguiente:


1.       Normalmente el equipo tendr� las dos puertas trabadas.

2.       Cuando arranca el d�a y cada cierta frecuencia, el responsable del
laboratorio oprimir� el bot�n rojo para que haga un ciclo de limpieza sin ning�n
material dentro (por eso es que nunca libera las puertas cuando se usa esta
opci�n).

3.       Para el pasaje de materiales del lado A al lado B, el operario oprime
el bot�n verde, se abre la puerta, mete el material y la cierra (activ�ndose
nuevamente el electroim�n). El passbox prender� el ventilador por 30 seg y
listo.

4.       Para sacar el material del otro lado, lado B, el 2do operador oprimir�
su bot�n verde. �ste le permitir� abrir la puerta, y cuando esta se cierre se
har� el ciclo de 30 seg de ventilador.

 

Luces:



Siempre que est� conectado a la red 220 volts estar� prendida la luz de POWER
ON.

Habr� 3 luces testigo:

Cuando no se est� pasando materiales estar�n todas apagadas (excepto Power ON).

Siempre se encienden y se apagan del lado donde.


1-      Se prende �Puerta habilitada� cuando presiono el bot�n verde. Se
enciende del lado del usuario que presion� el bot�n. Del otro lado se prende
�puerta deshabilitada�

2-      Se prende �Puerta Deshabilitada� cuando cierro dicha puerta y funciona
el ventilador. Se enciendo del lado A y se mantiene del lado B.

3-      De �Descontaminaci�n� cuando presiono el bot�n rojo. Se enciende de los
dos lados.

Pondremos un interruptor on/off en el techo del equipo, que libere las puertas y
haga funcionar el ventilador directamente. El equipo tiene un filtro, y esto es
para que puedan hacerle el test correspondiente de hermeticidad.

*/



#include <htc.h>
#include "16F886.h"


// Variables

unsigned char T20ms=0;
unsigned char T1Seg=0;
unsigned char c_BotA=0;
unsigned char c_BotB=0;
unsigned char c_BotRojo=0;
unsigned char c_SensorA,c_SensorB;
unsigned char PDel1,PDel0;			//Para el delay

unsigned char t_Vent;
unsigned char t_LedA;
unsigned char t_Leds;
unsigned char t_timeout;

unsigned char modo;

bit flag_intTimers=0;
bit flag_seg=0;
bit flag_leerentradas=0;
bit flag_NuevaSalida=0;
bit flag_PrimeraPasada=0;

bit flag_BotA=0;
bit flag_BotB=0;
bit flag_BotRojo=0;
bit flag_SensorA=0;
bit flag_SensorB=0;
bit flag_ReleA;
bit flag_ReleB;
bit flag_Ventilador;
bit flag_LedAE;
bit flag_LedAD;
bit flag_LedBD;
bit flag_LedBE;

bit flag_PPSA;
bit flag_PPSB;
bit flag_preopen;
bit flag_timeout;

void device_init(void)
{ 
// Configuracion puertos (1=entrada y 0=salida)

    PORTA = 0x00;
    TRISA = 0b00000000;


    PORTB = 0x00;
    TRISB = 0b00011111;
    nRBPU = 1;							//Pull ups 

// Puerto C
            
    PORTC = 0x00;
    TRISC = 0b00000111;





// Configuracion interrupciones globales (Todas!!!)
    GIE = 1;	// Interrupciones globales
    PEIE = 0;	// Interrupciones de perifericos    
    T0IE = 1;	// Interrupciones por Timer0
    INTE = 0;	// Interrupciones por flanco
    RBIE = 0;	// Interrupcion por RBIE
    T0IF = 0;
    INTF = 0;
    RBIF = 0; 	
     
		INTEDG=1;			// Interrupt on Rising edge

// Configuracion de Timer
// Timer0
		T0CS = 0;	// Contador en el pin T0CKI = 1; Reloj interno = 0;
		T0SE = 1;	// Transicion de bajo a alto = 0;
		PSA = 0;	// Pre escaler asignado a: 0=TMR0/1=WDT;
		PS2 = 1;
		PS1 = 1;	// Pre escaler 1:128
		PS0 = 0;

// Timer1
		T1CKPS1 = 1;	// Pre escaler 1:8
		T1CKPS0 = 1	;	// Pre escaler 1:8
		T1OSCEN = 0;	// 
		nT1SYNC = 1;		// 
		TMR1CS = 0;		// Timer0 como reloj interno
    TMR1IE = 0;		// Interrupciones por Timer1
    TMR1ON = 0;		// Timer 1 On/Off
    TMR1L=0x00;
		TMR1H=0x00;

// Timer2
    TMR2IE = 0;		// Interrupciones por Timer2
    TOUTPS3 = 0;
    TOUTPS2 = 0;
    TOUTPS1 = 0;	//Bits de Post escaler
    TOUTPS0 = 1;
    TMR2ON = 0;		//Enciendo el timer2
    T2CKPS1 = 1;
    T2CKPS0 = 1;	// Pre escaler
    TMR2   = 0;


// Modulo PWM

      CCP1CON=0x00;

// Deshabilito los comparadores/Conversor A/D
    ADCON1 = 0x07;

// Configuracion de CCP
    CCP1IE = 0;   // Interrupciones por CCP (desactivado)

 
}


void Delay_70us(void)
{

	#asm
	PLoop13: movlw     0x40      ; 1 set numero de repeticion  A
	        movwf     _PDel1     ; 1 |
	PLoop23: clrwdt              ; 1 clear watchdog
	        clrwdt              ; 1 ciclo delay
	        decfsz    _PDel1,f  ; 1 + (1) es el tiempo 0  ? (A)
	        goto      PLoop23    ; 2 no, loop
	PDelL13: goto PDelL23         ; 2 ciclos delay
	PDelL23: clrwdt
	#endasm

}


void Delay_500ms(void)
{
	#asm
  			 movlw     0xEF      ; 1 set numero de repeticion  B
         movwf     _PDel0    ; 1 |
PLoop11: movlw     0xE8      ; 1 set numero de repeticion  A
         movwf     _PDel1    ; 1 |
PLoop21: clrwdt              ; 1 clear watchdog
PDelL11: goto PDelL21        ; 2 ciclos delay
PDelL21: goto PDelL3         ; 2 ciclos delay
PDelL3:  clrwdt              ; 1 ciclo delay
         decfsz    _PDel1,f  ; 1 + (1) es el tiempo 0  ? (A)
         goto      PLoop21   ; 2 no, loop
         decfsz    _PDel0,f	 ; 1 + (1) es el tiempo 0  ? (B)
         goto      PLoop11   ; 2 no, loop
PDelL4:  goto PDelL5         ; 2 ciclos delay
PDelL5:  goto PDelL6         ; 2 ciclos delay
PDelL6:  goto PDelL7         ; 2 ciclos delay
PDelL7:  clrwdt              ; 1 ciclo delay
	#endasm
}


void lecturaentradas(void)
{
	flag_leerentradas=0;

	if(Bot_LadoA && !flag_BotA)
	{
		if(!flag_PrimeraPasada)
		{
			c_BotA=25;
			flag_PrimeraPasada=1;
		}

		c_BotA--;
	
		if(!c_BotA)
		{
			
			flag_PrimeraPasada=0;
			flag_BotA=1;
			flag_NuevaSalida=1;
		}	
		
	}	
	
	if(!Bot_LadoA && flag_BotA)
	{
		if(!flag_PrimeraPasada)
		{
			c_BotA=25;
			flag_PrimeraPasada=1;
		

		}

		c_BotA--;

		if(!c_BotA)
		{
			flag_PrimeraPasada=0;
			flag_BotA=0;
			flag_NuevaSalida=1;
		}			
	
	}	

	if(Bot_LadoB && !flag_BotB)
	{
		if(!flag_PrimeraPasada)
		{
			c_BotB=25;
			flag_PrimeraPasada=1;

		}

		c_BotB--;

		if(!c_BotB)
		{
			flag_PrimeraPasada=0;
			flag_BotB=1;
			flag_NuevaSalida=1;
		}	
		
	}	
	
	if(!Bot_LadoB && flag_BotB)
	{
		if(!flag_PrimeraPasada)
		{
			c_BotB=25;
			flag_PrimeraPasada=1;

		}

		c_BotB--;

		if(!c_BotB)
		{
			flag_PrimeraPasada=0;
			flag_BotB=0;
			flag_NuevaSalida=1;
		}			
	
	}	


	if(Bot_Rojo && !flag_BotRojo)
	{
		if(!flag_PrimeraPasada)
		{
			c_BotRojo=25;
			flag_PrimeraPasada=1;
			
		}

		c_BotRojo--;
	
		if(!c_BotRojo)
		{
			flag_PrimeraPasada=0;
			flag_BotRojo=1;
			flag_NuevaSalida=1;
		}	
		
	}	
	
	if(!Bot_Rojo && flag_BotRojo)
	{
		if(!flag_PrimeraPasada)
		{
			c_BotRojo=25;
			flag_PrimeraPasada=1;

		}

		c_BotRojo--;

		if(!c_BotRojo)
		{
			flag_PrimeraPasada=0;
			flag_BotRojo=0;
			flag_NuevaSalida=1;
		}			
	
	}	
	
	if(Sensor_A && !flag_SensorA)
	{
		if(!flag_PPSA)
		{
			c_SensorA=25;
			flag_PPSA=1;
			
		}

		c_SensorA--;
	
		if(!c_SensorA)
		{
			flag_PPSA=0;
			flag_SensorA=1;
			flag_NuevaSalida=1;
		}	
		
	}	
	
	if(!Sensor_A && flag_SensorA)
	{
		if(!flag_PPSA)
		{
			c_SensorA=25;
			flag_PPSA=1;

		}

		c_SensorA--;

		if(!c_SensorA)
		{
			flag_PPSA=0;
			flag_SensorA=0;
			flag_NuevaSalida=1;
		}			
	
	}		
	
	if(Sensor_B && !flag_SensorB)
	{
		if(!flag_PPSB)
		{
			c_SensorB=25;
			flag_PPSB=1;
			
		}

		c_SensorB--;
	
		if(!c_SensorB)
		{
			flag_PPSB=0;
			flag_SensorB=1;
			flag_NuevaSalida=1;
		}	
		
	}	
	
	if(!Sensor_B && flag_SensorB)
	{
		if(!flag_PPSB)
		{
			c_SensorB=25;
			flag_PPSB=1;

		}

		c_SensorB--;

		if(!c_SensorB)
		{
			flag_PPSB=0;
			flag_SensorB=0;
			flag_NuevaSalida=1;
		}			
	
	}		

	
}

void Activa_Salidas(void)
{
	flag_NuevaSalida=0;

	if(flag_ReleA)
		Rele_A=1;
	else
		Rele_A=0;
		
			
	if(flag_ReleB)
		Rele_B=1;
	else
		Rele_B=0;			
	
	
	if(flag_Ventilador)
	{
		Led_Vent=1;
		Rele_Vent=1;
	}	
	else
	{
		Led_Vent=0;
		Rele_Vent=0;
	}		
	

	if(flag_LedAE)
		LedA_Enable=1;
	else
		LedA_Enable=0;


	if(flag_LedAD)
		LedA_Dis=1;
	else
		LedA_Dis=0;	

	if(flag_LedBE)
		LedB_Enable=1;
	else
		LedB_Enable=0;


	if(flag_LedBD)
		LedB_Dis=1;
	else
		LedB_Dis=0;	

	
}


void Timers(void)			//Cada 20mseg
{
	flag_intTimers=0;
	
	flag_leerentradas=1;
	
	
// **** Cosas para hacer cada 1 Segundo dentro de esta Rutina	****	
	
	if(flag_seg)
	{
		flag_seg=0;
		
		if(t_Vent)
		{
			t_Vent--;
			
			if(!t_Vent)
			{
				flag_Ventilador=0;
				flag_LedAD=0;
				flag_LedBD=0;
				flag_NuevaSalida=1;				
				modo=Standby;
			}	
			
			
		}	

		if(t_LedA)
		{
			t_LedA--;
			
			if(!t_LedA)
			{
				flag_LedAE=0;
				flag_NuevaSalida=1;
			}	
			
			
		}			

		if(t_Leds)
		{
			t_Leds--;
			
			if(!t_Leds)
			{
				flag_LedAE=0;
				flag_LedAD=0;	
				flag_LedBE=0;
				flag_LedBD=0;
				flag_Ventilador=0;
				flag_NuevaSalida=1;
				modo=Startup;
			}	
			
			
		}			
		
		if(t_timeout && flag_timeout)
		{
			t_timeout--;
			
			if(!t_timeout)
			{
				flag_LedAE=0;
				flag_LedAD=0;	
				flag_LedBE=0;
				flag_LedBD=0;
				flag_ReleB=1;
				flag_ReleA=1;
				flag_Ventilador=0;
				flag_NuevaSalida=1;
				modo=Standby;				
			}		
		}	
		

	// **** Cosas para hacer cada 1 Minuto dentro de esta Rutina	****			
		if(T1Seg==60)
		{
					T1Seg=0;
					
		}	
		
		
	}			
}	
	

void interrupt interrupciones(void)	// Interrupciones
{

	if(T0IE && T0IF)			
	{
		T0IF=0;
		TMR0=99;						//Preescales:1:128 - Interrumpe cada 20mseg
				
		flag_intTimers=1;
		
		T20ms++;

		if(T20ms==50)
		{
			T20ms=0;
			
			flag_seg=1;					//Aviso que paso un segundo a la rutina de timers
			

		}			
				
	}	

	if(TMR1IF && TMR1IE)	// Control tiempo funcionamiento
	{
		TMR1IF=0;
	
		TMR1L=0xDC;
		TMR1H=0x0B;
	
	}


}


void main(void)
{
	device_init();
	
	
	flag_LedAE=1;
	flag_LedAD=1;	
	flag_LedBE=1;
	flag_LedBD=1;
	flag_Ventilador=1;
	t_Leds=2;
	
	flag_preopen=0;
	
	flag_NuevaSalida=1;
	
	modo=10;
	Delay_500ms();	
	
	while(1)
	{
		
		if(modo==Startup)	//Chequea que las puertas esten cerradas
		{

			
			if(flag_SensorA && flag_SensorB)	// Puertas A y B abiertas
			{
				flag_LedAD=1;
				flag_LedAE=1;
				flag_LedBD=1;
				flag_LedBE=1;
				flag_preopen=1;
			}	
			
			if(flag_SensorA && !flag_SensorB)	//Puerta A abierta
			{
				flag_LedAD=1;
				flag_LedAE=1;
				flag_LedBD=1;
				flag_LedBE=0;
				flag_preopen=1;
			}						
			
			if(!flag_SensorA &&  flag_SensorB)	// Puerta B Abierta
			{
				flag_LedAD=1;
				flag_LedAE=0;
				flag_LedBD=1;
				flag_LedBE=1;
				flag_preopen=1;
			}						

			if(!flag_SensorA &&  !flag_SensorB && flag_preopen)	// Se cerraron las puertas, ciclo de descontaminacion
			{
				flag_LedAD=1;
				flag_LedAE=0;
				flag_LedBD=1;
				flag_LedBE=0;
				
				flag_Ventilador=1;
				t_Vent=30;
				modo=Standby;
				
			}						

			if(!flag_SensorA &&  !flag_SensorB && !flag_preopen)	// Arranco con todo cerrado
				modo=Standby;
			
			flag_ReleA=1;
			flag_ReleB=1;
			flag_NuevaSalida=1;
			
			
		}	
		
		if(flag_BotRojo && modo==Standby)
		{
				flag_Ventilador=1;
				flag_LedAD=1;
				flag_LedBD=1;
				t_Vent=120;
				modo=Limpieza;		
				flag_NuevaSalida=1;	
		}	


		if(flag_BotA && modo==Standby)
		{
			flag_LedAE=1;
			flag_LedBD=1;
			flag_ReleA=0;
			modo=A2B_Inprogress;
			flag_NuevaSalida=1;
			flag_preopen=0;
			t_timeout=10;
			flag_timeout=1;
			
		}	

		if(flag_BotB && modo==Standby)
		{
			flag_LedBE=1;
			flag_LedAD=1;
			flag_ReleB=0;
			modo=B2A_Inprogress;
			flag_NuevaSalida=1;
			flag_preopen=0;
			t_timeout=10;
			flag_timeout=1;
		}	

		if(modo==A2B_Inprogress)
		{
			if(flag_SensorA && !flag_preopen)
			{
				Delay_500ms();
				flag_ReleA=1;
				flag_NuevaSalida=1;
				flag_preopen=1;
				flag_timeout=0;
			}			
		
			if(!flag_SensorA && flag_preopen)
			{
				modo=Descontaminacion;
				flag_preopen=0;
			}	
			
		
		}		
		
		if(modo==Descontaminacion)
		{
				flag_LedAD=1;
				flag_LedAE=0;
				flag_LedBD=1;
				flag_LedBE=0;
				
				flag_Ventilador=1;
				t_Vent=30;
				modo=Limpieza;
				flag_NuevaSalida=1;
		}	

		if(modo==B2A_Inprogress)
		{
			if(flag_SensorB && !flag_preopen)
			{
				Delay_500ms();
				flag_ReleB=1;
				flag_NuevaSalida=1;
				flag_preopen=1;
				flag_timeout=0;
				
			}			
		
			if(!flag_SensorB && flag_preopen)
			{
				modo=Descontaminacion;
				flag_preopen=0;
			}	
	
		}		
		
		
		if(flag_intTimers)			//Cada 20 mseg
			Timers();					

		if(flag_leerentradas)
			lecturaentradas();
		
		if(flag_NuevaSalida)
			Activa_Salidas();			

	}	


}