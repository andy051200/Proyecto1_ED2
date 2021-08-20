/*------------------------------------------------------------------------------
Archivo: mainsproject.s
Microcontrolador: PIC16F887
Autor: Pablo Herrarte y Andy Bonilla
Compilador: pic-as (v2.30), MPLABX v5.40
    
Programa: pic para motores de proyecto 1 de Electronica Digital 2
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
#include "UART_CONFIG.h"

/*-----------------------------------------------------------------------------
 ----------------------- VARIABLES A IMPLEMTENTAR------------------------------
 -----------------------------------------------------------------------------*/

//-------DIRECTIVAS DEL COMPILADOR
#define _XTAL_FREQ 4000000
#define PinEcho PORTAbits.RA1   //Pin RA1 conectado al Pin Echo. (entrada digital)
#define PinTrig PORTAbits.RA0   //Pin RA0 conectado al Pin Trig. (salida digital)
//-------VARIABLES DE PROGRAMA
unsigned char antirrebote;
unsigned char botonazo;
unsigned int hc04;
unsigned char sensorOn;
unsigned int duracion;         
unsigned int distancia;
unsigned int T1H;
unsigned int T1L;
unsigned int distancia;       //Variable distancia.
unsigned int anterior;        //Variable anterios.
unsigned int dismax;          //Variable dis_max.
unsigned int dismin;          //Variable dis_min.
unsigned int talanquera;
/*-----------------------------------------------------------------------------
 ------------------------ PROTOTIPOS DE FUNCIONES ------------------------------
 -----------------------------------------------------------------------------*/
void setup(void);
void sensor_ultrasonico(void);
/*-----------------------------------------------------------------------------
 --------------------------- INTERRUPCIONES -----------------------------------
 -----------------------------------------------------------------------------*/
void __interrupt() isr(void) //funcion de interrupciones
{
    //-------INTERRUPCION POR BOTONAZO
    if (INTCONbits.RBIF)
    {
        if (PORTB==0b11111101)
            antirrebote=1;
        else
            antirrebote=0;
        INTCONbits.RBIF=0;
    }
    //-------INTERRUPCION POR TIMER0
    
    
}
/*-----------------------------------------------------------------------------
 ----------------------------- MAIN LOOP --------------------------------------
 -----------------------------------------------------------------------------*/
void main(void)
{
    setup();
    
    while(1)
    {
        //-------ANTIRREBOTE DE BOTON
        if (antirrebote==1 && PORTBbits.RB1==0  )
        {
            botonazo++;
            //PORTD=botonazo;
            antirrebote=0;
        }
        //-------FUNCIONAMIENTO DEL SENSOR
        switch(botonazo)
        {
            case(0):                //servo en posicion de 0°
                PORTDbits.RD0=1;
                __delay_ms(1);
                PORTDbits.RD0=0;
                break;
            case(1):                //servo en posicion 90°
                PORTDbits.RD0=1;
                __delay_ms(2);
                PORTDbits.RD0=0;
                break;
            case(2):
                botonazo=0;
                break;
        }
        
        //sensor_ultrasonico();
        //PORTCbits.RC1=talanquera;
        
        /*distancia=ObtenerDistancia();   //todo el timepo estará midiendo
        dismax=anterior+2;              //tomar un rango maximo 
        dismin=anterior-2;              //tomar un rango minimo
        
        if((distancia>=dismin)&&(distancia<=dismax))
        distancia=anterior;
        anterior=distancia;  */
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
    TRISDbits.TRISD0=0;
    TRISC=0;
    TRISBbits.TRISB0=1;                 //entrada boton prueba
    TRISBbits.TRISB1=1;                 //entrada boton prueba
    TRISEbits.TRISE0=0;                 //salida para PWM de servo
    TRISEbits.TRISE1=0;                 //salida para PWM de servo
   
    //-------LIMPIEZA DE PUERTOS
    PORTB=0;
    PORTC=0;
    PORTD=0;
    PORTE=0;
    //-------CONFIGURACION DE RELOJ A 4MHz
    osc_config(4);
    //-------CONFIGURACION DE TIMER0
    OPTION_REGbits.T0CS = 0;    //Uso reloj interno
    OPTION_REGbits.PSA = 0;     //Uso pre-escaler
    OPTION_REGbits.PS = 0b111;  //PS = 111 / 1:256
    TMR0 = 78;                  //Reinicio del timmer
    //-------CONFIGURACION DE WPUB
    OPTION_REGbits.nRBPU=0;             //se activan WPUB
    WPUBbits.WPUB1=1;                   //RB0, boton prueba
    //-------CONFIGURACION DE COMUNICACION UART
    uart_config();
    //-------CONFIGURACION DEL TIMER1
    T1CONbits.T1CKPS=0x00;      //Configuramos un Pre-escaler para el timer1 de 1
    T1CONbits.nT1SYNC=1;        //No sincronizado con la fuente de reloj externa.
    T1CONbits.TMR1CS=0;         //Configuramos el Reloj Interno 4/Fosc
    T1CONbits.TMR1ON=0;         //Inicialmente el Timer1 inicia deshabilitado.
        
    //-------CONFIGURACION DE INTERRUPCIONES
    INTCONbits.GIE=1;           //se habilita interrupciones globales
    INTCONbits.PEIE = 1;        //habilitan interrupciones por perifericos
    //INTCONbits.T0IE=1;          //se habilita interrupcion timer0
    //INTCONbits.T0IF=0;          //se baja bandera de timer0
    INTCONbits.RBIE=1;          //se  habilita IntOnChange B
    INTCONbits.RBIF=0;          //se  apaga bandera IntOnChange B
    IOCBbits.IOCB1=1;           //habilita IOCB RB0
}
/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
void sensor_ultrasonico(void)
{   
    PinTrig=1;                  //se da secuencia inicial de trigger
    __delay_us(10); 
    PinTrig=0;                  //off de secuencia inicial
    
    while(PinEcho==0);
    T1CONbits.TMR1ON=1;         //Activamos el Timer1 como temporizador.
    while(PinEcho==1);
    T1CONbits.TMR1ON=0;         //Deshabilitamos el Timer1 como temporizador.
    T1L=TMR1L;                  //Almacenamos los 8 bits menos significativos del Timer1 en T1L.
    T1H=TMR1H;                  //Almacenamos los 8 bits mas significativos del Timer1 en T1H.
    duracion=(T1L|(T1H<<8));    
    if (duracion > 232 && duracion<250)    //232/58 = 4 cm y mayor a 2cm
        talanquera=1;
    else                        
        talanquera=0;
    
    duracion=0;                 //se reunicia la variable
    TMR1L=0;                    //se reinicia el almacenamiento de timer1
    TMR1H=0;                    //se reinicia el almacenamiento de timer1
    
   /* 
    if(duracion<=23200)         //El sensor Ultrasónico llega como máximo a 4 metros 
    distancia=(duracion/58);    //23200/58 = 400cm
    else if (duracion<116)      //116/58 = 2 cm
    distancia=0;
    else distancia=0;
    duracion=0;
    TMR1L=0;
    TMR1H=0;*/
    //return distancia;
}