#include "scheduler.h"

Ticker msTicker;

// Flags utilizadas por el scheduler
volatile bool flag_LED = false;
volatile bool flag_arm = false;
volatile bool flag_wifi_server = false;

// Funcion del TICK
void onTick() 
{
  static uint8_t counter = 0;
  counter++;

  if (counter % 10 == 0) //cada 100ms
  {
    flag_arm = true;
    flag_wifi_server = true;
  }

  if (counter >= 100) //cada 1sec
  {
    flag_LED = true;
    counter = 0;
  }
}

void SCHEDULER_init()
{
  pinMode(2, OUTPUT);  //Debugging
  msTicker.attach(0.01, onTick);  // 10 ms
}

//Ejecuta funciones en funcion al estado de los flags
void SCHEDULER_dispatch_tasks() 
{
  if (flag_arm) 
  {
    ARM_update();
  } 

  if (flag_wifi_server)
  {
    WEB_SERVER_update();
  }

  if (flag_LED)    
  {
    //Usado para testear que el timer funciona correctamente
    if (digitalRead(2))
      digitalWrite (2,LOW); 
    else
      digitalWrite (2,HIGH); 
    flag_LED = 0;
  } 
     
}
