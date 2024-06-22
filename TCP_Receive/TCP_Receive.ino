/*
 * TCP Receiver starter code
 *  By Yonatan Rozin (yonatanrozin.com)
 *  
 * Displays received TCP messages,
 *  Toggles the onboard LED when message received
 * Configure sketch with your WiFi network username + password,
 *  sender IP address and TCP port # 
 *  (make sure sender and receiver use the same #)
 *  and make sure both devices are connected to the same network!
 */
 
#ifdef ESP32
  #include <WiFi.h>   // Include WiFi library for ESP32
#else
  #include <WiFiNINA.h>   // Include WiFiNINA library for Arduino boards
#endif

const char ssid[] = ""; //enter WiFi network name here
const char pass[] = ""; //enter WiFi password here

const int port = 4242; //enter your favorite number to use as port
const char senderIP[] = "";

WiFiClient wifi;

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

  pinMode(LED_BUILTIN, OUTPUT);
  wifi.connect(senderIP, port); //start TCP connection
}

void loop() {

  // if there's data available, read a packet
  int packetSize = wifi.available();
  if (packetSize) {

    //create a buffer with length of message contents
    char inBuffer[packetSize];
    //write contents of the UDP message into buffer
    int msgLen = wifi.readBytes(inBuffer, packetSize);
    inBuffer[msgLen] = 0;

    Serial.println(inBuffer);

    // to parse message into a float:
    // float udpNum = atof(inBuffer);

    //do stuff with buffer contents here!
    
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}
