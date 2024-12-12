#include "headers/arm_controller.h"

static int current_pos_x = 0;
static int current_pos_y = 0;
static int target_pos_x = 0;
static int target_pos_y = 0;

static bool initialized_flag = false;
static bool currently_moving = false;

static bool instant_move_next = false; //Indica que el siguiente movimiento debe ser "instantaneo" (independiente del caminos)
static bool move_to_standby = false; //Indica que el siguiente movimiento debe ser a la posicion de standby (fuera del area de dibujo)

static bool check_valid_coords(int coord_x, int coord_y); //Chequea que las coordenadas se encuentren en el rango posible de graficar

void ARM_init()
{
  SERVO_init();

  if (check_valid_coords(STARTING_X, STARTING_Y))
  {
    //SERVO_moveto(STARTING_X + MIN_X, STARTING_Y + MIN_Y, true);
    current_pos_x = STARTING_X;
    current_pos_y = STARTING_Y;
    target_pos_x = STARTING_X;
    target_pos_y = STARTING_Y;
  }
  else
  {
    //SERVO_moveto(MIN_X, MAX_Y, true);
    current_pos_x = MIN_X;
    current_pos_y = MAX_Y;
    target_pos_x = MIN_X;
    target_pos_y = MAX_Y;
  }
  initialized_flag = true; 
}

//Chequea que las coordenadas se encuentren en el rango posible de graficar
static bool check_valid_coords(int coord_x, int coord_y)
{
  if ((coord_x >= 0 && coord_x <= RANGE_X) && (coord_y >= 0) && (coord_y <= RANGE_Y))
    return true;
  else 
    return false;
}

bool ARM_is_busy() //Devuelve true si el brazo no esta listo para recibir mas instrucciones
{
  return (currently_moving);
}

// Mueve el brazo hacia las coordenadas recibidas (Devuelve 'false' si aun no puede procesar la instruccion)
bool ARM_move_to(int target_x, int target_y, bool instant_move)
{
  //Solo acepta la siguiente linea si no se esta moviendo
  if (!currently_moving)
  {
    //Solo se mueve si las coordenadas de destino se encuentran dentro del area de dibujo
    if (check_valid_coords(target_x, target_y))
    {
      if (target_pos_x == target_x && target_pos_y == target_y) //Si ya esta en esa posicion, devuelve 'true'
        return true;
      else
      {
        target_pos_x = target_x;
        target_pos_y = target_y;
        currently_moving = true;
        instant_move_next = instant_move;
      }
    }
    return true;
  }
  else
    return false; //Devuelve 'false' si aun no puede procesar la instruccion
}

void ARM_standby_position()
{
  if (!currently_moving)
  {
    current_pos_x = MIN_X-1;
    current_pos_y = MAX_Y+1;
    target_pos_x = MIN_X-1;
    target_pos_y = MAX_Y+1;
    
    currently_moving = true;
    move_to_standby = true;
  }
}

void ARM_lift(bool lift) //Controla la posicion vertical del brazo
{
  if (initialized_flag)
  {
    SERVO_lift(lift);
  }
}

bool ARM_is_lifted() //Devuelve true si el brazo se encuentra elevado
{
  return SERVO_is_lifted();
}

//Desplaza el brazo una cierta cantidad de unidades en (x,y)
//Utilizado para el controlador manual de movimiento (joystick / botones)
void ARM_shift_by(int shift_x, int shift_y)
{
  if (!currently_moving)
  {
    //Solo se mueve si las coordenadas de destino se encuentran dentro del area de dibujo
    if (check_valid_coords(current_pos_x + shift_x, current_pos_y + shift_y))
    {
      target_pos_x = current_pos_x + shift_x;
      target_pos_y = current_pos_y + shift_y;

      currently_moving = true;
    }
  } 
}

//Actualiza la posicion del brazo si su posicion actual no es la deseada, y espera a que se muevan los servos.
void ARM_update()
{
  if (initialized_flag)
  {
    if (!SERVO_waiting()) //Si el servo puede recibir instrucciones
    {
      if (move_to_standby)
      {
        SERVO_standby_position();
        currently_moving = false;
        move_to_standby = false;
        return;
      }

      if ((current_pos_x != target_pos_x) || (current_pos_y != target_pos_y))
      {
        SERVO_moveto(target_pos_x + MIN_X, target_pos_y + MIN_Y, instant_move_next);
        current_pos_x = target_pos_x;
        current_pos_y = target_pos_y;

        currently_moving = true;
        instant_move_next = false; //resetea el modo instantaneo
      }
      else
      {
        currently_moving = false; //Deja de moverse cuando llega a su destino
      }
    }
  }
}
