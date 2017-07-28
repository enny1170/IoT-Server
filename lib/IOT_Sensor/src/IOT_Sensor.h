<<<<<<< HEAD
#ifndef IOT_SENSOR_H
#define IOT_SENSOR_H
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#define MQTT_MAX_PACKET_SIZE 512
#include <PubSubClient.h>
#include <Ticker.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266SSDP.h>
#include "HTTPUpdateServer.h"
#include <IOT_Config.h>
#include <list>
#include <WebMenuEntry.h>

#define WEBMENU_CALLBACK(callback)   void (*callback)(std::list<WebMenuEntry> *webPages)
#define PAGEBUILDER_CALLBACK(callback) void(*callback)(WebMenuEntry *requestedPage,char *htmlBuffer)

class IOTServer{
    public:
    IOTServer();
    IOT_Config Config;
    void begin();
    bool wifi_connected;
    void factory_reset();

    // Callbacks
    WEBMENU_CALLBACK(on_init_webmenu);
    /*      Receives: std::list<WebMenuEntry>*
     *      This handler can be used to provice device specific web pages.
     *      Simply you have to add additional entrys in this list with
     *      IsCallbackPage=true to receive a callback from PageBuilder to provide 
     *      Contend is page requested
     */
    PAGEBUILDER_CALLBACK(on_before_content);
    /*      Receives: WebMenuEntry for requested Page and char* as htmlBuffer to return
     *      HTML Content.
     *      Is fired: from PageBuilder after Header and Menu of
     *      requested page send. And before the Contend is requested.
     *      This Handler can be used to include some custom things in internal
     *      provided pages before contend.
     */
    PAGEBUILDER_CALLBACK(on_content);
    /*      Receives: WebMenuEntry for requested Page and char* as htmlBuffer to return
     *      HTML Content.
     *      Is fired: from PageBuilder after Header and Menu and on_before_content of
     *      requested page send. Only is this requested Page marked with isCallbackPage
     *      This Handler is used to provide device specific WebPages.
     */
    PAGEBUILDER_CALLBACK(on_after_content);
    /*      Receives: WebMenuEntry for requested Page and char* as htmlBuffer to return
     *      HTML Content.
     *      Is fired: from PageBuilder after Header and Content
     *      requested page send. And before the Footer is written.
     *      This Handler can be used to include some custom things in internal
     *      provided pages after contend.
     */

    

    bool setupWiFi(char *ap_name, char *ap_password, char *sta_name, char *sta_password);

    private:

    std::list<WebMenuEntry> _webMenu;

    ESP8266WebServer* _webServer;
};
#endif
=======
#ifndef IOT_SENSOR_H
#define IOT_SENSOR_H
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#define MQTT_MAX_PACKET_SIZE 512
#include <PubSubClient.h>
#include <Ticker.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266SSDP.h>
#include "HTTPUpdateServer.h"
#include <IOT_Config.h>
#include <list>
#include <WebMenuEntry.h>

#define WEBMENU_CALLBACK(callback)   void (*callback)(std::list<WebMenuEntry> *webPages)
#define PAGEBUILDER_CALLBACK(callback) void(*callback)(WebMenuEntry *requestedPage,char *htmlBuffer)

class IOTServer{
    public:
    IOTServer();
    IOT_Config Config;
    void begin();
    bool wifi_connected;
    void factory_reset();

    // Callbacks
    WEBMENU_CALLBACK(on_init_webmenu);
    /*      Receives: std::list<WebMenuEntry>*
     *      This handler can be used to provice device specific web pages.
     *      Simply you have to add additional entrys in this list with
     *      IsCallbackPage=true to receive a callback from PageBuilder to provide 
     *      Contend is page requested
     */
    PAGEBUILDER_CALLBACK(on_before_content);
    /*      Receives: WebMenuEntry for requested Page and char* as htmlBuffer to return
     *      HTML Content.
     *      Is fired: from PageBuilder after Header and Menu of
     *      requested page send. And before the Contend is requested.
     *      This Handler can be used to include some custom things in internal
     *      provided pages before contend.
     */
    PAGEBUILDER_CALLBACK(on_content);
    /*      Receives: WebMenuEntry for requested Page and char* as htmlBuffer to return
     *      HTML Content.
     *      Is fired: from PageBuilder after Header and Menu and on_before_content of
     *      requested page send. Only is this requested Page marked with isCallbackPage
     *      This Handler is used to provide device specific WebPages.
     */
    PAGEBUILDER_CALLBACK(on_after_content);
    /*      Receives: WebMenuEntry for requested Page and char* as htmlBuffer to return
     *      HTML Content.
     *      Is fired: from PageBuilder after Header and Content
     *      requested page send. And before the Footer is written.
     *      This Handler can be used to include some custom things in internal
     *      provided pages after contend.
     */

    

    bool setupWiFi(char *ap_name, char *ap_password, char *sta_name, char *sta_password);

    private:

    std::list<WebMenuEntry> _webMenu;

    ESP8266WebServer* _webServer;
};
#endif
>>>>>>> fc7b9f8d9fff489640d6f5ca8bfdb9acca643cf4
