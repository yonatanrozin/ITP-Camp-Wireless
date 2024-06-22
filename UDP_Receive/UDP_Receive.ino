#ifdef ESP32
  #include <WiFi.h>   // Include WiFi library for ESP32
#else
  #include <WiFiNINA.h>   // Include WiFiNINA library for Arduino boards
#endif

#include <WiFiUdp.h>

const char ssid[] = ""; //enter WiFi network name here
const char pass[] = ""; //enter WiFi password here

WiFiUDP udp;

const int port = 1234; //enter your favorite number to use as port

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(3000);
  Serial.print("Connecting to WiFi network");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    WiFi.begin(ssid, pass);
    delay(1000);
  }
  Serial.println("\nConnected to WiFi network!");
  Serial.println(WiFi.localIP());

  udp.begin(port);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

  // if there's data available, read a packet
  int packetSize = udp.parsePacket();
  if (packetSize) {
    IPAddress remoteIp = udp.remoteIP();

    //create a buffer with length of message contents
    char udpBuffer[packetSize];
    //write contents of the UDP message into buffer
    int msgLen = udp.read(udpBuffer, packetSize);
    udpBuffer[msgLen] = 0;

    Serial.println(udpBuffer);

    //do stuff with udp buffer contents here!
    
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}
