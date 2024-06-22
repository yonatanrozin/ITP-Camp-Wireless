/*
 * UDP Sender starter code
 *  By Yonatan Rozin (yonatanrozin.com)
 *  
 * Sends high-frequency accelerometer data through UDP
 *  Be sure to configure the code with your WiFi network name + password,
 *  the IP address of your receiver and the correct port #.
 *  Also, be sure both devices are connected to the same wireless network.
 *  
 * You may need to switch out the Arduino_LSM6DS3 library for a different one
 *  if your Arduino model has a different IMU
 */

#include <Arduino_LSM6DS3.h>

#ifdef ARDUINO_ARCH_ESP32 
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

const int port = 4242; //enter your favorite number to use as port

const char receiverIP[] = "";

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
  IMU.begin();
}

void loop() {
  if (IMU.accelerationAvailable()) {
    float x, y, z;
    IMU.readAcceleration(x, y, z);

    udp.beginPacket(receiverIP, port); //create a UDP packet with IP and port
    udp.print(x); //write data to packet
    udp.print(",");
    udp.print(z);
    //this can be a button push state, knob value, etc.
    udp.endPacket(); //finish packet and send!
  }
}
