#define WIFI_SSID "TIGO-70C3"
#define WIFI_PASS "2D9557300820"
#include "AdafruitIO_WiFi.h"

#define IO_USERNAME "Phac"
#define IO_KEY "aio_Peba00KwOzoLcIVHoFcDlTmazyUR"
#include "config.h"
#define LED_PIN 15

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
AdafruitIO_Feed *led = io.feed("led");

void setup() {
  
  pinMode(LED_PIN, OUTPUT);
  
  
  Serial.begin(115200);

 
  while(! Serial);

  
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  
  led->onMessage(handleMessage);

  
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  led->get();

}

void loop() {

  
  io.run();

}

void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");

  if(data->toPinLevel() == HIGH)
    Serial.println("HIGH");
  else
    Serial.println("LOW");


  digitalWrite(LED_PIN, data->toPinLevel());
}
