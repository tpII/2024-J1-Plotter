
#ifndef _JOYSTICK_CONTROLLER_H_
#define _JOYSTICK_CONTROLLER_H_

#include <Arduino.h>
#include "servo_controller.h"
#include "arm_controller.h"

//Pines a los que se conectan las entradas de X, Y, y el boton del Stick respectivamente
const int VRxPin = 2;  
const int VRyPin = 4;
const int SWPin = 26; 

void JOYSTICK_init();
void JOYSTICK_update();

#endif