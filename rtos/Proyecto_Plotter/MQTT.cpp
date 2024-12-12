#include "headers/MQTT.h"

static bool initialized_flag = false;
static bool manual_mode = false;

WiFiClientSecure net;
PubSubClient client(net);

static void messageArrived(char* topic, byte* payload, unsigned int length); //Recibe los mensajes
static void processCommand(const char* command, JsonVariant data); //Ejecuta las funciones correspondientes al comando recibido 
static void process_stroke(JsonVariant data); //Recibe los puntos de un trazo, y los une con lineas rectas

static void sendMessage(const char* topic, const char* field, const char* messageContent); //Formatea el mensaje para ser enviado por MQTT
static void sendMessage(const char* topic, const char* messageContent); //Envia un mensaje con el campo predeterminado

static int ping_timer = 0;

bool MQTT_is_manual_mode() //Devuelve "true" si el modo de dibujo manual se encuentra activado
{
  //Si no esta inicializado el MQTT, automaticamente se setea a modo manual
  if (!initialized_flag) return true;
  else return manual_mode;
}

void MQTT_init() 
{
  // Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    //Conectando
    delay(500);
  }

  // Certificados SSL 
  net.setCACert(root_ca);
  net.setCertificate(certificate);
  net.setPrivateKey(private_key);

  // MQTT client
  client.setServer(ADDRESS, 8883); // Secure MQTT port for AWS IoT Core
  client.setCallback(messageArrived);

  // Conecta al MQTT broker
  while (!client.connected()) {
    if (client.connect(CLIENTID)) 
    {
      //Conectado a AWS
    } 
    else 
    {
      //Reintentando conexion
      delay(500);
    }
  }
  
  //Configuraciones
  client.setBufferSize(49152); //48 kB
  client.setKeepAlive(60);
  client.subscribe(TOPIC_IN, QOS);
  sendMessage(TOPIC_OUT, "Plotter Conectado");

  initialized_flag = true;
}

void MQTT_update()
{
  if (initialized_flag)
  {
    // Se reconecta si es necesario
    if (!client.connected()) 
    {
      //Reconectando a MQTT
      while (!client.connected()) {
        if (client.connect(CLIENTID)) {
          client.subscribe(TOPIC_IN, QOS);
        } else {
          Serial.print("Error al conectar al MQTT: ");
          Serial.println(client.state());
        }
      }
    }
    // Procesa mensajes entrantes
    client.loop();

    if (ping_timer > 0) ping_timer--; //Decrementa el timer desde la ultima vez que se recibio un ping
  }
}

// Funcion callback para mensajes entrantes
static void messageArrived(char* topic, byte* payload, unsigned int length)
{
  String payloadString = "";
  for (int i = 0; i < length; i++) 
  {
    payloadString += (char)payload[i];
  }
  Serial.println(payloadString);

  // Parsea el JSON
  DynamicJsonDocument doc(40000);

  DeserializationError error = deserializeJson(doc, payloadString);
  if (error) 
  {
    sendMessage(TOPIC_OUT,"Error: Formato JSON invalido");
    return;
  }

  if (doc.containsKey(COMMAND_FIELD))
  {
    // Extrae los datos de "command" y "data" del JSON
    const char* command = doc["command"];
    JsonVariant data = doc["data"];
    processCommand(command, data);
  }
  else
  {
    sendMessage(TOPIC_OUT, "Error: Campos no reconocidos");
    return;
  }
}

// Procesa comandos segun el tipo
static void processCommand(const char* command, JsonVariant data)
{
  if (strcmp(command, "START") == 0)
  {
    DRAWING_MODULE_start();
    sendMessage(TOPIC_OUT, "Dibujo iniciado");
  } 
  else if (strcmp(command, "STOP") == 0)
  {
    DRAWING_MODULE_stop();
    DRAWING_MODULE_reset();
    ARM_lift(true);
    ARM_standby_position();
    sendMessage(TOPIC_OUT, "Dibujo detenido y lineas eliminadas. Posicion Standby.");
  } 
  else if (strcmp(command, "STROKE") == 0)
  {
    process_stroke(data);
  }
  else if (strcmp(command, "MANUAL_ON") == 0)
  {
    manual_mode = true;
    sendMessage(TOPIC_OUT, "Modo manual activado");
  } 
  else if (strcmp(command, "MANUAL_OFF") == 0)
  {
    manual_mode = false;
    sendMessage(TOPIC_OUT, "Modo manual desactivado");
  } 
  else if (strcmp(command, "VERTUP") == 0)
  {
    ARM_lift(true);
    sendMessage(TOPIC_OUT, "Brazo Up");
  } 
  else if (strcmp(command, "VERTDOWN") == 0)
  {
    ARM_lift(false);
    sendMessage(TOPIC_OUT, "Brazo Down");
  }
  else if (strcmp(command, "VERTTOGGLE") == 0)
  {
    ARM_lift(!ARM_is_lifted());
    sendMessage(TOPIC_OUT, "Toggled");
  }
  else if (strcmp(command, "MOVE") == 0)
  {
    int dx = data["dx"].as<int>();  
    int dy = data["dy"].as<int>();  

    ARM_shift_by(dx, dy); 
    sendMessage(TOPIC_OUT, "Brazo controlado");
  } 
  else if (strcmp(command, "PING") == 0)
  {
    ping_timer = PING_CYCLES;
    sendMessage(TOPIC_OUT, "Ping Recibido");
  } 
  else if (strcmp(command, "STANDBY") == 0)
  {
    DRAWING_MODULE_stop();
    ARM_lift(true);
    ARM_standby_position();
    sendMessage(TOPIC_OUT, "Standby - Posicion inicial");
  } 

  //Dibujos prearmados
  else if (strcmp(command, "PREMADE") == 0)
  {
    const char* shape = data.as<const char*>();

    if (strcmp(shape, "CIRCLE") == 0) { PREMADE_drawCircle(); }
    else if (strcmp(shape, "CIRCLES") == 0) { PREMADE_drawMultipleCircles(); }
    else if (strcmp(shape, "SQUARE") == 0) { PREMADE_drawSquare(); }
    else if (strcmp(shape, "STAR") == 0) { PREMADE_drawStar(); }
    else if (strcmp(shape, "TRIANGLE") == 0) { PREMADE_drawTriangle(); }
    else if (strcmp(shape, "HEART") == 0) { PREMADE_drawHeart(); }
    else { 
        sendMessage(TOPIC_OUT, "Error: Figura no reconocida"); 
        return; 
    }
    sendMessage(TOPIC_OUT, "Figura cargada");
  }

  else 
  { 
    sendMessage(TOPIC_OUT, "Error: Comando no reconocido"); 
  }
}

static void process_stroke(JsonVariant data) 
{
  int error_code = 0;
  int startX, startY, endX, endY;

  JsonObject obj = data.as<JsonObject>(); 
  JsonArray x = obj["x"];
  JsonArray y = obj["y"];

  // Chequea que los datos sean utilizables
  if (x.size() < 2 || y.size() < 2) {
    sendMessage(TOPIC_OUT, "Error: Se necesitan al menos 2 puntos para trazar una linea");
    return;
  }
  if (x.size() != y.size()) {
    sendMessage(TOPIC_OUT, "Error: Los arreglos x e y deben tener el mismo tamaño.");
    return;
  }

  // Draw lines between consecutive points
  for (size_t i = 0; i < x.size() - 1; i++) {
    startX = x[i];
    startY = y[i];
    endX = x[i + 1];
    endY = y[i + 1];

    error_code = DRAWING_MODULE_add_line(startX, startY, endX, endY);

    if (error_code == 1) { sendMessage(TOPIC_OUT, "Error: Arreglo de lineas lleno"); return; }
    else if (error_code == 2) { sendMessage(TOPIC_OUT, "Error: Coordenadas fuera de rango"); return; }
  }

  sendMessage(TOPIC_OUT, "Linea Procesada");
}


static void sendMessage(const char* topic, const char* field, const char* messageContent)
{
  // Documento Json
  StaticJsonDocument<200> jsonDoc;
  jsonDoc[field] = messageContent;
  char jsonBuffer[256];
  serializeJson(jsonDoc, jsonBuffer);

  // Envia el mensaje al topic especificado, formateado como JSON
  client.publish(topic, jsonBuffer, false);
}
static void sendMessage(const char* topic, const char* messageContent)
{
  sendMessage(topic, "message", messageContent);
}

bool MQTT_ping_status() //Devuelve verdadero si el timer del ping es mayor a cero
{
  return (ping_timer > 0);
}

