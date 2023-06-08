//Kütüphaneler

#include <WiFiManager.h>
#include <ThingSpeak.h>;

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "DHT.h"
#include <WiFiUdp.h>
#include <NTPClient.h>               
#include <TimeLib.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>



//Tanımlı değişkenler


#define FIREBASE_HOST "haber-a2d51-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "*************************************"
#define WIFI_SSID "SUPERONLINE_Wi-Fi_6551"
#define WIFI_PASSWORD "*********"
#define Role D3
#define DHTPIN D5
#define DHTTYPE DHT11
#define topraknem A0





DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcdekranim(0x27, 16, 2);
WiFiManager wm;
WiFiClient client;
String yazi;
String yazi2;
String on = "1";
String off = "0";
int toprak;
unsigned long myChannelNumber = 1990015; 
const char * myWriteAPIKey = "8GCP86JWRV1FU4FT";
bool res; 
void setup() {
  dht.begin();
  pinMode(topraknem, INPUT);
  pinMode(Role, OUTPUT);
  digitalWrite(Role, HIGH);
  lcdekranim.init();
  lcdekranim.backlight();
  lcdekranim.setCursor(0, 0);
  lcdekranim.print("Hello Sabri");
  lcdekranim.setCursor(0, 1);
  lcdekranim.print("Welcome Back!");
  delay(2000);
  lcdekranim.clear();
  lcdekranim.setCursor(0, 0);
  lcdekranim.print("Sistem");
  lcdekranim.setCursor(0, 1);
  lcdekranim.print("Baslatiliyor...");
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  res = wm.autoConnect("SabriESP", "12345678");
  if (!res) {
    Serial.println("Bağlantı Sağlanamadı");
  }
  else {
    Serial.println("Ağ Bağlantısı Kuruldu");
  }
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(3000);
    ThingSpeak.begin(client);
}
void sicaklikokuma() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
    delay(1000);
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  lcdekranim.clear();
  lcdekranim.setCursor(0, 0);
  lcdekranim.print("Sicaklik:");
  lcdekranim.setCursor(10, 0);
  lcdekranim.print(t);
  lcdekranim.setCursor(0, 1);
  lcdekranim.print("Hava Nem: %");
  lcdekranim.setCursor(11, 1);
  lcdekranim.print(h);
  toprak = analogRead(topraknem);
    delay(1000);
    ThingSpeak.writeField(myChannelNumber, 8,t, myWriteAPIKey); 
  delay(1000);
  ThingSpeak.writeField(myChannelNumber, 7,h, myWriteAPIKey);
  delay(1000);
  Firebase.setFloat("sicaklik", t);
  if (Firebase.failed()) {
    Serial.print("setting /sicaklik failed:");
    Serial.println(Firebase.error());
    return;
  }

  delay(1000);
  
  Firebase.setFloat("nem", h);
  if (Firebase.failed()) {
    Serial.print("setting /nem failed:");
    Serial.println(Firebase.error());
    return;
  }

    delay(1000);
  

  
  Serial.println(toprak);
  int  toprak1 = map(toprak, 630, 1023, 100, 0);
  Serial.println(toprak1);

  
  Firebase.setFloat("toprak", toprak1);
    delay(1000);


  
   if (Firebase.failed()) {
    Serial.print("setting /toprak failed:");
    Serial.println(Firebase.error());
    return;
  }
  delay(1000);

  
  if (toprak1 < 40) {
    Firebase.setString("role", "0");
    if (Firebase.failed()) {
    Serial.print("setting /sicaklik failed:");
    Serial.println(Firebase.error());
    return;
  }
    delay(1000);
  }
  else if (toprak1 > 60) {
    Firebase.setString("role", "1");
    if (Firebase.failed()) {
    Serial.print("setting /sicaklik failed:");
    Serial.println(Firebase.error());
    return;
  }
  }
    delay(1000);
}
void loop() {
    delay(1000);
  sicaklikokuma();
    delay(1000);
  if (on == Firebase.getString("role")) {
    digitalWrite(Role, HIGH);
    if (Firebase.failed()) {
    Serial.print("setting /sicaklik failed:");
    Serial.println(Firebase.error());
    return;
  }
  delay(1000);
  }
  
  else if (off == Firebase.getString("role")) {
    digitalWrite(Role, LOW);
    if (Firebase.failed()) {
    Serial.print("setting /sicaklik failed:");
    Serial.println(Firebase.error());
    return;
  }
  delay(1000);
  }

  
}
