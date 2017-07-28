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

    const char* htmlHead="<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><link rel='icon' href='favicon.ico'><title>CS&sup2; IOT-Server</title><link href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css' rel='stylesheet'><link href='starter-template.css' rel='stylesheet'></head><body>";
    const char* htmlNav="<nav class='navbar navbar-inverse navbar-fixed-top'><div class='container'><div class='navbar-header'><button type='button' class='navbar-toggle collapsed' data-toggle='collapse' data-target='#navbar' aria-expanded='false' aria-controls='navbar'><span class='sr-only'>Toggle navigation</span><span class='icon-bar'></span><span class='icon-bar'></span><span class='icon-bar'></span></button><a class='navbar-brand' href='index.html'>CS&sup2; IOT-Server</a></div><div id='navbar' class='collapse navbar-collapse'><ul class='nav navbar-nav'><li class='active'><a href='index.html'>Home</a></li><li><a href='state.html'>State</a></li><li><a href='switch.html'>Switch</a></li><li><a href='log.html'>Log</a></li><li><a href='about.html'>About</a></li></ul></div><!--/.nav-collapse --></div></nav>";
    const char* htmlFooter="<div><p><small> &copy; by Customized Software Solutions (CS&sup2;) 2017</small></p></div><script src='/js/jqBootstrapValidation.js'></script><script>  $(function () { $('input,select,textarea').not('[type=submit]').jqBootstrapValidation(); } );</script><script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script><script>window.jQuery || document.write(\'<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js\"><\\/script>\')</script><script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script></body></html>";
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
