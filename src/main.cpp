#include <Arduino.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>

#include "Definitions.h"
#include "ConfigManager.h"
#include "HttpServer.h"
#include "ColorController.h"

//=========================
// Define constantes
//=========================
#define HTTP_PORT 80
#define LED_STRIP_PIN 6

//=========================
// Instanciate classes
//=========================
ConfigManager configs;
HttpServer server(HTTP_PORT, &configs);
Adafruit_NeoPixel *pixels;

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
  pixels = new Adafruit_NeoPixel(configs.get().getQtyLeds(), LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

  // Connect with WiFi
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

  // Start local DNS
  if (MDNS.begin(configs.get().mDnsName))
    Serial.println("MDNS> Listening at http://" + String(configs.get().mDnsName) + ".local");

  // Configure HTTP routes
  server.begin();
  server.on("/", HTTP_GET, ColorController::getHome);
  server.on("/color", HTTP_POST, ColorController::getHome);
  Serial.println("HTTP> Running in :" + String(HTTP_PORT));
}

void clear() {}

void static_animation()
{
  pixels->clear();

  for (int i = 0; i < (configs.get().getQtyLeds()); i++)
  {
    pixels->setPixelColor(i, pixels->Color(configs.get().primaryColor.r, configs.get().primaryColor.g, configs.get().primaryColor.b));
    pixels->show();
  }
}

void rain_animation() {}

void rainbow_animation() {}