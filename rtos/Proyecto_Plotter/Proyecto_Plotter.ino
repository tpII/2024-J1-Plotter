#include "arm_controller.h"
#include "drawing_module.h"
#include "web_server.h"
#include "MQTT.h"
#include "joystick_controller.h"
#include "scheduler.h"

void setup() 
{
  Serial.begin(115200);
  DRAWING_MODULE_init();
  WEB_SERVER_init();
  JOYSTICK_init();
  //MQTT_init();
  SCHEDULER_init();
  Serial.println("Setup Complete");
}

void loop() 
{
  SCHEDULER_dispatch_tasks();
}
