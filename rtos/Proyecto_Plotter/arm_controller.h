#ifndef _ARM_CONTROLLER_H_
#define _ARM_CONTROLLER_H_

// Controlador del brazo mecanico (controlador de movimiento)
#include "servo_controller.h"

//Valores maximos y minimos de coordenadas que puede alcanzar el lapiz
//Usados para abstraer los limites del area de dibujo
#define MIN_X 375
#define MAX_X 1400
#define MIN_Y -725
#define MAX_Y 725
#define RANGE_Y (MAX_Y-MIN_Y)
#define RANGE_X (MAX_X-MIN_X)
  //Dimensiones del area de dibujo (X,Y): [1025, 1450] 

// Coordenadas iniciales del brazo
#define STARTING_X RANGE_X-MIN_X
#define STARTING_Y RANGE_Y

// Ciclos del scheduler que espera el brazo a que se terminen de mover los servos
#define WAIT_CYCLES 2

void ARM_init();
void ARM_update(); //Actualiza la posicion del brazo si su posicion actual no es la deseada, y espera a que se muevan los servos.

void ARM_shift_by(int shift_x, int shift_y); //Desplaza el brazo una cierta cantidad de unidades en (x,y)
bool ARM_line_to(int target_x, int target_y); //Mueve el brazo en linea recta hacia las coordenadas recibidas (Devuelve 'false' si aun no puede procesar la instruccion)


#endif