#include "headers/drawing_module.h"

//Constantes usadas para determinar el estado del controlador
#define DRAWER_PRIMERA_LINEA -2
#define DRAWER_STANDBY -1
#define DRAWER_EMPTY -1

typedef struct 
{
    short startX; 
    short startY; 
    short endX;   
    short endY;   
} Line;

static Line lines[MAX_LINES];
static int final_index = DRAWER_EMPTY; //Indice de la ultima linea introducida
static int current_index = DRAWER_STANDBY; // '-1' si no esta dibujando, '-2' si comienza el dibujo

static bool flag_drawing = false; //Indica si esta dibujando

static void ARM_advance_line(int *curr_x, int *curr_y); //Avanza n unidades hacia el final de la linea dependiendo del progreso restante de cada eje

void DRAWING_MODULE_init()
{
  ARM_init();
}

bool DRAWING_MODULE_is_drawing() //Devuelve true si el modulo se encuentra dibujando (ocupado)
{
  return flag_drawing;
}

int DRAWING_MODULE_add_line(int startX, int startY, int endX, int endY) //Agrega la linea a la lista (coords de inicio y fin)
{
  //Si las coordenadas de inicio y fin se encuentran dentro del rango de dibujo
  if (startX >= 0 && startX <= RANGE_X &&
      endX >= 0 && endX <= RANGE_X &&
      startY >= 0 && startY <= RANGE_Y &&
      endY >= 0 && endY <= RANGE_Y)
  {
    
    if ((final_index+1) < MAX_LINES)
    {
      final_index++;
      lines[final_index].startX = startX;
      lines[final_index].endX = endX;
      lines[final_index].startY = startY;
      lines[final_index].endY = endY;
    }
  }
  return 0;
}

void DRAWING_MODULE_reset() //Elimina todas las lineas de la lista
{
  final_index = DRAWER_EMPTY;
  current_index = DRAWER_STANDBY;
}

void DRAWING_MODULE_start() //Comienza a dibujar desde la linea 0
{
  if (!flag_drawing && final_index >= 0)
  {
    flag_drawing = true;
    current_index = DRAWER_PRIMERA_LINEA;

    ARM_lift(true);
    ARM_move_to(lines[0].startX, lines[0].startY, true);
  }
}

void DRAWING_MODULE_stop() //Detiene el dibujo antes de terminarlo
{
  flag_drawing = false;
  current_index = DRAWER_STANDBY;
}

void DRAWING_MODULE_update() //Actualiza el estado del modulo (utilizado por el scheduler)
{
  static int current_x = 0;
  static int current_y = 0;

  if (flag_drawing && !ARM_is_busy())
  {
    if (current_index == DRAWER_PRIMERA_LINEA) //Indica que empieza el dibujo
    {
      current_index = 0;
      current_x = lines[current_index].startX;
      current_y = lines[current_index].startY;
    }
    else
    {
      //Si llega al final de la linea
      if (current_x == lines[current_index].endX && current_y == lines[current_index].endY)
      {
        current_index++;

        //Si llega al final del dibujo
        if (current_index > final_index) 
        {
          flag_drawing = false;
          ARM_lift(true);
          ARM_standby_position();
          return;
        }

        //Si la siguiente linea NO empieza donde termina la anterior, se levanta y se mueve
        if (!(lines[current_index-1].endX == lines[current_index].startX) ||
            !(lines[current_index-1].endY == lines[current_index].startY))
        {
          ARM_lift(true);
          current_x = lines[current_index].startX;
          current_y = lines[current_index].startY;
          ARM_move_to(current_x, current_y, true);           
        }
      }
      else //Si aun no llega al final
      {
        ARM_advance_line(&current_x, &current_y);
        ARM_move_to(current_x, current_y, false);
      }
      
    }
  }
}

//Avanza n unidades hacia el final de la linea dependiendo del progreso restante de cada eje
static void ARM_advance_line(int *curr_x, int *curr_y)
{
  double delta_x, delta_y, remaining_x, remaining_y;

  ARM_lift(false);
  
  delta_x = abs(lines[current_index].endX - lines[current_index].startX);
  delta_y = abs(lines[current_index].endY - lines[current_index].startY);
  if (delta_x != 0)
  {
    remaining_x = abs(lines[current_index].endX - (*curr_x))/delta_x;
  }
  else remaining_x = -1;

  if (delta_y != 0)
  {
    remaining_y = abs(lines[current_index].endY - (*curr_y))/delta_y;
  }
  else remaining_y = -1;

  if (remaining_x > remaining_y)
  {
    if ((*curr_x) < lines[current_index].endX) (*curr_x)++;
    else if ((*curr_x) > lines[current_index].endX) (*curr_x)--;
  }
  else
  {
    if (remaining_x < remaining_y)
    {
      if ((*curr_y) < lines[current_index].endY) (*curr_y)++;
      else if ((*curr_y) > lines[current_index].endY) (*curr_y)--;
    }
    else
    {
      if ((*curr_x) < lines[current_index].endX) (*curr_x)++;
      else if ((*curr_x) > lines[current_index].endX) (*curr_x)--;

      if ((*curr_y) < lines[current_index].endY) (*curr_y)++;
      else if ((*curr_y) > lines[current_index].endY) (*curr_y)--;
    }
  }
}






