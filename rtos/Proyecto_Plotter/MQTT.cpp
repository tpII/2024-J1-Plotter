#include "headers/MQTT.h"

static bool initialized_flag = false;

WiFiClientSecure net;
PubSubClient client(net);

static bool manual_mode = false;

static void messageArrived(char* topic, byte* payload, unsigned int length); //Recibe los mensajes
static void sendMessage(const char* topic, const char* messageContent); //Formatea el mensaje para ser enviado por MQTT

//Dibujos de Prueba
static void PREMADE_drawCircle(); 
static void PREMADE_drawHeart(); 
static void PREMADE_drawStar(); 

bool MQTT_is_manual_mode() //Devuelve "true" si el modo de dibujo manual se encuentra activado
{
  return manual_mode;
}

void MQTT_init() 
{
  // Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Certificados SSL 
  net.setCACert(root_ca);
  net.setCertificate(certificate);
  net.setPrivateKey(private_key);

  // MQTT client
  client.setServer(ADDRESS, 8883); // Secure MQTT port for AWS IoT Core
  client.setCallback(messageArrived);

  // Conecta al MQTT broker
  Serial.println("Connecting to AWS IoT Core...");
  while (!client.connected()) {
    if (client.connect(CLIENTID)) {
      Serial.println("Connected to AWS IoT Core");
    } else {
      Serial.print("Failed to connect, retrying in 5 seconds... Error code: ");
      Serial.println(client.state());
      delay(1000);
    }
  }

  // Subscribe to the topic
  client.setKeepAlive(60);
  Serial.print("Subscribing to topic: ");
  Serial.println(TOPIC_IN);
  client.subscribe(TOPIC_IN, QOS);

  client.publish(TOPIC_OUT, "ESP32 Conectado", false);

  initialized_flag = true;

}

void MQTT_update()
{
  if (initialized_flag)
  {
    // Se reconecta si es necesario
    if (!client.connected()) 
    {
      Serial.println("Reconnecting to MQTT...");
      while (!client.connected()) {
        if (client.connect(CLIENTID)) {
          Serial.println("Reconnected to MQTT");
          client.subscribe(TOPIC_IN, QOS);
        } else {
          Serial.print("Failed to reconnect, retrying in 5 seconds... Error code: ");
          Serial.println(client.state());
        }
      }
    }
    // Procesa mensajes entrantes
    client.loop();
  }
}


// Funcion callback para mensajes entrantes
static void messageArrived(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived - Topic: ");
  Serial.println(topic);

  Serial.print("Message: ");
  String message = "";

  // Convert payload to a String
  for (int i = 0; i < length; i++) 
  {
    message += (char)payload[i];
  }
  Serial.println(message);

  // Process the message
  if (message == "STANDBY") 
  {
    DRAWING_MODULE_stop();
    ARM_move_to(STARTING_X, STARTING_Y, true);
    sendMessage(TOPIC_OUT, "Standby - Posicion inicial");
  } 
  else if (message == "START")
  {
    DRAWING_MODULE_start();
    sendMessage(TOPIC_OUT, "Dibujo iniciado");
  } 
  else if (message == "STOP") 
  {
    DRAWING_MODULE_stop();
    sendMessage(TOPIC_OUT, "Dibujo detenido");
  } 
  else if (message == "CIRCLE") 
  {
    PREMADE_drawCircle();
    sendMessage(TOPIC_OUT, "Circulo cargado");
  } 
  else if (message == "STAR") 
  {
    PREMADE_drawStar();
    sendMessage(TOPIC_OUT, "Estrella cargada");
  } 
  else if (message == "HEART") 
  {
    PREMADE_drawHeart();
    sendMessage(TOPIC_OUT, "Corazon cargado");
  } 
  else 
  {
    Serial.println("Unknown command: " + message);
    sendMessage(TOPIC_OUT, "Unknown command");
  }
}

static void sendMessage(const char* topic, const char* messageContent)
{
  // Documento Json
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["message"] = messageContent;
  char jsonBuffer[256];
  serializeJson(jsonDoc, jsonBuffer);

  // Envia el mensaje al topic especificado, formateado como JSON
  client.publish(topic, jsonBuffer, false);
}

////////////////////////////////////////////////////
// Dibujos predeterminados
////////////////////////////////////////////////////

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