
/* #include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#define MQTT_MAX_PACKET_SIZE 512
#include <PubSubClient.h>
#include <Ticker.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266SSDP.h>
 */
//#include <IOT_Sensor.h>
//#include <Hash.h>
#include <ConfigBase.h>

//IOTServer iotServer;
ConfigBase conf;
void setup()
{
    //FS.begin();
    Serial.begin(115200);
    //iotServer.begin();
}

void loop()
{
    //delay(5000);

}