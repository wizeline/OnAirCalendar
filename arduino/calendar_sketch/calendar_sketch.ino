/**
 * A simple Azure IoT example for sending telemetry to Iot Hub.
 */

#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#include "Esp32MQTTClient.h"
#include "secrets.h"

#include <sstream>
#include <iomanip>

#define INTERVAL 10000
#define MESSAGE_MAX_LEN 256
#define NUMPIXELS 16
#define PIN 1


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_BUILTIN, NEO_GRB + NEO_KHZ800);
const char* ssid     = SECRET_SSID;
const char* password = SECRET_PASS;
static const char* connectionString = SECRET_CONNECTION_STRING;
const char *messageData = "{\"messageId\":%d, \"Temperature\":%f, \"Humidity\":%f}";
static bool hasIoTHub = false;
static bool hasWifi = false;
int messageCount = 1;
static bool messageSending = true;
static uint64_t send_interval_ms;

static void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result)
{
  if (result == IOTHUB_CLIENT_CONFIRMATION_OK)
  {
    Serial.println("Send Confirmation Callback finished.");
  }
}

static void MessageCallback(const char* payLoad, int size)
{
  Serial.println("Message callback:");
//  if(strcmp(payLoad,"true") == 0){
//    digitalWrite(LED_BUILTIN, HIGH);
//  }
//  else{
//    digitalWrite(LED_BUILTIN, LOW);
//  }

  Serial.println("Valor");
  std::istringstream converter(payLoad);
  uint32_t value;
  converter >> std::hex >> value;
  Serial.println(value);
  setArrayColor(value);
}

static void DeviceTwinCallback(DEVICE_TWIN_UPDATE_STATE updateState, const unsigned char *payLoad, int size)
{
  char *temp = (char *)malloc(size + 1);
  if (temp == NULL)
  {
    return;
  }
  memcpy(temp, payLoad, size);
  temp[size] = '\0';
  // Display Twin message.
  Serial.println(temp);
  free(temp);
}

static int  DeviceMethodCallback(const char *methodName, const unsigned char *payload, int size, unsigned char **response, int *response_size)
{
  LogInfo("Try to invoke method %s", methodName);
  const char *responseMessage = "\"Successfully invoke device method\"";
  int result = 200;

  if (strcmp(methodName, "start") == 0)
  {
    LogInfo("Start sending temperature and humidity data");
    messageSending = true;
  }
  else if (strcmp(methodName, "stop") == 0)
  {
    LogInfo("Stop sending temperature and humidity data");
    messageSending = false;
  }
  else
  {
    LogInfo("No method %s found", methodName);
    responseMessage = "\"No method found\"";
    result = 404;
  }

  *response_size = strlen(responseMessage) + 1;
  *response = (unsigned char *)strdup(responseMessage);

  return result;
}



void setup() {
  Serial.begin(115200);
  // pinMode(LED_BUILTIN, OUTPUT);      // set the LED pin mode
  pixels.begin();
  blink(true);
  Serial.println("ESP32 Device");
  Serial.println("Initializing...");
  Serial.println(" > WiFi");
  Serial.println("Starting connecting WiFi.");

  delay(10);
  WiFi.mode(WIFI_AP);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    hasWifi = false;
  }
  hasWifi = true;
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(" > IoT Hub");
  if (!Esp32MQTTClient_Init((const uint8_t*)connectionString, true))
  {
    hasIoTHub = false;
    Serial.println("Initializing IoT hub failed.");
    return;
  }
  hasIoTHub = true;
  // Esp32MQTTClient_SetSendConfirmationCallback(SendConfirmationCallback);
  Esp32MQTTClient_SetMessageCallback(MessageCallback);
  // Esp32MQTTClient_SetDeviceTwinCallback(DeviceTwinCallback);
  // Esp32MQTTClient_SetDeviceMethodCallback(DeviceMethodCallback);
  // Serial.println("Start sending events.");
  // randomSeed(analogRead(0));
  send_interval_ms = millis();
  blinkAnimation();
}

void loop() {
if (hasWifi && hasIoTHub)
  {
//    if (messageSending && 
//        (int)(millis() - send_interval_ms) >= INTERVAL)
//    {
//      // Send teperature data
//      char messagePayload[MESSAGE_MAX_LEN];
//      float temperature = (float)random(0,500)/10;
//      float humidity = (float)random(0, 1000)/10;
//      snprintf(messagePayload, MESSAGE_MAX_LEN, messageData, messageCount++, temperature, humidity);
//      // Serial.println(messagePayload);
//      EVENT_INSTANCE* message = Esp32MQTTClient_Event_Generate(messagePayload, MESSAGE);
//      // Esp32MQTTClient_SendEventInstance(message);
//      send_interval_ms = millis();
//    }
//    else
//    {
      Esp32MQTTClient_Check();
//    }
  }
  delay(10);
}

void setArrayColor(uint32_t color){
  if(color == 0){
    pixels.clear();
  }
  else{
    for(int i = 0; i < NUMPIXELS; i++){
      pixels.setPixelColor(i, color);
    }
  }

  pixels.show();
}

void blinkAnimation(){
  blink(false);
  blink(true);
  blink(false);
  blink(true);
  blink(false);
}

void blink(bool on){
  uint32_t c;
  if(on)
      c = 16777215;
    else
      c = 0;
  setArrayColor(c);
  delay(200);
}
