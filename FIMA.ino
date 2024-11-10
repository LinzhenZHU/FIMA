#include "LIS3DHTR.h"
#include "rpcWiFi.h"
#include "TFT_eSPI.h"
#include <PubSubClient.h>
#include <Arduino.h>
#include <SensirionI2CSht4x.h>
#include <Wire.h>
LIS3DHTR<TwoWire> lis;

const char* ssid = ""; // your mobile hotspot SSID (WiFi Name)
const char* password = "";  // your mobile hotspot password (WiFi Password)
const char* mqtt_server = "broker.hivemq.com";  // MQTT Broker URL
const char *ID = "";  // Name of you device, must be unique. Replace xxxx with your last 4 digits of student ID 
const char *topic = ""; //Topic from your device

// Declare all variables and objects here
SensirionI2CSht4x sht4x;
TFT_eSPI tft;
WiFiClient wioClient;
PubSubClient client(wioClient);
char msg[50];
char RFmsg[16];
uint16_t errorCode;
float temperature;
float humidity;
String data;
String RF;
float IMU_values;

// Inner using parameters
int on_status = -1;
int trigger;
int RFID;
int flag;

// WiFi setup functions
void setup_wifi() {

  delay(10); //small delay to prevent error

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(360, 20);
  tft.print("Connect Wi-Fi");

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); // Connecting WiFi

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  tft.fillScreen(TFT_BLACK);
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // Display Local IP Address
}

// MQTT reconnect function
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(ID)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      //client.subscribe("subcribedTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(WIO_BUZZER, OUTPUT);
  lis.begin(Wire1);
  lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ); //Data output rate
  lis.setFullScaleRange(LIS3DHTR_RANGE_2G); //Scale range set to 2g
  Wire.begin(); //initialise I2C
  tft.begin();  //initialise LCD
  tft.setTextSize(3);
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);

  setup_wifi();
  client.setServer(mqtt_server, 1883); // Connect the MQTT Server

  sht4x.begin(Wire);
}

void loop() {

  delay(5);

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  errorCode = sht4x.measureHighPrecision(temperature, humidity);
  IMU_values = lis.getAccelerationX();

  // Fridge open: refresh the stock
  if (on_status == 1 && flag == 0){
    tft.fillScreen(TFT_WHITE);
    flag = 1;
  }
  if (on_status == 1 && flag == 1){
    client.publish(topic, "Open");
    tft.setCursor(360, 20);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.print("Scan RFID Tag");
    /*
    RF = String("ID:")+String("1"); //Match with items in the database
    RF.toCharArray(RFmsg, 16);
    client.publish(topic, "refresh");
    client.publish(topic, RFmsg); */
    // refresh stock
  }
  //fridge close: monitor the temperature and humidity

  if (on_status == -1 && flag == 1){
    tft.fillScreen(TFT_BLACK);
    flag = 0;
  }

  if (on_status == -1){
    //Unusual Temperature & Humidity alarm
    if (temperature >= 25 || humidity >= 70){
      analogWrite(WIO_BUZZER, 128);
      delay(200);
      analogWrite(WIO_BUZZER,64);
      delay(100);
      analogWrite(WIO_BUZZER,0);
      client.publish(topic, "Fridge Condition Warning!");
    }
    //String data;
    data = String("Temperature:\n")+String(temperature)+String("\n\n")+String("Humidity:\n")+String(humidity);
    data.toCharArray(msg, 50);
    tft.setCursor(260, 0);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.print(data);
    tft.setCursor(260, 150);
    tft.print("Inventory Low\nApple\n\nExpire Recent\nEgg, Fish");
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(topic, msg);
  }
  // Frideg on/off detection
  if (IMU_values > 0.25){
    if (on_status == -1){
      on_status = - on_status;
      delay(600);
    }
    else if (on_status == 1){
      on_status = - on_status;
      delay(1000);
    }
  }

}