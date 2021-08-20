/*------------------------------------------------------------------------------
Archivo: mainsproject.s
Microcontrolador: PIC16F887
Autores: Pablo Herrarte y Andy Bonilla
Compilador: pic-as (v2.30), MPLABX v5.45
    
Programa: pic para sensores de proyecto 1 de Electronica Digital 2
Hardware: PIC16F887
    
Creado: 15 de agosto de 2021    
Descripcion: 
------------------------------------------------------------------------------*/

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT   //configuracion de oscilador interno
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

/*-----------------------------------------------------------------------------
 ----------------------------LIBRERIAS-----------------------------------------
 -----------------------------------------------------------------------------*/
#include <stdint.h>
#include <pic16f887.h>
#include <xc.h>
#include <proc/pic16f887.h>
#include "Osc_config.h"

/*-----------------------------------------------------------------------------
 ----------------------- VARIABLES A IMPLEMTENTAR------------------------------
 -----------------------------------------------------------------------------*/

//-------DIRECTIVAS DEL COMPILADOR
#define _XTAL_FREQ 4000000

//-------VARIABLES DE PROGRAMA
unsigned char antirrebote;
/*-----------------------------------------------------------------------------
 ------------------------ PROTOTIPOS DE FUNCIONES ------------------------------
 -----------------------------------------------------------------------------*/
void setup(void);
void infrarrojos(void);
/*-----------------------------------------------------------------------------
 --------------------------- INTERRUPCIONES -----------------------------------
 -----------------------------------------------------------------------------*/
void __interrupt() isr(void) //funcion de interrupciones
{
    //-------INTERRUPCION POR BOTONAZO
    /*if (INTCONbits.RBIF)
    {
        if (PORTB==0b11111110)
        {
            PORTEbits.RE0=0;
            antirrebote=1;
        }
        INTCONbits.RBIF=0;
    }*/
    //-------INTERRUPCION 
   
}
/*-----------------------------------------------------------------------------
 ----------------------------- MAIN LOOP --------------------------------------
 -----------------------------------------------------------------------------*/
void main(void)
{
   
    setup();
    
    while(1)
    {
        //-------FUNCION PARA CONTROL DE SENSORES INFRARROJOS
        infrarrojos();
        
    }
   
}
/*-----------------------------------------------------------------------------
 ---------------------------------- SET UP -----------------------------------
 -----------------------------------------------------------------------------*/
void setup(void)
{
    //-------CONFIGURACION ENTRADAS ANALOGICAS
    ANSEL=0;
    ANSELH=0;
    //-------CONFIGURACION IN/OUT
    TRISBbits.TRISB0=1;                 //entrada de sensor
    TRISEbits.TRISE0=0;                 //salida para led verde
    TRISEbits.TRISE1=0;                 //salida para led rojo
   
    //-------LIMPIEZA DE PUERTOS
    PORTB=0;
    PORTE=0;
    //-------CONFIGURACION DE RELOJ A 4MHz
    osc_config(4);
    //-------CONFIGURACION DE COMUNICACION I2C
        
    //-------CONFIGURACION DE INTERRUPCIONES
    
    
}
/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
//-------FUNCION PARA CONTROL DE SENSORES INFRARROJOS
void infrarrojos(void)
{
    //-------PARQUEO 1
    if (PORTBbits.RB0==1)
    {
        PORTEbits.RE0=1;
        PORTEbits.RE1=0;
    }
    else
    {
        PORTEbits.RE0=0;
        PORTEbits.RE1=1;
    }
    //-------PARQUEO2
    //-------PARQUEO3
}