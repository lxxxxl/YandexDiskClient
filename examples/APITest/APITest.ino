#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "YandexDiskClient.h"

#define WIFI_SSID "ssid"
#define WIFI_PASSWORD "password"
#define AUTH_TOKEN "TOKEN"

YandexDiskClient disk(AUTH_TOKEN);

/* Connects to specified Wi-Fi network */
void wifiConnect(){
  Serial.print("Connecting to AP");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println();
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void setup() {

  Serial.begin(9600);

  wifiConnect();


  uint8_t buf[] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x00};
  uint16_t len = 8;

  if (!disk.mkdir("/tmp")){
    Serial.println("mkdir() failed");
    return;
  }

  if (!disk.upload("/tmp/aaa.txt", buf, len)){
    Serial.println("upload() failed");
    return;
  }

  uint8_t buf2[20];
  len = 20;
  if (!disk.download("/tmp/aaa.txt", (uint8_t*)&buf2, &len)){
    Serial.println("download() failed");
    return;
  }
  Serial.println((char*)&buf2);

  if (!disk.remove("/tmp")){
    Serial.println("remove() failed");
    return;
  }

  Serial.println("all tests passed");
}

void loop() {
  delay(100000);
} 