////////////////////////////////////////////////////
// Dibujos pre-armados - usados para testear      //
////////////////////////////////////////////////////

#include "headers/premade_drawings.h"

void PREMADE_drawCircle() 
{
  DRAWING_MODULE_reset();
  const int centerX = 65;
  const int centerY = 70;
  const int radius = 45;
  const int numSegments = 60;

  float angleStep = 2 * 3.14159265 / numSegments;
  float startX = centerX + radius * cos(0);
  float startY = centerY + radius * sin(0);

  float lastX = startX;
  float lastY = startY;

  for (int i = 1; i <= numSegments; i++) {
      float angle = i * angleStep;
      float nextX = centerX + radius * cos(angle);
      float nextY = centerY + radius * sin(angle);

      DRAWING_MODULE_add_line(lastX, lastY, nextX, nextY);

      lastX = nextX;
      lastY = nextY;
  }
}

void PREMADE_drawHeart() 
{
  DRAWING_MODULE_reset();
  DRAWING_MODULE_add_line(55, 100, 40, 110);
  DRAWING_MODULE_add_line(40, 110, 30, 100);
  DRAWING_MODULE_add_line(30, 100, 30, 80);
  DRAWING_MODULE_add_line(30, 80, 40, 70);
  DRAWING_MODULE_add_line(40, 70, 55, 50);
  DRAWING_MODULE_add_line(55, 50, 70, 70);
  DRAWING_MODULE_add_line(70, 70, 80, 80);
  DRAWING_MODULE_add_line(80, 80, 80, 100);
  DRAWING_MODULE_add_line(80, 100, 70, 110);
  DRAWING_MODULE_add_line(70, 110, 55, 100);
}

void PREMADE_drawStar() 
{
  DRAWING_MODULE_reset();

  int points[10][2] = {
      {55, 30}, {63, 60},
      {90, 60}, {67, 75},
      {75, 110}, {55, 90},
      {35, 110}, {43, 75},
      {20, 60}, {47, 60}
  };

  for (int i = 0; i < 10; i++) {
      int startX = points[i][0];
      int startY = points[i][1];
      int endX = points[(i + 1) % 10][0];
      int endY = points[(i + 1) % 10][1];

      DRAWING_MODULE_add_line(startX, startY, endX, endY);
  }
}

void PREMADE_drawTriangle() 
{
  DRAWING_MODULE_reset();
  DRAWING_MODULE_add_line(20, 120, 90, 120);
  DRAWING_MODULE_add_line(90, 120, 55, 30); 
  DRAWING_MODULE_add_line(55, 30, 20, 120);  
}

void PREMADE_drawSquare()
{
DRAWING_MODULE_reset();
DRAWING_MODULE_add_line( 10, 120, 90, 120);
DRAWING_MODULE_add_line( 90, 120, 90, 40);
DRAWING_MODULE_add_line( 90, 40, 10, 40);
DRAWING_MODULE_add_line( 10, 40, 10, 120);
}

void PREMADE_drawMultipleCircles() 
{
  DRAWING_MODULE_reset();

  float angle;
  float nextX;
  float nextY;

  //Dimensiones del area de dibujo (X,Y): [111, 140] 

  //Primera circunferencia
  int centerX = 75;
  int centerY = 90;
  int radius = 30;
  int numSegments = 30;
  float angleStep = 2 * 3.14159265 / numSegments;
  float startX = centerX + radius * cos(0);
  float startY = centerY + radius * sin(0);
  float lastX = startX;
  float lastY = startY;
  for (int i = 1; i <= numSegments; i++) {
      angle = i * angleStep;
      nextX = centerX + radius * cos(angle);
      nextY = centerY + radius * sin(angle);
      DRAWING_MODULE_add_line(lastX, lastY, nextX, nextY);
      lastX = nextX;
      lastY = nextY;
  }

  //Segunda circunferencia
  centerX = 80;
  centerY = 50;
  radius = 30;
  numSegments = 30;
  angleStep = 2 * 3.14159265 / numSegments;
  startX = centerX + radius * cos(0);
  startY = centerY + radius * sin(0);
  lastX = startX;
  lastY = startY;
  for (int i = 1; i <= numSegments; i++) 
  {
      angle = i * angleStep;
      nextX = centerX + radius * cos(angle);
      nextY = centerY + radius * sin(angle);
      DRAWING_MODULE_add_line(lastX, lastY, nextX, nextY);
      lastX = nextX;
      lastY = nextY;
  }

  //Tercera circunferencia
  centerX = 40;
  centerY = 50;
  radius = 30;
  numSegments = 30;
  angleStep = 2 * 3.14159265 / numSegments;
  startX = centerX + radius * cos(0);
  startY = centerY + radius * sin(0);
  lastX = startX;
  lastY = startY;
  for (int i = 1; i <= numSegments; i++) 
  {
      angle = i * angleStep;
      nextX = centerX + radius * cos(angle);
      nextY = centerY + radius * sin(angle);
      DRAWING_MODULE_add_line(lastX, lastY, nextX, nextY);
      lastX = nextX;
      lastY = nextY;
  }
}
