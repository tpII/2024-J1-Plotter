
#ifndef _SERVO_CONTROLLER_H_
#define _SERVO_CONTROLLER_H_

#include <ESP32Servo.h>

//Longitud de cada brazo
#define ARM_LENGTH_A 90  //Brazo conectado al servo alpha
#define ARM_LENGTH_B 90 //Brazo conectado al servo beta

//ESP32 PWM Pins: 0-19, 21-23, 25-27, 32-39
#define SERVO_ALPHA_PIN 18
#define SERVO_BETA_PIN 19
#define SERVO_VERTICAL_PIN 23

#define LIFT_DELAY 40 //Ciclos de FMS que espera el brazo al subir/bajar antes de volver a moverse
#define LIFT_ANGLE_UP 140 //Angulo que debe moverse el servo vertical
#define LIFT_ANGLE_DOWN 0 //Angulo que debe moverse el servo vertical

void SERVO_init(); 
void SERVO_standby_position(); //Setea los servos en la posicion de Standby
void SERVO_moveto(int x_coord, int y_coord, bool instant_move); //Calcula los angulos alfa y beta para posicionar el extremo en (X,Y)
void SERVO_lift(bool lifted); //Eleva o desciende el servo vertical
bool SERVO_is_lifted(); //Indica si el servo vertical esta en posicion maxima
bool SERVO_waiting(); //Devuelve "false" si los servos pueden recibir instrucciones nuevas

void SERVO_test(); //Funcion Bloqueante para testear el funcionamiento de los tres servos
void SERVO_calculate_range(); //Funcion usada para calcular e imprimir por consola los valores maximos y minimos de X para cada altura (rango de dibujo)

void SERVO_update();

#endif