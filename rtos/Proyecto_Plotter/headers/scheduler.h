#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <Arduino.h>
#include <Ticker.h>
#include "servo_controller.h"
#include "arm_controller.h"
#include "drawing_module.h"
#include "joystick_controller.h"
#include "web_server.h"
#include "MQTT.h"

void SCHEDULER_init();
void SCHEDULER_dispatch_tasks();

#endif