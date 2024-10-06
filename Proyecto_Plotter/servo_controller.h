
#ifndef _SERVO_CONTROLLER_H_
#define _SERVO_CONTROLLER_H_

#include <ESP32Servo.h>

//ESP32 PWM Pins: 0-19, 21-23, 25-27, 32-39
#define SERVO_ALPHA_PIN 18
#define SERVO_BETA_PIN 19
#define SERVO_VERTICAL_PIN 23

#define SERVO_ALPHA_ID 0
#define SERVO_BETA_ID 1
#define SERVO_VERTICAL_ID 2

//Longitud de cada brazo
#define ARM_LENGTH_A 75 //Brazo conectado al servo alpha
#define ARM_LENGTH_B 60 //Brazo conectado al servo beta

void SERVO_init(); 
void SERVO_update();

void SERVO_test();

void SERVO_setAngle(int servoID, int angle);
void SERVO_moveto(int x_coord, int y_coord); //Calcula los angulos alfa y beta para posicionar el extremo en (X,Y)

#endif