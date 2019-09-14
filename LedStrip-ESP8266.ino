#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FastLED.h>
#include "index.h"

// ----------------- Server
const char* ssid = "";
const char* password = "";
ESP8266WebServer server(80);

// ----------------- Leds
#define LED_PIN     4
#define NUM_LEDS    120
#define BRIGHTNESS  60
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100
CRGB leds[NUM_LEDS];

enum struct ColorChannel
{
  RED = 0, GREEN = 1, BLUE = 2
};

void setup(void) {

  // Initilize LED strip
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  setAllLedsColor(CRGB::Red);

  // Turn off onboard LED
  pinMode(0, OUTPUT);
  digitalWrite(0, 0);

  // Start WiFi station
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/turnLed", HTTP_GET, handleTurnLed);
  server.on("/setLedBrightness", HTTP_GET, handleSetLedBrightness);
  server.on("/setLedChannelValue", HTTP_GET, handleSetLedChannelValue);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  setAllLedsColor(CRGB::Green);

}

void loop(void) {
  server.handleClient();
  MDNS.update();
}

void handleRoot()
{
  String s = MAIN_page;
  server.send(200, "text/html", s);
}

void handleTurnLed()
{
  String ledState = server.arg("ledState");
  Serial.println(ledState);
  if (ledState == "1")
  {
    setAllLedsColor(CRGB::Green);
    FastLED.show();
  }
  else if (ledState == "0")
  {
    FastLED.clear();
    FastLED.show();
  }
  server.send(200, "text/plane", ledState);
}

void handleSetLedBrightness()
{
  String ledBrightness = server.arg("ledBrightness");
  Serial.println(ledBrightness);
  FastLED.setBrightness(ledBrightness.toInt());
  FastLED.show();
  server.send(200, "text/plane", ledBrightness);
}

void handleSetLedChannelValue()
{
  String ledRedValue = server.arg("ledRedValue");
  String ledGreenValue = server.arg("ledGreenValue");
  String ledBlueValue = server.arg("ledBlueValue");

  if (ledRedValue.length() != 0)
  {
    setAllLedsChannelValue((int)ColorChannel::RED, ledRedValue.toInt());
    server.send(200, "text/plane", ledRedValue);
  }
  if (ledGreenValue.length() != 0)
  {
    setAllLedsChannelValue((int)ColorChannel::GREEN, ledGreenValue.toInt());
    server.send(200, "text/plane", ledGreenValue);
  }
  if (ledBlueValue.length() != 0)
  {
    setAllLedsChannelValue((int)ColorChannel::BLUE, ledBlueValue.toInt());
    server.send(200, "text/plane", ledBlueValue);
  }
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setAllLedsColor(CRGB color)
{
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = color;
  FastLED.show();
}

void setAllLedsChannelValue(int colorChannel, int value)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    switch (colorChannel)
    {
      case ((int) ColorChannel::RED):
        leds[i].red = value;
        break;
      case ((int) ColorChannel::GREEN):
        leds[i].green = value;
        break;
      case ((int) ColorChannel::BLUE):
        leds[i].blue = value;
        break;
    }
  }

  FastLED.show();
}
