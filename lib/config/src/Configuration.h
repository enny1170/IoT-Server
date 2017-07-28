#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include <DeviceConfig.h>
#include <WiFiConfig.h>

class IotConfig{

    public:
    void IotConfig();
    deviceconfig Device;
    wificonfig Wifi;
    
};
#endif