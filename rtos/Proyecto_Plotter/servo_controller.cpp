#include "headers/servo_controller.h"

#define SERVO_ALPHA_ID 0
#define SERVO_BETA_ID 1
#define SERVO_VERTICAL_ID 2

static Servo servo_alpha;
static Servo servo_beta;
static Servo servo_vertical;

//Constantes para evitar calculos repetitivos
static const float constant_1 = (ARM_LENGTH_A*ARM_LENGTH_A)+(ARM_LENGTH_B*ARM_LENGTH_B);
static const float constant_2 = (2*ARM_LENGTH_A*ARM_LENGTH_B);
static const float to_degrees = (180.0 / PI);

static int angle_vertical = 0;

static int current_alpha, current_beta = 0;
static int start_alpha, start_beta = 0;
static int target_alpha, target_beta = 0;
static int remaining_alpha, remaining_beta = 0;
static int delta_alpha, delta_beta = 0;

static bool instant_next = false;
static bool servo_waiting = false;
static int lift_timer = 0;  //Ciclos restante de espera al servo vertical

static void SERVO_setAngle(int servoID, int angle); //Setea el angulo del servo correspondiente 
static void SERVO_move_towards_target(); //Calcula el angulo con el mayor porcentaje de camino restante, y lo mueve

void SERVO_init()
{
  servo_alpha.attach(SERVO_ALPHA_PIN);
  servo_beta.attach(SERVO_BETA_PIN);
  servo_vertical.attach(SERVO_VERTICAL_PIN);
  delay(100);

  SERVO_setAngle(SERVO_VERTICAL_ID, LIFT_ANGLE_UP);
  SERVO_standby_position();
}

void SERVO_standby_position() //Setea los servos en la posicion de Standby
{
  target_alpha = 90;
  target_beta = 0;
  SERVO_setAngle(SERVO_ALPHA_ID, target_alpha); 
  SERVO_setAngle(SERVO_BETA_ID, target_beta);
}

//Setea el angulo del servo correspondiente 
static void SERVO_setAngle(int servoID, int angle)
{
  //Correcciones por software para mitigar imperfecciones en el ensamblaje del brazo
  const int correction_alpha = 8;
  const int correction_beta = 8;
  int aux = 0;

  if (angle < 0) angle = 0;
  else if (angle > 180) angle = 180;

  switch (servoID)
  {
    case SERVO_ALPHA_ID: 
      current_alpha = angle;
      aux = angle + correction_alpha;
      if (aux < 0) aux = 0;
      else if (aux > 180) aux = 180;
      servo_alpha.write(180-aux);
    break;

    case SERVO_BETA_ID: 
      current_beta = angle;
      aux = angle + correction_beta;
      if (aux < 0) aux = 0;
      else if (aux > 180) aux = 180;
      servo_beta.write(180-aux);
    break;

    case SERVO_VERTICAL_ID: 
      angle_vertical = angle;
      servo_vertical.write(angle_vertical);
    break;

    default:
    break;
  }
}

static void SERVO_move_towards_target() //Calcula el angulo con el mayor porcentaje de camino restante, y lo mueve
{
  delta_alpha = abs(target_alpha - start_alpha);
  delta_beta = abs(target_beta - start_beta);

  if (delta_alpha != 0)
  {
    remaining_alpha = abs(target_alpha - current_alpha)/delta_alpha;
  }
  else remaining_alpha = -1;
  if (delta_beta != 0)
  {
    remaining_beta = abs(target_beta - current_beta)/delta_beta;
  }
  else remaining_beta = -1;

  if (remaining_alpha > remaining_beta)
  {
    if (current_alpha < target_alpha) current_alpha++;
    else if (current_alpha > target_alpha) current_alpha--;
  }
  else
  {
    if (remaining_alpha < remaining_beta)
    {
      if (current_beta < target_beta) current_beta++;
      else if (current_beta > target_beta) current_beta--;
    }
    else
    {
      if (current_alpha < target_alpha) current_alpha++;
      else if (current_alpha > target_alpha) current_alpha--;

      if (current_beta < target_beta) current_beta++;
      else if (current_beta > target_beta) current_beta--;
    }
  }
}

void SERVO_update()
{
  if (lift_timer > 0) //Si esta esperando a que se acomode el servo vertical, decrementa la espera
  {
    lift_timer--;
  }
  else
  {
    if (servo_waiting)
    {
      if (instant_next) //Si el movimiento es instantaneo, salta directo al final
      {
        SERVO_setAngle(SERVO_ALPHA_ID, target_alpha); 
        SERVO_setAngle(SERVO_BETA_ID, target_beta);
        start_alpha = current_alpha;
        start_beta = current_beta;
        instant_next = false;
        lift_timer += LIFT_DELAY; //Agrega tiempo de espera para no bajar el brazo inmediatamente
      }
      else
      {
        //Si llegaron los angulos al destino
        if (current_alpha == target_alpha && current_beta == target_beta)
        {
          servo_waiting = false; //Termina la espera
        }
        else //Si falta movimiento de algun angulo
        {
          SERVO_move_towards_target();
          SERVO_setAngle(SERVO_ALPHA_ID, current_alpha); 
          SERVO_setAngle(SERVO_BETA_ID, current_beta); 
        }
      }
    }
  }
}

//Calcula los angulos alfa y beta para posicionar el extremo en (X,Y)
//Si el boolean "instant_move" es verdadero, mueve en un solo movimiento, sino, mueve de a pasos parciales
void SERVO_moveto(int x_coord, int y_coord, bool instant_move)
{
  int alpha_shift, beta_shift;

  //unsigned long time = micros(); //Tiempo de inicio (debugging)
  if (x_coord == 0) x_coord = 1; //Evita dividir por 0

  //(X,Y) a coordenadas polares
  float R = sqrt( (float)(x_coord*x_coord) + (float)(y_coord*y_coord));
  float theta = atan((float)y_coord / (float)x_coord) * (to_degrees);
  if (R > ARM_LENGTH_A + ARM_LENGTH_B) return; //Valor Invalido de coordenadas

  //Calcula los angulos necesarios
  float gamma = acos((float)(constant_1-(R*R)) / constant_2);
  float alpha = (asin(sin(gamma) * ARM_LENGTH_B / R) * to_degrees) + theta;
  float beta = 180 - alpha - (gamma * to_degrees);

  if (instant_move)
  {
    instant_next = true;
  }
  //Guarda los angulos iniciales
  start_alpha = current_alpha;
  start_beta = current_beta;

  //Guarda los angulos destino
  target_alpha = (int)alpha;
  target_beta = (int)beta;

  servo_waiting = true;

  return;
}

bool SERVO_waiting() //Devuelve "false" si los servos pueden recibir instrucciones nuevas
{
  return servo_waiting;
}

bool SERVO_is_lifted()
{
  return (angle_vertical == LIFT_ANGLE_UP);
}

//Eleva o desciende el servo vertical
void SERVO_lift(bool lifted)
{
  if (lifted)
  { 
    if (angle_vertical != LIFT_ANGLE_UP)
    {
      SERVO_setAngle(SERVO_VERTICAL_ID, LIFT_ANGLE_UP);
      servo_waiting = true;
      lift_timer = LIFT_DELAY;
    }
  }
  else
  {
    if (angle_vertical != LIFT_ANGLE_DOWN)
    {
      SERVO_setAngle(SERVO_VERTICAL_ID, LIFT_ANGLE_DOWN);
      servo_waiting = true;
      lift_timer = LIFT_DELAY;
    }
  }
}


/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
//Funcion Bloqueante para testear el funcionamiento de los tres servos
void SERVO_test()
{
  while (1)
  { 
    servo_alpha.write(0);
    servo_beta.write(0);
    servo_vertical.write(0);
    Serial.println("Pos: 0");
    delay(1500);
   
    servo_alpha.write(90);
    servo_beta.write(90);
    servo_vertical.write(90);
    Serial.println("Pos: 90");
    delay(1500);

    servo_alpha.write(180);
    servo_beta.write(180);
    servo_vertical.write(180);
    Serial.println("Pos: 180");
    delay(1500);
  }
}

//Funcion usada para calcular los valores maximos y minimos de X para cada altura (rango de dibujo)
void SERVO_calculate_range()
{
  float R, theta, gamma, alpha, beta;

  int pos_y = ARM_LENGTH_A;
  int pos_x = ARM_LENGTH_B;
  bool negative_found = false;

  int min_x = ARM_LENGTH_B;
  int max_x = ARM_LENGTH_B;
  bool min_x_found, max_x_found;

  for (int y = ARM_LENGTH_A; y > -ARM_LENGTH_A; y--) // For each height
  {
    pos_y = y;  
    pos_x = ARM_LENGTH_B; // Reset X position and min/max values
    min_x = ARM_LENGTH_B; 
    max_x = ARM_LENGTH_B;
    min_x_found = false;
    max_x_found = false;

    // Calculate min_x
    negative_found = false;
    while (!negative_found)
    {
      pos_x--;
      R = sqrt((float)(pos_x * pos_x) + (float)(pos_y * pos_y));

      if (R > ARM_LENGTH_A + ARM_LENGTH_B || pos_x <= 0) {
        negative_found = true;
        break;
      }

      theta = atan((float)pos_y / (float)pos_x) * to_degrees;
      gamma = acos((float)(constant_1 - (R * R)) / constant_2);
      alpha = (asin(sin(gamma) * ARM_LENGTH_B / R) * to_degrees) + theta;
      beta = 180 - alpha - (gamma * to_degrees);

      if ((alpha >= 0 && beta >= 0) && (alpha <= 180 && beta <= 180)) {
        min_x = pos_x; // Update minimum X
        min_x_found = true;
      }
      else
        negative_found = true;
    }

    // Calculate max_x
    pos_x = ARM_LENGTH_B;
    negative_found = false;
    while (!negative_found)
    {
      pos_x++;
      R = sqrt((float)(pos_x * pos_x) + (float)(pos_y * pos_y));

      if (R > ARM_LENGTH_A + ARM_LENGTH_B) {
        negative_found = true;
        break;
      }

      theta = atan((float)pos_y / (float)pos_x) * to_degrees;
      gamma = acos((float)(constant_1 - (R * R)) / constant_2);
      alpha = (asin(sin(gamma) * ARM_LENGTH_B / R) * to_degrees) + theta;
      beta = 180 - alpha - (gamma * to_degrees);

      if ((alpha >= 0 && beta >= 0) && (alpha <= 180 && beta <= 180)) {
        max_x = pos_x; // Update maximum X
        max_x_found = true;
      }
      else
        negative_found = true;
    }

    if (min_x_found && max_x_found)
      Serial.println("Altura (" + String((int)pos_y) + "): Min X: (" + String((int)min_x) + "), Max X: (" + String((int)max_x) + ")");
    else
      Serial.println("Altura (" + String((int)pos_y) + "): No valid range for X");
  }
  Serial.println("TEST FINALIZADO");
  delay(5000);
}
