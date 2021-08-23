/* 
 * File:   lcd_pablo.h
 * Author: Andy Bonilla
 *
 * Created on 21 de agosto de 2021, 01:21 PM
 */

#ifndef LCD_PABLO_H
#define	LCD_PABLO_H
#ifndef _XTAL_FREQ 
#define _XTAL_FREQ 8000000
#endif
#ifndef EN
#define EN PORTEbits.RE0
#endif
#ifndef RS
#define RS PORTEbits.RE1
#endif
#ifndef RW
#define RW PORTEbits.RE2
#endif
#include <xc.h>

//LCD Functions Developed by electroSome


void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Clear(void);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init(void);
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);



#endif	/* LCD_PABLO_H */

