#ifdef ESP32 
  #include <WiFi.h> 
  //Use WiFi.h for ESP32 boards (and some other Arduino boards)
#else
  #include <WiFiNINA.h>
  //Use WiFiNINA.h for Arduino Nano 33 Iot boards
#endif

#include <WiFiUdp.h>

const char ssid[] = ""; //enter WiFi network name here
const char pass[] = ""; //enter WiFi password here

WiFiUDP udp;

const int port = 1234; //enter your favorite number to use as port

const char receiverIP[] = "192.168.0.208";

void setup() {
  Serial.begin(9600);
  delay(3000);
  Serial.print("Connecting to WiFi network.");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    WiFi.begin(ssid, pass);
    delay(1000);
  }
  Serial.println("\nConnected to WiFi network!");
  udp.begin(port);
}

void loop() {
  udp.beginPacket(receiverIP, port); //create a UDP packet with IP and port
  udp.print(millis()); //write data to packet
  //this can be a button push state, knob value, etc.
  udp.print("!!"); //you can write multiple datapoints if you want
  udp.endPacket(); //finish packet and send!

  delay(1000);
}
