#include "web_server.h"

// Datos Access Point
const char *ssid = "ESP32-Access-Point";
const char *password = "12345678";

static String getPage();
WebServer server(80);

static bool server_initialized = false;

static String getPage();

void handleRoot() 
{
  server.send(200, "text/html", getPage());
}

// Inicializar el servidor
void WEB_SERVER_init() 
{
  WiFi.softAP(ssid, password);
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.begin();
  server_initialized = true;

  Serial.println("Servidor inicializado");
}

void WEB_SERVER_update() 
{
  if (server_initialized)
  {
    //server.handleClient();
  }
}

static String getPage() 
{
  String page = "<html> </html>";
  return page;
}