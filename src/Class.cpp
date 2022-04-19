#include <ArduinoJson.h>
#include "Class.h"
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>

StaticJsonDocument<1000> doc;
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

void Parsing::dataSensor()
{
  if(Serial.available()>0)
  {



    String sensorStr = Serial.readStringUntil('\n');
    // double konduktivitas;
    // double suhuAir;
    // double jarak;
    // double suhuDHT;

    // Serial.print("Konduktivitas :");
    // Serial.println(konduktivitas);
    // Serial.print("suhuAir :");
    // Serial.println(suhuAir);
    // Serial.print("Jarak :");
    // Serial.println(jarak);
    // Serial.print("suhuDHT :");
    // Serial.println(suhuDHT);
    // Serial.println("");
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
