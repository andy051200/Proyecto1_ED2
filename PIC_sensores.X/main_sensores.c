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
#include "ADC_CONFIG.h"
/*-----------------------------------------------------------------------------
 ----------------------- VARIABLES A IMPLEMTENTAR------------------------------
 -----------------------------------------------------------------------------*/

//-------DIRECTIVAS DEL COMPILADOR
#define _XTAL_FREQ 8000000

//-------VARIABLES DE PROGRAMA
unsigned char antirrebote;
unsigned char infrarrojo1, infrarrojo2, infrarrojo3, suma_ir;
unsigned char conversion1, conversion_total, temperatura_aprox;
/*-----------------------------------------------------------------------------
 ------------------------ PROTOTIPOS DE FUNCIONES ------------------------------
 -----------------------------------------------------------------------------*/
void setup(void);
void infrarrojos(void);
void toggle_adc(void);
void temperatura(void);

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
        //-------FUNCION PARA CONVERSION DEL ADC
        toggle_adc();
        //-------FUNCION PARA CONTROL DE SENSORES INFRARROJOS
        infrarrojos();
        //-------prueba de sensor de temperatura
        temperatura();
        PORTC=temperatura_aprox;
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
    ANSELbits.ANS0=1;                   //entrada analogica de sensor temperatura
    //-------CONFIGURACION IN/OUT
    TRISAbits.TRISA0=1;                 //entrada de sensor temperatura
    TRISBbits.TRISB1=1;                 //entrada de sensor IR1
    TRISBbits.TRISB2=1;                 //entrada de sensor IR2
    TRISBbits.TRISB3=1;                 //entrada de sensor IR3
    TRISEbits.TRISE0=0;                 //salida para led verde
    TRISEbits.TRISE1=0;                 //salida para led rojo
    TRISC=0;
    TRISD=0;
    //-------LIMPIEZA DE PUERTOS
    PORTB=0;
    PORTD=0;
    PORTE=0;
    //-------CONFIGURACION DE RELOJ A 4MHz
    osc_config(8);
    //-------CONFIGURACION DE ADC
    ADC_config();
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
    if (PORTBbits.RB1==1)
    {
        PORTDbits.RD0=1;
        PORTDbits.RD1=0;
        infrarrojo1=1;
    }
    if (PORTBbits.RB1==0)
    {
        PORTDbits.RD0=0;
        PORTDbits.RD1=1;
        infrarrojo1=0;
    }
    //-------PARQUEO2
    if (PORTBbits.RB2==1)
    {
        PORTDbits.RD2=1;
        PORTDbits.RD3=0;
        infrarrojo2=1;
    }
    if (PORTBbits.RB2==0)
    {
        PORTDbits.RD2=0;
        PORTDbits.RD3=1;
        infrarrojo2=0;
    }
    //-------PARQUEO3
    if (PORTBbits.RB3==1)
    {
        PORTDbits.RD4=1;
        PORTDbits.RD5=0;
        infrarrojo3=1;
    }
    if (PORTBbits.RB3==0)
    {
        PORTDbits.RD4=0;
        PORTDbits.RD5=1;
        infrarrojo3=0;
    }
    suma_ir=infrarrojo1+infrarrojo2+infrarrojo3;
    switch(suma_ir)
    {
        default:
            PORTC=0;
            break;
        case(0):
            PORTC=0;
            break;
        case(1):
            PORTC=suma_ir;
            break;
        case(2):
            PORTC=suma_ir;
            break;
        case(3):
            PORTC=suma_ir;
            break;
    }
}
//-------FUNCION PARA CONVERSION DEL ADC
void toggle_adc(void)
{
    if (ADCON0bits.GO==0)
    {
        conversion1=ADRESH<<8;                  //toma los MSB del ADRE
        conversion_total=conversion1+ADRESL;    //le suma los LSB
        __delay_ms(1);                          //tiempo de carga
        ADCON0bits.GO=1;
    }
}

//-------FUNCION PARA MAPEO DE TEMPERATURA  
void temperatura(void)
{
    if (conversion_total>0 && conversion_total <2.046)
        temperatura_aprox=0;
    else if (conversion_total>2.046 && conversion_total<4.092)
        temperatura_aprox=1;
    else if (conversion_total>4.092 && conversion_total <6.138)
        temperatura_aprox=2;
    else if (conversion_total>6.138 && conversion_total<8.184)
        temperatura_aprox=3;
    else if (conversion_total>8.184 && conversion_total<10.23)
        temperatura_aprox=4;
    else if (conversion_total>10.23 && conversion_total<12.276)
        temperatura_aprox=5;
    else if (conversion_total>12.276 && conversion_total<14.322)
        temperatura_aprox=6;
    else if (conversion_total>14.322 && conversion_total<16.368)
        temperatura_aprox=7;
    else if (conversion_total>16.368 && conversion_total<18.414)
        temperatura_aprox=8;
    else if (conversion_total>18.414 && conversion_total<20.46)
        temperatura_aprox=9;
    else if (conversion_total>20.46 && conversion_total<22.506)
        temperatura_aprox=10;
    else if (conversion_total>22.506 && conversion_total<24.552)
        temperatura_aprox=11;
    else if (conversion_total>24.552 && conversion_total<26.598)
        temperatura_aprox=12;
    else if (conversion_total>26.598 && conversion_total<28.64)
        temperatura_aprox=13;
    else if (conversion_total>28.64 && conversion_total<30.69)
        temperatura_aprox=14;
    else if (conversion_total>30.69 && conversion_total<32.73)
        temperatura_aprox=15;
    else if (conversion_total>32.73 && conversion_total<34.78)
        temperatura_aprox=16;
    else if (conversion_total>34.782 && conversion_total<36.82)
        temperatura_aprox=17;
    else if (conversion_total<36.82 && conversion_total<38.87)
        temperatura_aprox=18;
    else if (conversion_total>38.87 && conversion_total<40.92)
        temperatura_aprox=19;
    else if (conversion_total>40.92 && conversion_total<42.96)
        temperatura_aprox=20;
    else if (conversion_total>42.96 && conversion_total<45.01)
        temperatura_aprox=21;
    else if (conversion_total>45.01 && conversion_total<47.05)
        temperatura_aprox=22;
    else if (conversion_total>47.05 && conversion_total<49.10)
        temperatura_aprox=23;
    else if (conversion_total>49.10 && conversion_total<51.15)
        temperatura_aprox=24;
    else if (conversion_total>51.15 && conversion_total<53.19)
        temperatura_aprox=25;
    else if (conversion_total>53.19 && conversion_total<55.24)
        temperatura_aprox=26;
    else if (conversion_total>55.24 && conversion_total<57.28)
        temperatura_aprox=27;
    else if (conversion_total>57.28 && conversion_total<59.38)
        temperatura_aprox=28;
    else if (conversion_total>59.33 && conversion_total<61.38)
        temperatura_aprox=29;
    else if (conversion_total>61.38 && conversion_total<63.42)
        temperatura_aprox=30;
    else if (conversion_total>63.42 && conversion_total<65.47)
        temperatura_aprox=31;
    else if (conversion_total>65.47 && conversion_total<67.51)
        temperatura_aprox=32;
    else if (conversion_total>67.51 && conversion_total<69.56)
        temperatura_aprox=33;
    else if (conversion_total>69.56 && conversion_total<71.61)
        temperatura_aprox=34;
    else if (conversion_total>71.61 && conversion_total<73.65)
        temperatura_aprox=35;
    else if (conversion_total>73.65 && conversion_total<75.70)
        temperatura_aprox=36;
    else if (conversion_total>75.70 && conversion_total<77.74)
        temperatura_aprox=37;
    else if (conversion_total>77.74 && conversion_total<79.79)
        temperatura_aprox=38;
    else if (conversion_total>79.79 && conversion_total<81.84)
        temperatura_aprox=39;
    else if (conversion_total>81.84 && conversion_total<83.88)
        temperatura_aprox=40;
    else if (conversion_total>83.88 && conversion_total<85.93)
        temperatura_aprox=41;
    else if (conversion_total>87.97 && conversion_total<90.02)
        temperatura_aprox=42;
    else if (conversion_total>90.02 && conversion_total<92.07) 
        temperatura_aprox=43;
    else if (conversion_total>92.07 && conversion_total<94.11)
        temperatura_aprox=44;
    else if (conversion_total>94.11 && conversion_total<96.16)
        temperatura_aprox=45;
    else if (conversion_total>96.16 && conversion_total<98.20)
        temperatura_aprox=46;
    else if (conversion_total>98.20 && conversion_total<100.25)
        temperatura_aprox=47;
    else if (conversion_total>100.25 && conversion_total<102.3)
        temperatura_aprox=48;
    else if (conversion_total>102.3 && conversion_total<104.94)
        temperatura_aprox=49;
    else 
        temperatura_aprox=50;
            
    //return temperatura_aprox;
}