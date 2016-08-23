#include <Energia.h>
#include <WiFi.h>
#include "wifi.h"
#include "http.h"
#include "Ultrasonic.h"

#define WLAN_PUB_SSID   "INSERT_HERE"
#define WLAN_KEY_CODE   ""
#define RPI_IP_ADDRESS  "192.168.10.35"
#define RPI_PORT        80
#define RPI_PATH        "/distance"
#define TRIGGER         15
#define ECHO            14

Ultrasonic ultrasonic(TRIGGER, ECHO);

int ports[2][7] = {{30,24,10,2,27,28,29},{3,4,5,6,7,8,9}};

int leds [10][7]= {{1,1,1,1,1,1,0},
                   {0,1,1,0,0,0,0},
                   {1,1,0,1,1,0,1},
                   {1,1,1,1,0,0,1},
                   {0,1,1,0,0,1,1},
                   {1,0,1,1,0,1,1},
                   {1,0,1,1,1,1,1},
                   {1,1,1,0,0,0,0},
                   {1,1,1,1,1,1,1},
                   {1,1,1,1,0,1,1}};

void showDigit(int x, int num){
  for (int i = 0; i < 7; i++){
    digitalWrite (ports[num][i],leds[x][i]? HIGH:LOW);
  }
}

void show(int dist){
  showDigit((dist / 10) % 10,0);
  showDigit((dist % 10),1);
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 7; i++){
    pinMode(ports[0][i], OUTPUT);
    pinMode(ports[1][i], OUTPUT);
  } 
  connectWifi(WLAN_PUB_SSID, WLAN_KEY_CODE);
}

void loop() {
  char receive_msg_buffer[1024];
  char send_msg_buffer[128];
  
  while (true) {
    delay(100);
    float distance = ultrasonic.Ranging(CM);
    if (distance > 49) {
      distance = 50.0;
    }
    show((int)distance);
    Serial.println(distance);
    sprintf(send_msg_buffer, "%lf", distance);
    httpPostRequest(RPI_IP_ADDRESS, RPI_PORT, RPI_PATH, send_msg_buffer, receive_msg_buffer);
  }
}
