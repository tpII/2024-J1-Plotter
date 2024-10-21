#include "arm_controller.h"

static int current_pos_x = 0;
static int current_pos_y = 0;
static int target_pos_x = 0;
static int target_pos_y = 0;

static bool currently_moving = false;

void ARM_init()
{
  SERVO_init();
  SERVO_moveto(STARTING_X, STARTING_Y);

  current_pos_x = STARTING_X;
  current_pos_y = STARTING_Y;
  target_pos_x = STARTING_X;
  target_pos_y = STARTING_Y;
}

// Mueve el brazo en linea recta hacia las coordenadas recibidas (Devuelve 'false' si aun no puede procesar la instruccion)
bool ARM_line_to(int target_x, int target_y)
{
  //Solo acepta la siguiente linea si no se esta moviendo
  if (!currently_moving)
  {
    target_pos_x = target_x;
    target_pos_y = target_y;

    currently_moving = true;
    return true;
  }
  else
    return false; //Devuelve 'false' si aun no puede procesar la instruccion
}

//Desplaza el brazo una cierta cantidad de unidades en (x,y)
//Utilizado para el controlador manual de movimiento (joystick / botones)
void ARM_shift_by(int shift_x, int shift_y)
{
  if (!currently_moving)
  {
    target_pos_x = current_pos_x + shift_x;
    target_pos_y = current_pos_y + shift_y;

    currently_moving = true;
  } 
}

//Actualiza la posicion del brazo si su posicion actual no es la deseada, y espera a que se muevan los servos.
void ARM_update()
{
  static int wait_timer = 0;
  int shift_x = 0;
  int shift_y = 0;

  if (wait_timer == 0)
  {
    if ((current_pos_x != target_pos_x) || (current_pos_y != target_pos_y))
    {
      //De momento se mueve directamente al destino final
      //Seria posible dividir el camino en multiples lineas intermedias para mejorar la presicion del dibujo
      SERVO_moveto(target_pos_x, target_pos_y);
      current_pos_x = target_pos_x;
      current_pos_y = target_pos_y;

      //Setea el tiempo de espera hasta que se terminen de mover los servos
      //La cantidad de ciclos que espera podria ser proporcional a la distancia que debe moverse
      wait_timer = WAIT_CYCLES;     
    }
    else
    {
      //Deja de moverse cuando llega a su destino
      currently_moving = false;
    }
  }
  else
  {
    //Si esta esperando, decrementa el tiempo de espera
    wait_timer--; 
  }
}
