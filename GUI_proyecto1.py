'''------------------------------------------------------------------------------
Autores: Pablo Herrarte y Andy Bonilla
Programa: proyecto 1
Creado: 21 de agosto de 2021    
Descripcion: un laboratoria bien fumado tbh pero chilero
intefaz gráfica para el proyecto de parqueo para hotwheels
-------------------------------------------------------------------------------'''

'''------------------------------------------------------------------------------
-------------------------IMPORTAR LIBRERIAS--------------------------------------
------------------------------------------------------------------------------'''
import builtins
import tkinter as tk            #se importa libreria de GUI
from tkinter import *
#import serial, time             #se importa libreria serial y cuenta de tiempo
from Adafruit_IO import Client, RequestError, Feed

'''------------------------------------------------------------------------------
-----------------------DEFINICION DE OBJETOS------------------------------------
------------------------------------------------------------------------------'''
root = Tk()                     #se le da nombre al objeto principal
root.counter = 0                #se declara una variables en el objeto

'''------------------------------------------------------------------------------
-----------------------DEFINICION DE PUERTO SERIAL-------------------------------
------------------------------------------------------------------------------'''
'''port1=serial.Serial()             #declarar puerto serial y braudeaje
port1.port='COM1'                 #se dice el puerto a usar
port1.baudrate = 9600             #set Baud rate to 9600
port1.bytesize = 8                # Number of data bits = 8
port1.parity   ='N'               # No parity
port1.stopbits = 1                # Number of Stop bits = 1
port1.open()                      #apertura del puerto serial '''                    
'''------------------------------------------------------------------------------
-------------------INTERCAMBIO DE DATOS CON ADAFRUIT-----------------------------
------------------------------------------------------------------------------'''
#---------INICIALIZACION DE COMUNICACION CON ADAFRUIT
ADAFRUIT_IO_USERNAME = "anbo_one"
ADAFRUIT_IO_KEY = "al"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)     #parametros

#---------SE MANDAN VALORES DE CANTIDAD DE PARQUEOS DESDE PIC PARA ADAFRUIT
digital_feed = aio.feeds('parqueos')
aio.send_data(digital_feed.key, 1)
digital_data = aio.receive(digital_feed.key)

#---------SE MANDA SI ESTA OCUPADO EL PARQUEO1
digital_feed = aio.feeds('parqueo1')
aio.send_data(digital_feed.key, 1)
digital_data = aio.receive(digital_feed.key)

#---------SE MANDA SI ESTA OCUPADO EL PARQUEO2
digital_feed = aio.feeds('parqueo2')
aio.send_data(digital_feed.key, 0)
digital_data = aio.receive(digital_feed.key)

#---------SE MANDA SI ESTA OCUPADO EL PARQUEO3
digital_feed = aio.feeds('parqueo3')
aio.send_data(digital_feed.key, 0)
digital_data = aio.receive(digital_feed.key)

'''------------------------------------------------------------------------------
-----------------------DEFINICION DE FUNCIONES-----------------------------------
------------------------------------------------------------------------------'''
def actualizar():
    #slider.set(uart_recibido)
    '''uart_recibido1 = port1.read_until(b',',4)
    uart_recibido2 = uart_recibido1.split(b',')'''
    digital_feed = aio.feeds('parqueos')
    aio.send_data(digital_feed.key, slider2.get())
    
    digital_feed = aio.feeds('temperatura')
    aio.send_data(digital_feed.key, slider1.get())
    #aio.send_data(digital_feed.key, int(uart_recibido2[0]))
    #print(int(uart_recibido2[0]))


'''------------------------------------------------------------------------------
----------------------------CUERPO DE INTERFAZ-----------------------------------
------------------------------------------------------------------------------'''
#---------TITULO
titulo=tk.Label(root,text = "GUI para Proyecto 1, Electrónica Digital 2") #texto como titulo de GUI
titulo.place(x=90, y=20)
subtitulo=tk.Label(root, text="Comunicacion con Adafruit")
subtitulo.place(x=115,y=50)
#---------TITULO DE VENTANA
root.title("Electronica Digital 2")                   #le pones titulo al objeto
root.minsize(400,300)                                           #le decis el tamaño a la ventana
#---------SLIDER CON VALORES DESDE ADAFRUIT
slider1=Scale(root, from_=0,to=50, )
slider1.pack()
slider1.place(x=130, y=110)

#---------SLIDER CON VALORES DESDE ADAFRUIT
slider2=Scale(root, from_=0,to=3, )
slider2.pack()
slider2.place(x=190, y=110)
#---------BOTON ACTUALIZADOR PARA ADAFRUIT
b3=Button(root, text='Actualizar', command=actualizar)
b3.place(x=160, y=250)

'''------------------------------------------------------------------------------
---------------------------------MAIN LOOP---------------------------------------
------------------------------------------------------------------------------'''
root.mainloop()
