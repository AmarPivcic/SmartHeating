/*
  Rui Santos
  Complete project details at our blog.
    - ESP32: https://RandomNerdTutorials.com/esp32-firebase-realtime-database/
    - ESP8266: https://RandomNerdTutorials.com/esp8266-nodemcu-firebase-realtime-database/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  Based in the RTDB Basic Example by Firebase-ESP-Client library by mobizt
  https://github.com/mobizt/Firebase-ESP-Client/blob/main/examples/RTDB/Basic/Basic.ino
*/

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
#include "DHT.h"
#include <Servo.h>

#define DHTPIN D8     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define LED D7

// Insert your network credentials
#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_PASSWORD"

// Insert Firebase project API Key
#define API_KEY "AIzaSyBN53IxSp887GFfnfqTtjnVV6i01cT0spo"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://smart-heating-11b6f-default-rtdb.europe-west1.firebasedatabase.app/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

DHT dht(DHTPIN, DHTTYPE);
Servo servo;
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void setup(){
  pinMode(D7, OUTPUT);
  pinMode(DHTPIN, INPUT);
  dht.begin();
  servo.attach(D4, 550, 2100);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

}

void loop(){
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();
  int setting;
  int autoMode;
  int wantedTemp;

  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    if (Firebase.RTDB.setFloat(&fbdo, "values/temp", temp)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    if (Firebase.RTDB.setFloat(&fbdo, "values/hum", hum)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.getInt(&fbdo, "values/setting")) {
      if (fbdo.dataType() == "int") {
        setting = fbdo.intData();
      }
    }

    else {
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.getInt(&fbdo, "values/auto")) {
      if (fbdo.dataType() == "int") {
        autoMode = fbdo.intData();
      }
    }

    else {
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.getInt(&fbdo, "values/wantedtemp")) {
      if (fbdo.dataType() == "int") {
        wantedTemp = fbdo.intData();
      }
    }

    else {
      Serial.println(fbdo.errorReason());
    }

    if(autoMode==0)
    {
      if(setting == 0)
        {
          servo.write(0);
        }

        else if(setting == 1)
        {
          servo.write(30);
        }
        else if(setting == 2)
        {
          servo.write(60);
        }
        else if(setting == 3)
        {
          servo.write(90);
        }
        else if(setting == 4)
        {
          servo.write(120);
        }
        else if(setting == 5)
        {
          servo.write(150);
        }
        else if(setting == 6)
        {
          servo.write(180);
        }
    }

    else if(autoMode==1)
    {
      if(setting == 0 || setting == 6)
      {
        if(temp < wantedTemp - 0.5)
        {
          servo.write(180);
          Firebase.RTDB.setInt(&fbdo, "values/setting", 6);
        }

        else if(temp > wantedTemp + 0.5)
        {
          servo.write(0);
          Firebase.RTDB.setInt(&fbdo, "values/setting", 0);
        }
      }
      else
      {
        setting=0;
        servo.write(0);
        Firebase.RTDB.setInt(&fbdo, "values/setting", 0);
      }
    }
  }
}
