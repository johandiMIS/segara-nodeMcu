#include <Arduino.h>
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

void API::PostAPI(String SERVER, String JsonBody)
{
    http.begin(client, SERVER);
    http.addHeader("Content-Type", "application/json");
    
    int responseCode = http.POST(JsonBody);  
    const String response = http.getString();

    Serial.print("Response Status : ");
    Serial.print(responseCode);
    Serial.print("Response Body : ");
    Serial.println(response);
}



// void Parsing::dataSensor(String dataIn)
// {
//     int j = 0;
//     dt[j]="";
//     for(int i=0 ; i < (int)dataIn.length() ; i++){
//         if ((dataIn[i] == '#') || (dataIn[i] == ','))
//         {
//             j++;
//             dt[j] = "";    
//         }
//         else
//         {
//             dt[j] = dt[j] + dataIn[i];
//         }
//     } 
      
//     Serial.print("data 1 : ");
//     Serial.println(dt[0]);
//     Serial.print("data 2 : ");
//     Serial.println(dt[1]);
//     Serial.print("data 3 : ");
//     Serial.println(dt[2]);
//     Serial.print("data 4 : ");
//     Serial.println(dt[3]);
//     Serial.print("\n\n");
// }
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
