
#ifndef _SERVO_CONTROLLER_H_
#define _SERVO_CONTROLLER_H_

#include <ESP32Servo.h>

//ESP32 PWM Pins: 0-19, 21-23, 25-27, 32-39
#define SERVO_ALPHA_PIN 18
#define SERVO_BETA_PIN 19
#define SERVO_VERTICAL_PIN 23

//Longitud de cada brazo
#define ARM_LENGTH_A 90 //Brazo conectado al servo alpha
#define ARM_LENGTH_B 90 //Brazo conectado al servo beta

void SERVO_init(); 
void SERVO_moveto(int x_coord, int y_coord); //Calcula los angulos alfa y beta para posicionar el extremo en (X,Y)
void SERVO_lift(bool lifted); //Eleva o desciende el servo vertical

void SERVO_test(); //Funcion Bloqueante para testear el funcionamiento de los tres servos
void SERVO_calculate_range(); //Funcion usada para calcular e imprimir por consola los valores maximos y minimos de X para cada altura (rango de dibujo)

#endif