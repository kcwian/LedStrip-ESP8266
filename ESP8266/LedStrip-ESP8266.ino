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
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100
CRGB leds[NUM_LEDS];

enum struct ColorChannel
{
  RED = 0, GREEN = 1, BLUE = 2
};

int animationNumber = 0;
int ledBrightness = 30;
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

bool gReverseDirection = false;

// ----------------- Buttons
#define BUTTON_UP_PIN   12
#define BUTTON_DOWN_PIN 14

void setup(void) {

  // Initilize LED strip
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(ledBrightness);
  setAllLedsColor(CRGB::Red);

  // Turn off onboard LED
  pinMode(LED_BUILTIN, OUTPUT);

  // digitalWrite(0, 1);

  // Set 2 push-buttons as input
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP); // turn on pullup resistors
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP); // and set pins to input

  // Start WiFi station
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    connectingAnimation();
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
  server.on("/setAnimationNumber", HTTP_GET, handleSetAnimationNumber);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  delay(2000);
  setAnimation(1);
}

void loop(void) {
  server.handleClient();
  MDNS.update();
  handleAnimations();
}

void handleAnimations() {

  if (animationNumber == 0)
  {
    // Do nothing
    handleButtons();
  }
  // Special Animations
  else if (animationNumber == 4)
  {
    ledAnimationCylon(); // handleButtons() inside
  }
  else if (animationNumber == 5)
  {
    ledAnimationFire2012();
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
    handleButtons();
  }
  else
  {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    FillLEDsFromPaletteColors( startIndex);
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
    handleButtons();
  }
}

void handleButtons()
{
  static bool  upButtonState = HIGH, downButtonState = HIGH, lastUpButtonState = HIGH, lastDownButtonState = HIGH;
  static unsigned long lastUpButtonDebounceTime = 0, lastDownButtonDebounceTime = 0; // the last time the output pin was toggled
  static unsigned long lastUpButtonHoldDownTime = 0, lastDownButtonHoldDownTime = 0;
  unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
  unsigned long holdDownDelay = 1000;    // the hold down time; changees brightness
  static bool brightnessChangedUp = false, brightnessChangedDown = false;


  int readingUpButton = digitalRead(BUTTON_UP_PIN);
  int readingDownButton = digitalRead(BUTTON_DOWN_PIN);

  if (readingUpButton != lastUpButtonState) {
    lastUpButtonDebounceTime = millis();

    if (readingUpButton == LOW)
      lastUpButtonHoldDownTime = millis();
  }

  if ((millis() - lastUpButtonDebounceTime) > debounceDelay) {
    if (readingUpButton != upButtonState) {

      upButtonState = readingUpButton;

      if (animationNumber < 20 && upButtonState == HIGH) { // Release button changes animation
        if (brightnessChangedUp)
          brightnessChangedUp = false;
        else
          setAnimation(animationNumber + 1);
      }
    }
    else if (upButtonState == LOW && (millis() - lastUpButtonHoldDownTime) > holdDownDelay) {
      setLedBrightness(ledBrightness + 1);
      lastUpButtonHoldDownTime = millis() - (holdDownDelay - 50); // Waits 2s for the first time, then only 100ms
      upButtonState = readingUpButton;
      brightnessChangedUp = true;
    }
  }

  if (readingDownButton != lastDownButtonState) {
    lastDownButtonDebounceTime = millis();

    if (readingDownButton == LOW)
      lastDownButtonHoldDownTime = millis();
  }

  if ((millis() - lastDownButtonDebounceTime) > debounceDelay) {
    if (readingDownButton != downButtonState) {

      downButtonState = readingDownButton;

      if (animationNumber > 0 && downButtonState == HIGH) { // Release button changes animation
        if (brightnessChangedDown)
          brightnessChangedDown = false;
        else
          setAnimation(animationNumber - 1);
      }
    }
    else if (downButtonState == LOW && (millis() - lastDownButtonHoldDownTime) > holdDownDelay) {
      setLedBrightness(ledBrightness - 1);
      lastDownButtonHoldDownTime = millis() - (holdDownDelay - 50); // Waits 2s for the first time, then only 100ms
      downButtonState = readingDownButton;
      brightnessChangedDown = true;
    }
  }

  lastUpButtonState = readingUpButton;
  lastDownButtonState = readingDownButton;
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
    animationNumber = 0;
    FastLED.clear();
    FastLED.show();
  }
  server.send(200, "text/plane", ledState);
}

void handleSetLedBrightness()
{
  String ledBrightness_s = server.arg("ledBrightness");
  setLedBrightness(ledBrightness_s.toInt());
  server.send(200, "text/plane", ledBrightness_s);
}

void setLedBrightness(int newLedBrightness)
{
  if (newLedBrightness < 0)
    newLedBrightness = 0;
  else if (newLedBrightness > 60)
    newLedBrightness = 60;
  ledBrightness = newLedBrightness;
  FastLED.setBrightness(ledBrightness);
  FastLED.show();
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

void handleSetAnimationNumber()
{
  String animationNumber_s = server.arg("animationNumber");
  Serial.println(animationNumber_s);
  int animationNumber_i = animationNumber_s.toInt();
  if (animationNumber_i < 0 || animationNumber_i > 20)
    return;
  setAnimation(animationNumber_i);
  server.send(200, "text/plane", animationNumber_s);
}

void setAnimation(int newAnimationNumber)
{
  switch (newAnimationNumber)
  {
    case 0:
      break;
    case 1:
      currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND;
      break;
    case 2:
      currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;
      break;
    case 3:
      currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND;
      break;
    case 4:
      // Cylon animation
      break;
    case 5:
      // Fire2012 animation
      break;
    case 6:
      SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND;
      break;
    case 7:
      currentPalette = CloudColors_p;           currentBlending = LINEARBLEND;
      break;
    case 8:
      currentPalette = PartyColors_p;           currentBlending = LINEARBLEND;
      break;
    case 9:
      currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;
      break;
    case 10:
      currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND;
      break;
    case 11:
      SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND;
      break;
    case 12:
      SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND;
      break;
  }
  animationNumber = newAnimationNumber;
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

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}

void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     green,  green,  black,  black,
                     purple, purple, black,  black,
                     green,  green,  black,  black,
                     purple, purple, black,  black );
}


const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};

void connectingAnimation()
{
  static bool initalize = true;
  //int ledsToAnimate = 50;
  static int actualLed = NUM_LEDS - 1;

  if (initalize) {
    initalize = false;
    FastLED.clear();
  }

  if (actualLed >= 0 && actualLed < NUM_LEDS) {
    leds[actualLed] = CRGB::Red;
    FastLED.show();
    FastLED.delay(200);
  }
  actualLed--;

  if (WiFi.status() == WL_CONNECTED)
  {
    for (int i = actualLed; i >= 0; i--) {
      leds[i] = CRGB::Red;
      FastLED.show();
      FastLED.delay(10);
    }

    for (int j = 0; j < 128; j++) {
      for (int i = 0; i < NUM_LEDS; i++) {
        if (leds[i].red - 2 > 1)
          leds[i].red -= 2;
        else
          leds[i].red = 0;
        if (leds[i].green < 128)
          leds[i].green += 2;
      }
      FastLED.show();
      FastLED.delay(1);
    }

  }

  else if (actualLed == 0) {
    FastLED.clear();
    actualLed = NUM_LEDS - 1;
  }
}


void ledAnimationCylon()
{
  static uint8_t hue = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i].nscale8(250);
    }
    delay(10);
    handleButtons();
    server.handleClient();
    MDNS.update();
    if (animationNumber != 4)
      return;
  }
  for (int i = (NUM_LEDS) - 1; i >= 0; i--) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i].nscale8(250);
    }
    delay(10);
    handleButtons();
    server.handleClient();
    MDNS.update();
    if (animationNumber != 4)
      return;
  }
}

void ledAnimationFire2012()
{
  int cooling =  55;
  int sparking = 120;

  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((cooling * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < sparking ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {
    CRGB color = HeatColor( heat[j]);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (NUM_LEDS - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}
