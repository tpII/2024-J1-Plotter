#include "servo_controller.h"

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

static int angle_alpha = 0;
static int angle_beta = 0;
static int angle_vertical = 0;


static void SERVO_setAngle(int servoID, int angle); //Setea el angulo del servo correspondiente 
static void SERVO_update();

void SERVO_init()
{
  servo_alpha.attach(SERVO_ALPHA_PIN);
  servo_beta.attach(SERVO_BETA_PIN);
  servo_vertical.attach(SERVO_VERTICAL_PIN);

  //Angulos iniciales
  SERVO_setAngle(SERVO_ALPHA_ID, 90); 
  SERVO_setAngle(SERVO_BETA_ID, 0); 
}

//Setea el angulo del servo correspondiente 
static void SERVO_setAngle(int servoID, int angle)
{
  if (angle < 0) angle = 0;
  if (angle > 180) angle = 180;

  switch (servoID)
  {
    case SERVO_ALPHA_ID: angle_alpha = 180-angle;
    break;

    case SERVO_BETA_ID: angle_beta = 180-angle;
    break;

    case SERVO_VERTICAL_ID: angle_vertical = angle;
    break;

    default:
    break;
  }
}

static void SERVO_update()
{
  servo_alpha.write(angle_alpha);
  servo_beta.write(angle_beta);
  servo_vertical.write(angle_vertical);
}

//Calcula los angulos alfa y beta para posicionar el extremo en (X,Y)
void SERVO_moveto(int x_coord, int y_coord)
{
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

  //Actualiza los angulos de los servos alpha y beta
  SERVO_setAngle(SERVO_ALPHA_ID, (int)alpha);
  SERVO_setAngle(SERVO_BETA_ID, (int)beta);

  SERVO_update();

  /*
  //Debugging
  //time = micros() - time;
  Serial.println("");
  Serial.println("Coordenadas (X,Y): (" + String(x_coord) + " , " + String(y_coord) + ")");
  //Serial.println("Coordenadas Polares (R,Theta): (" + String(R) + " , " + String(theta) + ")");
  Serial.println("Angulos (Alfa,Beta): ("+ String(alpha) + "," + String(beta) + ")");
  //Serial.println("Gamma: "+ String(gamma));
  //Serial.println("Tiempo de procesamiento: "+ String(time) + "us");
  Serial.println("");*/
  
}

//Eleva o desciende el servo vertical
void SERVO_lift(bool lifted)
{
  if (lifted)
    SERVO_setAngle(SERVO_VERTICAL_ID, 180);
  else
    SERVO_setAngle(SERVO_VERTICAL_ID, 0);

  SERVO_update();
}

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
