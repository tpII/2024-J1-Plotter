#include "arm_controller.h"
#include "web_server.h"
#include "scheduler.h"

void setup() 
{
  Serial.begin(115200);
  ARM_init();
  //WEB_SERVER_init();
  //SCHEDULER_init();

  Serial.println("Setup Complete");
}

void loop() 
{
  //Testing 
  //SERVO_test(); //Funcion Bloqueante para testear el funcionamiento de los tres servos
  //SERVO_calculate_range();

  //SCHEDULER_dispatch_tasks();

}
