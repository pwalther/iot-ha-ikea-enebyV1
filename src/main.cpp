#include <Arduino.h>

// MQTT
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <mycredentials.h>


#define PWR D0 // GPIO 16 (has a pulldown res, the others have a pullup)
#define VOLUP D1 // VOL+
#define VOLDWN D2 // VOL-


byte mac[6];                     // the MAC address of your Wifi chip
char macStr [20];

WiFiClient wifiClient;
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

char* mqtt_server  = "192.168.1.36";
#define mqttPort 11883

#define topic_pwr "audio/eneby/power"
#define topic_volume "audio/eneby/power"

void callback(char* topic, byte* payload, unsigned int length);
void mqttreconnect();

PubSubClient client(mqtt_server, mqttPort, callback, wifiClient);


void setup() {
  Serial.begin(9600);

  pinMode(PWR, OUTPUT);
  digitalWrite(PWR, 255);

  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);



Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(WIFI_SSID, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  WiFi.macAddress(mac);

  sprintf(macStr, "%02x-%02x-%02x-%02x-%02x-%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println(macStr);
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  
}


void increaseVolume() {
  // Clockwise => V+ raises first (5-6ms / 20ms), then
  // V- raises, then 40ms
  // V+ sinks
  // V- sinks


  //
  //
  //   20         40
  //  +++-+-+-+-+-----
  //  +  -      +    -
  //  +  -      +    -
  //  +  -      +    -


  pinMode(VOLUP, OUTPUT);
  pinMode(VOLDWN, OUTPUT);

  digitalWrite(VOLUP, HIGH);
  delay(20);
  digitalWrite(VOLDWN, HIGH);
  delay(70);
  digitalWrite(VOLUP, LOW);
  delay(40);
  digitalWrite(VOLDWN, LOW);
  delay(50);

}


void pushPower() {
  // set pin LOW (pulldown)
  pinMode(PWR, INPUT_PULLDOWN_16);
  int read = digitalRead(PWR);
  Serial.printf("POWER: %i", read);

  // wait
  delay(200);

  // set pin back to HIGH again
  pinMode(PWR, OUTPUT);
  digitalWrite(PWR, 255);
}


void loop() {

  if (!client.connected()) {
    mqttreconnect();
  }
  client.loop();

  delay(3);
}


  // MQTT
void mqttreconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(macStr)) {
      Serial.println("connected");
      client.subscribe(topic_pwr);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived

  if(length > 50) {
    // ignore oversized msgs
    return;
  }
  char payloadBuffer[50];
 
  strncpy(&payloadBuffer[0], (char *)payload, length);
  payloadBuffer[length] = '\0';

  String payloads = String((char*)payload);
  Serial.print("MQTT msg: ");
  Serial.println(payloads);
  Serial.print("MQTT topic: ");
  Serial.println(topic);
  Serial.print("MQTT msg length: ");
  Serial.println(length);

  // whatever you send us... we do the things we can do.

  int neighborhoodEntertainmentLevel = 10;
  for(int i = 0; i < neighborhoodEntertainmentLevel; i++) {
    increaseVolume();
  }
  
  pushPower();
}