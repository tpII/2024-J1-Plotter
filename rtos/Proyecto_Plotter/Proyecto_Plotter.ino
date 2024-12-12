#include "headers/arm_controller.h"
#include "headers/drawing_module.h"
#include "headers/MQTT.h"
#include "headers/joystick_controller.h"
#include "headers/scheduler.h"
#include "headers/FSM.h"

void setup() 
{
  Serial.begin(115200);

  MQTT_init();
  //JOYSTICK_init();

  DRAWING_MODULE_init();
  SCHEDULER_init();
  Serial.println("Setup Complete");
}

void loop() 
{
  SCHEDULER_dispatch_tasks();
}
