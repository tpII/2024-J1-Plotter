#ifndef _MQTT_H_
#define _MQTT_H_

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "arm_controller.h"
#include "drawing_module.h"

// WiFi 

// MQTT Broker (AWS IoT Core)
#define ADDRESS "a8qpa2kd8arw9-ats.iot.us-east-1.amazonaws.com"  
#define CLIENTID "ESP32-Plotter"
#define TOPIC_IN "robot/draw"
#define TOPIC_OUT "robot/feedback"
#define QOS 1

// Claves MQTT
// Certificado CA1
static const char *root_ca = 
"-----BEGIN CERTIFICATE-----\n" \
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n" \
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n" \
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n" \
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n" \
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n" \
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n" \
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n" \
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n" \
"rqXRfboQnoZsG4q5WTP468SQvvG5\n" \
"-----END CERTIFICATE-----\n";

static const char* certificate = 
"-----BEGIN CERTIFICATE-----\n" \
"MIIDWjCCAkKgAwIBAgIVAJr9+Y/t+pB+0pq9ovNheYWZKSMfMA0GCSqGSIb3DQEB\n" \
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n" \
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDEyMDkyMjEy\n" \
"MjlaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n" \
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDVKuUWXFFKEZexIFwX\n" \
"jnfV421jKO1+GCRumRmFBcAe+kjeA0aIOdWejufmP7L1YojEAhaeiHOIe36ypj2y\n" \
"O2/t8mZ7bkbMbpfcuocu2sbjswybYsPWc73A9nKnBbpKXhDk4WwynnLbV0rOKCQR\n" \
"aqp65Ny2JLkUpka6C4A2su4193EI4weryI9ZmEzlPf9TWVE5OcblTmYaSBZpAgo+\n" \
"sS+AZuGho+mGRXFfAVYLfdBOqSBLgmStIoGrv0IZZD9wpRBGD7sXoCx7SWnNSeOX\n" \
"lz5fUqMG7tj9ysbB46JbSpFoRMeBDHBzj4QulLvbVxhrr7fMOOuUXSdwd2fRM7OZ\n" \
"ch8tAgMBAAGjYDBeMB8GA1UdIwQYMBaAFK+X+9OOeLFNbTcEopgMEfl7C4gOMB0G\n" \
"A1UdDgQWBBQhDwxs9MfJaujwUMxOjfsglkIxsDAMBgNVHRMBAf8EAjAAMA4GA1Ud\n" \
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAZr7Kuq8Q0+CxpHznj9xYTGTj\n" \
"Iwsu/gsnujqnFpW1TwwtK0KNDDX5OMolALLZrABR+TDE2vr0bqii+KSZ6gZGSw5G\n" \
"AquK7IVBZXMT1uKruC6zIoXJgQZD4Us+buUaEstEp78zsgX7YPhbrZy92qg3aWWY\n" \
"oePNWAKZ1GpgmQILaneu9HK6PTlmdQsGapfhkfNDQLnZ+8Bv0FRUCWPDYBCHm5XV\n" \
"clcqvLwBP7Er83SjjLb7oBaiY+E9j7KJBc98OoHRyYfSKFaeHroW5pIeqEIapiy4\n" \
"wriIX3axozlkBHmobXzCoNveqjwQDK4ekEA3W6n5TnPOkYgtBeQMhew2hPYARA==\n" \
"-----END CERTIFICATE-----\n";

static const char* private_key = 
"-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEpAIBAAKCAQEA1SrlFlxRShGXsSBcF4531eNtYyjtfhgkbpkZhQXAHvpI3gNG\n" \
"iDnVno7n5j+y9WKIxAIWnohziHt+sqY9sjtv7fJme25GzG6X3LqHLtrG47MMm2LD\n" \
"1nO9wPZypwW6Sl4Q5OFsMp5y21dKzigkEWqqeuTctiS5FKZGuguANrLuNfdxCOMH\n" \
"q8iPWZhM5T3/U1lROTnG5U5mGkgWaQIKPrEvgGbhoaPphkVxXwFWC33QTqkgS4Jk\n" \
"rSKBq79CGWQ/cKUQRg+7F6Ase0lpzUnjl5c+X1KjBu7Y/crGweOiW0qRaETHgQxw\n" \
"c4+ELpS721cYa6+3zDjrlF0ncHdn0TOzmXIfLQIDAQABAoIBADF8Sb5NJBnbx6k4\n" \
"5+gYDL28lDJG2AtDs4FxTTlSQm1ukOfpPsB1HKA4cKFE/Fh2zWUjTvjymE+Ay8LR\n" \
"UNVFcxB8C0pwKUNgESUOsD8yTqWhdTXFUc1BBTX8gFVBwSNufuGDa0i8+9DrjNQb\n" \
"AXSBnJ327z3hfI3Mo/KgpN0N4LrqD8PTAcNkn4COjmm9yWQi83/M7gpDS3qTCDHe\n" \
"jxIgKTBcy6g0XXNQ/iR3rWORCaspYrjr0qteeodALypjZ5XXcTADP6ovp2n0YP/e\n" \
"KIeGImLZsG2L7oWpo32vdugC4nTkzGaiVlYPVsnTDZpU2fh4neZ44/clRIb9+Z7+\n" \
"/dNqhYkCgYEA+W6b+mi/+T4ge6op5tFCDFcfNEBlUN5+gdE39xgx8boMMbDdDEau\n" \
"9MDgDBdMAJD0/pUGteU82j6hkci82MEWS5+yj+S+zijI13bdumnMOOsw/C6Melk8\n" \
"COqUiqJZuaGjjYv3SeZHUOl5/BpOE5Rxlt6ntAvp8ImRJsMqp3Gsu78CgYEA2sfU\n" \
"vSB3FqSJXjL293ZF2euwECaNyT8thH3UfdHOCPbQrNPeSu34qY3HESj0pI+DgTcp\n" \
"8isNxTbBCNA5QSzU0RxYQsP4h1GHn2yuc3T1W8qNODTS1mviA4xY4DQU2AyBo00s\n" \
"n8w4EOyQx865dScowKlWwQF62/dyx5WDS7gr0BMCgYA0V/LhnMPUkSYN7Yq3hmhe\n" \
"4JWKyIy6aMaRhY9CClDI0msmrl/k6JxkfmlGAEmRwNm45xsuZrnbbtNHHaILtpFw\n" \
"M0YsjitAZiynkqk7KmKR+0EfWsFeF4XStS+Q3DsKCpKg7KNrXajPoP9ZWUfTMg6Y\n" \
"Q4db8DgaozPTt5cm5mMCTwKBgQCIbAcqD65yvhKyXJFHacbQxni4+XF8Rhr5/QSd\n" \
"mFAjotQokMySaKWgBLpW5vT2EE9uKZnMWjByA1yHRGAPVgc2hbg7mbvFlX8+WCTH\n" \
"9exxWnKI09eMwSTkFYXt+EvvFI1Z6OukkVF8rCbzkOJwPQTabj2ciHG17Cz0azS9\n" \
"6I/ZZQKBgQDJIrqvNHZYGNuzPS6N+rOMybTcA85dczd5GzXt1yTy0u174ULwU65y\n" \
"IUdyOrs2Q1Mba74RE8SmpmSyDkEwSh8jLkdfDoBcF82fkVcLWNSB1gmHi8OEOP1a\n" \
"4sihvTFKLjNbNSqlMnuBSvxgRQhhAmSXkdvEDf64Xhi+LOHzKHO/4g==\n" \
"-----END RSA PRIVATE KEY-----\n";

void MQTT_init();
void MQTT_update();

bool MQTT_is_manual_mode(); //Devuelve "true" si el modo de dibujo manual se encuentra activado

#endif