#include<Arduino.h>
#include <WiFiManager.h> // WiFiManager.h 0.16.0
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h> //ArduinoJson.h 6.18.5
#include "Class.h"
#include <EEPROM.h>

StaticJsonDocument<1000> doc;
Node *node = new Node;
WiFiManager *wm = new WiFiManager;
Wifi *wifi = new Wifi;
Parsing *pars = new Parsing;
Kredensial *Credential = new Kredensial;
API *getapi = new API;

String dt[10];
String dataIn;
String serverName, body;
String cred;


void setup() 
{
  Serial.begin(9600);
  EEPROM.begin(512);
  wifi->Init(wm,D0);
  node->Init(wm);
 
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  cred = Credential->readCredential();
}

void loop() 
{
  wifi->CheckReset();
  if(Serial.available()>0)
  {
    String data = Serial.readStringUntil('\n');
    String command = data.substring(0,data.indexOf('='));
    String dataValue = data.substring(data.indexOf('[')+1,data.indexOf(']'));
    
    Serial.println(command);
    Serial.println(dataValue);
  
    if(command == "changeCredential"){ 
      // Change credential command format
      // changeCredential=[newCredential]
      Credential->writeCredential(dataValue);
      cred = Credential->readCredential();
      Serial.println(cred);
    }
    else if (command == "checkCredential"){
      // check credential command format
      // checkCredential=[]
      cred = Credential->readCredential();
      Serial.println(cred);
    }
    else if(command == "sensorData"){
      // Parse sensor value here
      dataIn = dataValue;

      int sensorValue[5];

      for(int i = 0; i < 5; i ++){
        int separator = dataIn.indexOf(',');
        sensorValue[i] = (int)dataIn.substring(0, separator).toInt();
        dataIn = dataIn.substring(separator+1, dataIn.length());
      }
      
      StaticJsonDocument<256> jsonDoc;
      StaticJsonDocument<256> jsonData;
      jsonData["KadarGaram"] = String(sensorValue[0]);
      jsonData["SuhuKotak"] = String(sensorValue[1]);
      jsonData["KetinggianAir"] = String(sensorValue[2]);
      jsonData["SuhuRuangan"] = String(sensorValue[3]);
      jsonData["Arus"] = String(sensorValue[4]);

      jsonDoc["credential"] = cred;
      jsonDoc["data"] = jsonData;

      String serial;
      serializeJson(jsonDoc, serial);
      Serial.println(serial);

      String uri = "http://safe-taiga-38670.herokuapp.com/api/sensor/insert";
      getapi->PostAPI(uri, serial);

    }
    else{
      Serial.println(data);
    }
  }
  delay(500);
}