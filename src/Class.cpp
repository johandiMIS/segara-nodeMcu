#include <ArduinoJson.h>
#include "Class.h"
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>


HTTPClient http;
WiFiClient client;

void Node::Init(WiFiManager *wm)
{
    this->wm = wm;
}

void Wifi::Init(WiFiManager *wm, int pin)
{
    this->pin = pin;
    pinMode(this->pin, INPUT_PULLUP);
    this->wm = wm;
    this->wm->autoConnect("Dua Musim");
    Serial.println("Connected . . . ");
}

void Wifi::CheckReset(){
    if(digitalRead(this->pin) == LOW){
        this->wm->resetSettings();
        ESP.restart(); 
    }
}

void API::readData(){
    char json [1000];
    String stringReceived = http.getString();
    stringReceived.toCharArray(json, 1000);
      
      DeserializationError error = deserializeJson(doc, json);

      if (error)
      {
          Serial.print(F("deserializeJson() Failed:"));
          Serial.println(error.f_str());
          return;
      }
      else
      {
          
            int light[4]={doc["light1"], doc["light2"], doc["light3"], doc["light4"]};
            int aerator[4]= {doc["aerator1"], doc["aerator2"], doc["aerator3"], doc["aerator4"]};
            int ppm[4] = { doc["ppm1"], doc["ppm2"], doc["ppm3"], doc["ppm4"]};
            int start = doc["start"];

            for(int i=0;i<4;i++)
            {
                Serial.print("light ");
                Serial.print (i+1);
                Serial.print(" = ");
                Serial.println(light[i]);

                Serial.print("aerotor ");
                Serial.print (i+1);
                Serial.print(" = ");
                Serial.println(aerator[i]);

                Serial.print("ppm ");
                Serial.print (i+1);
                Serial.print(" = ");
                Serial.println(ppm[i]);
            }
            Serial.println("start =");
            Serial.println(start);
      }
}

void Kredensial::writeCredential(String credential)
{
    int credentialLength = credential.length();
    EEPROM.write(0, credentialLength);
    char credentialChar[credentialLength+1];
    credential.toCharArray(credentialChar, credentialLength+1);
    for(int i = 0; i<credentialLength; i++)
    {
      EEPROM.write(i+1, credentialChar[i]);
    }
    EEPROM.commit();
}

void API::getAPI(String SERVER, String BODY)
{
    http.begin(client, SERVER);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");         
    int httpResponseCode = http.POST(BODY);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    Serial.println("Berhasil terhubung dengan API");
    this->readData();
}

void Parsing::dataSensor(String dataIn)
{
    unsigned int j=0;
    dt[j]="";
    for(i=0;i<dataIn.length();i++){
    if ((dataIn[i] == '#') || (dataIn[i] == ','))
    {
     j++;
     dt[j]="";    
    }
  
    else
    {
    dt[j] = dt[j] + dataIn[i];
    }
  } 
      
    Serial.print("data 1 : ");
    Serial.println(dt[0]);
    Serial.print("data 2 : ");
    Serial.println(dt[1]);
    Serial.print("data 3 : ");
    Serial.println(dt[2]);
    Serial.print("data 4 : ");
    Serial.println(dt[3]);
    Serial.print("\n\n");
}
String Kredensial::readCredential()
{
    char temp;
    int charCount = EEPROM.read(0);
    String credential = "";
    for(int i = 1;  i<= charCount; i++)
    {
        temp = EEPROM.read(i);
        credential += temp ;
    }
  return credential;
}
