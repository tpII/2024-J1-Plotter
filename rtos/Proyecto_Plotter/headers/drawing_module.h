
#ifndef _DRAWING_MODULE_H_
#define _DRAWING_MODULE_H_

#include "arm_controller.h"

#define MAX_LINES 16000 //Cantidad maxima de lineas guardadas en el arreglo

void DRAWING_MODULE_init(); //Inicializa el modulo

bool DRAWING_MODULE_is_drawing(); //Devuelve true si el modulo se encuentra dibujando (ocupado)
int DRAWING_MODULE_add_line(int startX, int startY, int endX, int endY); //Agrega la linea a la lista (coords de inicio y fin)
void DRAWING_MODULE_reset(); //Elimina todas las lineas de la lista

void DRAWING_MODULE_start(); //Comienza a dibujar desde la linea '0'
void DRAWING_MODULE_stop(); //Detiene el dibujo antes de terminarlo

void DRAWING_MODULE_update(); //Actualiza el estado del modulo (utilizado por el scheduler)

#endif