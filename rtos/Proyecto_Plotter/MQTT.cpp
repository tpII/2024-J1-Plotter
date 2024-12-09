
#include "MQTT.h"

WiFiClientSecure secureClient; 
PubSubClient mqttClient(secureClient); 

static void callback(char* topic, byte* message, unsigned int length);

void MQTT_init() 
{
  // Inicializa la conexion wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Carga los certificados
  secureClient.setCACert(ca_cert);       // CA certificate
  secureClient.setCertificate(client_cert); // Client certificate
  secureClient.setPrivateKey(client_key);   // Client private key

  // Configure la conexion con el MQTT Broker
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback); // Funcion callback para mensajes entrantes

  while (!mqttClient.connected()) 
  {
    Serial.println("Connecting to MQTT...");
    if (mqttClient.connect(thingName)) 
    {
      Serial.println("Connected to MQTT Broker!");
      mqttClient.publish(mqttTopic, "Hello, MQTT!");
      mqttClient.subscribe(mqttTopic);
    } 
    else 
    {
      Serial.print("MQTT Connection failed, rc=");
      Serial.println(mqttClient.state());
    }
  }

  //while (1) MQTT_update(); //DEBUGGING
}

static void callback(char* topic, byte* message, unsigned int length) 
{
  Serial.print("Message received on topic: ");
  Serial.println(topic);

  Serial.print("Message: ");
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)message[i]);
  }
  Serial.println();
}

void MQTT_update() 
{
  mqttClient.loop();

  if (WiFi.status() == WL_CONNECTED) {
    mqttClient.publish(mqttTopic, "Hello from ESP32!");
  }
  delay(5000); // Wait
}
