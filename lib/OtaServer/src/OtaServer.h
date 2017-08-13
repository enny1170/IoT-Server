#ifndef _OTA_SERVER_H_
#define _OTA_SERVER_H_

#include <WifiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>

class OtaServer
{

    public:
        void begin(int port=8266);
        void handleOta();
};

#endif