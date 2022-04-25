#include<Arduino.h>
#include <WiFiManager.h> // WiFiManager.h 0.16.0
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h> //ArduinoJson.h 6.18.5
#include "Class.h"
#include <EEPROM.h>

Node *node = new Node;
WiFiManager *wm = new WiFiManager;
Wifi *wifi = new Wifi;
Parsing *pars = new Parsing;
Kredensial *Credential = new Kredensial;
API *getapi = new API;

unsigned int i ,j=0;
String dt[10];
String dataIn;
String serverName, body;


void setup() 
{
  Serial.begin(115200);
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
}

void loop() 
{
  wifi->CheckReset();
  if(Serial.available()>0)
  {
    String data = Serial.readStringUntil('\n');
    String command = data.substring(0,data.indexOf('='));
    String dataValue = data.substring(data.indexOf('[')+1,data.indexOf(']'));
    String server = data.substring(data.indexOf('(')+1,data.indexOf(')'));
    String Body = data.substring(data.indexOf('{')+1,data.indexOf('}'));
    Serial.println(command);
    Serial.println(dataValue);
    if(command == "changeCredential"){ 
      // Change credential command format
      // changeCredential=[newCredential]
      Credential->writeCredential(dataValue);
      String cred = Credential->readCredential();
      Serial.println(cred);
    }
    else if (command == "checkCredential"){
      // check credential command format
      // checkCredential=[]
      String cred = Credential->readCredential();
      Serial.println(cred);
    }
    else if(command == "sensorData"){
      // Parse sensor value here
      dataIn += dataValue;
      pars->dataSensor(dataIn);
      dataIn="";
    }
    else if (command == "GetAPI") {
      Serial.println(server);
      Serial.println(Body);
      getapi->getAPI(server,Body);
    }
    
    else{
      Serial.println(data);
    }
  }
  
  delay(500);
}