#include <Arduino.h>
#include <WiFiManager.h> 

#define Class

class Node
{
    public:
        WiFiManager *wm;
        void Init(WiFiManager *wm);
       
};

class API 
{
    public:
        void getAPI(String server, String Body);
};

class Wifi
{
    public:
        WiFiManager *wm;
        int pin;
        void Init(WiFiManager *wm, int pin);
        void CheckReset();
};

class Parsing
{
    public:
        String sensor[5], dataIn;
        String dt[10];
        int i;
        void dataSensor(String dataIn);
};

class Kredensial
{
    public:
        String credential;
        String readCredential();
        void writeCredential(String credential);
};


