#ifndef _ARM_CONTROLLER_H_
#define _ARM_CONTROLLER_H_

// Controlador del brazo mecanico (controlador de movimiento)

#include "servo_controller.h"

// Coordenadas iniciales del brazo
#define STARTING_X 90
#define STARTING_Y 90

// Ciclos del scheduler que espera el brazo a que se terminen de mover los servos
#define WAIT_CYCLES 15

void ARM_init();
void ARM_update(); //Actualiza la posicion del brazo si su posicion actual no es la deseada, y espera a que se muevan los servos.

void ARM_shift_by(int shift_x, int shift_y); //Desplaza el brazo una cierta cantidad de unidades en (x,y)
bool ARM_line_to(int target_x, int target_y); //Mueve el brazo en linea recta hacia las coordenadas recibidas (Devuelve 'false' si aun no puede procesar la instruccion)


#endif