#ifndef _ARM_CONTROLLER_H_
#define _ARM_CONTROLLER_H_

// Controlador del brazo mecanico (controlador de movimiento)
#include "servo_controller.h"

//Valores maximos y minimos de coordenadas que puede alcanzar el lapiz
//Usados para abstraer los limites del area de dibujo
#define MIN_X 34
#define MAX_X 145
#define MIN_Y -70
#define MAX_Y 70
#define RANGE_Y (MAX_Y-MIN_Y)
#define RANGE_X (MAX_X-MIN_X)
  //Dimensiones del area de dibujo (X,Y): [111, 140] 

// Coordenadas iniciales del brazo
#define STARTING_X RANGE_X-MIN_X
#define STARTING_Y RANGE_Y

void ARM_init();
void ARM_update(); //Actualiza la posicion del brazo si su posicion actual no es la deseada, y espera a que se muevan los servos.

void ARM_standby_position(); //Desplaza el brazo a la posicion de Standby
void ARM_shift_by(int shift_x, int shift_y); //Desplaza el brazo una cierta cantidad de unidades en (x,y)
bool ARM_move_to(int target_x, int target_y, bool instant_move); //Mueve el brazo hacia las coordenadas recibidas (Devuelve 'false' si aun no puede procesar la instruccion)
void ARM_lift(bool lift); //Controla la posicion vertical del brazo
bool ARM_is_lifted(); //Devuelve true si el brazo se encuentra elevado
bool ARM_is_busy(); //Devuelve true si el brazo no esta listo para recibir mas instrucciones


#endif