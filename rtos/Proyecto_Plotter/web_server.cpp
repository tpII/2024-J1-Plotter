#include "web_server.h"

// Datos de la red
const char *ssid = "ESP32-Access-Point";
const char *password = "12345678";

WebServer server(80);
static bool server_initialized = false;

static void handleRoot();
static void handleButton1();
static void handleButton2();
static void handleButton3();
static void handleButton4();
static String getPage();

static void PREMADE_drawSquare();
static void PREMADE_drawTriangle(); 
static void PREMADE_drawCircle();
static void PREMADE_drawHeart(); 
static void PREMADE_drawStar(); 
static void PREMADE_drawCharacter(); 

// Funciones asociadas a los botones
static void executeFunction1() 
{
  //Volver a la posicion inicial
  DRAWING_MODULE_stop();
  ARM_move_to(STARTING_X, STARTING_Y);
}

static void executeFunction2() 
{
  //Comenzar Dibujo
  DRAWING_MODULE_start(); //Comienza a dibujar desde la linea '0'
}

static void executeFunction3() 
{
  //Detener Dibujo
  DRAWING_MODULE_stop();
}

static void executeFunction4() 
{
  PREMADE_drawSquare();
}
static void executeFunction5() 
{
  PREMADE_drawTriangle();
}
static void executeFunction6() 
{
  PREMADE_drawCircle();
}
static void executeFunction7() 
{
  PREMADE_drawHeart();
}
static void executeFunction8() 
{
  PREMADE_drawStar();
}
static void executeFunction9() 
{
  PREMADE_drawCharacter();
}


// Handle root page
static void handleRoot() 
{
  server.send(200, "text/html", getPage());
}

// Handle button 1
static void handleButton1() 
{
  executeFunction1();
  server.send(200, "text/plain", "Posicion Inicial");
}

// Handle button 2
static void handleButton2() 
{
  executeFunction2();
  server.send(200, "text/plain", "Dibujo Iniciado");
}

// Handle button 3
static void handleButton3() 
{
  executeFunction3();
  server.send(200, "text/plain", "Dibujo Detenido");
}

// Handle button 4
static void handleButton4() 
{
  executeFunction4();
  server.send(200, "text/plain", "Cuadrado Configurado");
}
static void handleButton5() 
{
  executeFunction5();
  server.send(200, "text/plain", "Triangulo Configurado");
}
static void handleButton6() 
{
  executeFunction6();
  server.send(200, "text/plain", "Circulo Configurado");
}
static void handleButton7() 
{
  executeFunction7();
  server.send(200, "text/plain", "Corazon Configurado");
}
static void handleButton8() 
{
  executeFunction8();
  server.send(200, "text/plain", "Estrella Configurado");
}
static void handleButton9() 
{
  executeFunction9();
  server.send(200, "text/plain", "Character Configurado");
}


void WEB_SERVER_init() 
{
  WiFi.softAP(ssid, password);
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/button1", handleButton1);
  server.on("/button2", handleButton2);
  server.on("/button3", handleButton3);
  server.on("/button4", handleButton4);
  server.on("/button5", handleButton5);
  server.on("/button6", handleButton6);
  server.on("/button7", handleButton7);
  server.on("/button8", handleButton8);
  server.on("/button9", handleButton9);

  server.begin();
  server_initialized = true;

  Serial.println("Server initialized");
}

void WEB_SERVER_update() 
{
  if (server_initialized) {
    server.handleClient();
  }
}

static String getPage() {
  String page = "<!DOCTYPE html>\n";
  page += "<html>\n";
  page += "<head><title>ESP32 Web Server</title></head>\n";
  page += "<body>\n";
  page += "<h1>ESP32 Web Server</h1>\n";
  page += "<button onclick=\"fetch('/button1').then(response => response.text()).then(alert)\">Starting Pos</button><br><br>\n";
  page += "<button onclick=\"fetch('/button2').then(response => response.text()).then(alert)\">Start</button><br><br>\n";
  page += "<button onclick=\"fetch('/button3').then(response => response.text()).then(alert)\">Stop</button><br><br>\n";
  page += "<button onclick=\"fetch('/button4').then(response => response.text()).then(alert)\">Square</button><br><br>\n";
  page += "<button onclick=\"fetch('/button5').then(response => response.text()).then(alert)\">Triangle</button><br><br>\n";
  page += "<button onclick=\"fetch('/button6').then(response => response.text()).then(alert)\">Circle</button><br><br>\n";
  page += "<button onclick=\"fetch('/button7').then(response => response.text()).then(alert)\">Heart</button><br><br>\n";
  page += "<button onclick=\"fetch('/button8').then(response => response.text()).then(alert)\">Star</button><br><br>\n";
  page += "<button onclick=\"fetch('/button9').then(response => response.text()).then(alert)\">Char</button><br><br>\n";
  page += "</body>\n";
  page += "</html>\n";
  return page;
}

//===========================================================
// Dibujos para testear

static void PREMADE_drawSquare()
{
  DRAWING_MODULE_reset();
  DRAWING_MODULE_add_line( 10, 120, 90, 120);
  DRAWING_MODULE_add_line( 90, 120, 90, 10);
  DRAWING_MODULE_add_line( 90, 10, 10, 10);
  DRAWING_MODULE_add_line( 10, 10, 10, 120);
}


static void PREMADE_drawTriangle() 
{
    DRAWING_MODULE_reset();
    DRAWING_MODULE_add_line(20, 120, 90, 120);
    DRAWING_MODULE_add_line(90, 120, 55, 30); 
    DRAWING_MODULE_add_line(55, 30, 20, 120);  
}

static void PREMADE_drawCircle() 
{
    DRAWING_MODULE_reset();
    const int centerX = 55;
    const int centerY = 70;
    const int radius = 45;
    const int numSegments = 20;

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

static void PREMADE_drawHeart() 
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

static void PREMADE_drawStar() 
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

static void PREMADE_drawCharacter() 
{
    DRAWING_MODULE_reset();

    DRAWING_MODULE_add_line(45, 110, 60, 110); 
    DRAWING_MODULE_add_line(60, 110, 65, 100);
    DRAWING_MODULE_add_line(65, 100, 60, 95);  
    DRAWING_MODULE_add_line(60, 95, 55, 100);  
    DRAWING_MODULE_add_line(55, 100, 45, 95); 
    DRAWING_MODULE_add_line(45, 95, 40, 100); 
    DRAWING_MODULE_add_line(40, 100, 45, 110); 

    DRAWING_MODULE_add_line(45, 95, 40, 75); 
    DRAWING_MODULE_add_line(40, 75, 50, 70);
    DRAWING_MODULE_add_line(50, 70, 45, 65);
    DRAWING_MODULE_add_line(45, 65, 55, 65); 
    DRAWING_MODULE_add_line(55, 65, 50, 70);  
    DRAWING_MODULE_add_line(50, 70, 60, 75);  
    DRAWING_MODULE_add_line(60, 75, 55, 95); 

    DRAWING_MODULE_add_line(42, 87, 47, 85);
    DRAWING_MODULE_add_line(47, 85, 45, 80);
    DRAWING_MODULE_add_line(45, 80, 42, 87);

    DRAWING_MODULE_add_line(52, 87, 57, 85);
    DRAWING_MODULE_add_line(57, 85, 55, 80);
    DRAWING_MODULE_add_line(55, 80, 52, 87);

    DRAWING_MODULE_add_line(60, 75, 65, 85);
    DRAWING_MODULE_add_line(65, 85, 70, 75);
    DRAWING_MODULE_add_line(70, 75, 75, 85);
    DRAWING_MODULE_add_line(75, 85, 80, 75);

}