#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define BULB1            15
#define BULB2            2
#define BULB3            18
#define BULB4            22
#define DHTPIN            4
#define DHTTYPE  DHT11     

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Soulmates";
const char* password = "1234512345";
const char* mqtt_server = "broker.hivemq.com"; 

#define sub1 "led-1/switch"
#define sub2 "led-2/switch"
#define sub3 "led-3/switch"
#define sub4 "led-4/switch"

char str_hum_data[10];
char str_temp_data[10];

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  if (strstr(topic, sub1))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    
    if ((char)payload[0] == '1') {
      digitalWrite(BULB1, HIGH);   
      
      
    } else {
      digitalWrite(BULB1, LOW);  
    }
  }

  else if ( strstr(topic, sub2))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    
    if ((char)payload[0] == '1') {
      digitalWrite(BULB2, HIGH);   
    
     
    } else {
      digitalWrite(BULB2, LOW);  
    }
  }
  else if ( strstr(topic, sub3))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    
    if ((char)payload[0] == '1') {
      digitalWrite(BULB3, HIGH);  
      
    } else {
      digitalWrite(BULB3, LOW);  
    }
  }
  else if ( strstr(topic, sub4))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    
    if ((char)payload[0] == '1') {
      digitalWrite(BULB4, HIGH);   

    
    } else {
      digitalWrite(BULB4, LOW);  
    }
  }

  else
  {
    Serial.println("unsubscribed topic");
  }



}

void reconnect() {
  
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str()) ) {
      Serial.println("connected");
      
      client.subscribe(sub1);
      client.subscribe(sub2);
      client.subscribe(sub3);
      client.subscribe(sub4);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BULB1, OUTPUT);
  pinMode(BULB2, OUTPUT);
  pinMode(BULB3, OUTPUT);
  pinMode(BULB4, OUTPUT);
  pinMode(DHTPIN, INPUT);
  Serial.begin(9800);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

    
  
    float hum_data = dht.readHumidity();
    Serial.println(hum_data);
   
    dtostrf(hum_data, 4, 2, str_hum_data);
    float temp_data = dht.readTemperature(); 
    dtostrf(temp_data, 4, 2, str_temp_data);
  
    Serial.print("Publish message: ");
    Serial.print("Temperature - "); Serial.println(str_temp_data);
    client.publish("get/temparature", str_temp_data);
    Serial.print("Humidity - "); Serial.println(str_hum_data);
    client.publish("get/humadity", str_hum_data);
  
}