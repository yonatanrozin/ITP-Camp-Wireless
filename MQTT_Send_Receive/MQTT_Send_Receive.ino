#ifdef ESP32 
  #include <WiFi.h> 
  //Use WiFi.h for ESP32 boards (and some other Arduino boards)
#else
  #include <WiFiNINA.h>
  //Use WiFiNINA.h for Arduino Nano 33 Iot boards
#endif

#include <ArduinoMqttClient.h>

const char ssid[] = ""; //enter WiFi network name here
const char pass[] = ""; //enter WiFi password here

//enter broker info: url and port #
const char broker[] = "public.cloud.shiftr.io";
const int port = 1883; //mqtt default is 1883

// choose a topic to send messages to
const char publishTopic[] = ""; 
// choose a topic to subscribe to
const char subscribeTopic[] = "";

WiFiClient wifi;
MqttClient mqtt(wifi);

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

  // give your device a name to appear in MQTT interfaces (doesn't affect anything)
  mqtt.setId("clientId");

  // provide a username and password, if required
  mqtt.setUsernamePassword("public", "public"); //use these with the public shiftr.io broker

  Serial.print("Connecting to MQTT broker.");

  while (!mqtt.connect(broker, port)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nMQTT broker connected!");

  // subscribe to your topic, or several
  mqtt.subscribe(subscribeTopic);

  // topics can be unsubscribed using:
  // mqtt.unsubscribe(topic);
}

void loop() {
  //do this regularly to keep your MQTT connection alive.
  //if you don't poll regularly and don't send any messages,
  //the MQTT broker will kick you off!
  mqtt.poll();
  
  int messageSize = mqtt.parseMessage();
  if (messageSize) {
    // we received a message, print out the topic and contents
    // Serial.print(mqtt.messageTopic()); //get message topic, if you'd like

    char mqttBuffer[messageSize];

    int msgLen = mqtt.readBytes(mqttBuffer, messageSize);
    mqttBuffer[msgLen] = 0;

    Serial.println(mqttBuffer);

    // to interpret mqtt buffer as a number:
    // float msgNum = atof(mqttBuffer);
    // Serial.println(msgNum / 3); //do stuff with number
  }

  //every second, publish an MQTT message
  if (millis() % 1000 < 10) {
    mqtt.beginMessage(publishTopic);
    mqtt.print(millis());
    mqtt.endMessage();
    delay(10);
  }
}
