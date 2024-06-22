/*
 * MQTT Sender/Receiver starter
 *  By Yonatan Rozin (yonatanrozin.com)
 *  
 * Subscribes to an MQTT topic, prints received msg contents
 * Publishes IMU data in MQTT messages
 * 
 * Configure code with your WiFi network name + password,
 *  MQTT Broker URL + port # (1883 by default),
 *  broker username + password, if needed, and
 *  topics to be subscribed + published to
 */

#include <Arduino_LSM6DS3.h>

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

const char mqtt_user[] = "public";
const char mqtt_pass[] = "public";

// choose a topic to send messages to
const char publishTopic[] = "testTopic"; 
// choose a topic to subscribe to
const char subscribeTopic[] = "click/+";

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
  mqtt.setId((String)millis());

  // provide a username and password, if required
  // remove this line if user/pass not required by broker
  mqtt.setUsernamePassword(mqtt_user, mqtt_pass); 

  Serial.print("Connecting to MQTT broker.");

  while (!mqtt.connect(broker, port)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nMQTT broker connected!");

  // subscribe to your topic, or several
   mqtt.subscribe(subscribeTopic);

   mqtt.onMessage(handleMQTTMessage);

  // topics can be unsubscribed using:
  // mqtt.unsubscribe(topic);

  IMU.begin();
}

void loop() {
  //do this regularly to keep your MQTT connection alive.
  //if you don't poll regularly and don't send any messages,
  //the MQTT broker will kick you off!
  mqtt.poll();

  //publish acceleration data
  //do note that MQTT is not particularly great
  //at real-time, high-frequency messages.
  //have a look at UDP and WebSockets for a 
  //good alternative
  if (IMU.accelerationAvailable()) {
    Serial.println("Sending mqtt message.");
    float x, y, z;
    IMU.readAcceleration(x, y, z);

    //publish arduino tilt values when updated
    mqtt.beginMessage(publishTopic);
    mqtt.print(x);
    mqtt.endMessage();
    
    delay(1000/60); //delay a bit
  }
}

void handleMQTTMessage(int msgSize) {
    Serial.print(mqtt.messageTopic()); //get message topic, if you'd like
    Serial.print(": ");

    //buffer to hold message contents
    char mqttBuffer[msgSize]; 
    //read message into buffer
    int msgLen = mqtt.readBytes(mqttBuffer, msgSize);
    mqttBuffer[msgLen] = 0;
 
    Serial.println(mqttBuffer);

    // to interpret mqtt buffer as a number:
    // float msgNum = atof(mqttBuffer);
    // Serial.println(msgNum / 3); //do stuff with number
}
