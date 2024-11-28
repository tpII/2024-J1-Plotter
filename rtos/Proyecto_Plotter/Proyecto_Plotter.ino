#include "arm_controller.h"
#include "web_server.h"
#include "joystick_controller.h"
#include "scheduler.h"

void setup() 
{
  Serial.begin(115200);
  ARM_init();
  //WEB_SERVER_init();
  SCHEDULER_init();
  JOYSTICK_init();
  Serial.println("Setup Complete");
}

void loop() 
{
  SCHEDULER_dispatch_tasks();
}
