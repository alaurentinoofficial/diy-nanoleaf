#include <Arduino.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include "Definitions.h"
#include "ConfigManager.h"
#include "HttpServer.h"
#include "ColorController.h"

//=========================
// Define constantes
//=========================
#define HTTP_PORT 80

//=========================
// Instanciate classes
//=========================
ConfigManager configs;
HttpServer server(HTTP_PORT, &configs);

//=========================
// Define functions
//=========================
void startup_server();
void setup_server();

void clear();
void static_animation();
void rain_animation();
void rainbow_animation();

//=========================
// Setup
//=========================
void setup()
{
  Serial.begin(115200);

  if (configs.begin())
    startup_server(); // Case is already configurated
  else
    setup_server(); // First time configuration
}

//=========================
// Loop
//=========================
void loop()
{
  server.handleClient();

  switch(configs.get().mode)
  {
    case Modes::Static:
      static_animation();
      break;
    case Modes::Rain:
      rain_animation();
      break;
    case Modes::Rainbow:
      rainbow_animation();
    default:
      clear();
  }
}

void setup_server()
{
}

void startup_server()
{
  WiFi.begin(configs.get().wifiSSID, configs.get().wifiPassword);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi> SSID: ");
  Serial.println(configs.get().wifiSSID);
  Serial.print("WiFi> IP: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin(configs.get().mDnsName))
    Serial.println("MDNS> Listening at http://" + String(configs.get().mDnsName) + ".local");

  server.begin();
  server.on("/", HTTP_GET, ColorController::getHome);
  server.on("/color", HTTP_POST, ColorController::getHome);
  Serial.println("HTTP> Running in :" + String(HTTP_PORT));
}

void clear() {}

void static_animation() {}

void rain_animation() {}

void rainbow_animation() {}