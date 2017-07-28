#include <IOT_Sensor.h>

// Constructor
IOTServer::IOTServer(){
    // initialize all the callbacks
    on_init_webmenu = NULL;
    on_before_content = NULL;
    on_content = NULL;
    on_after_content = NULL;
    //_webMenu = new <WebMenuEntry>list();
    _webServer = new ESP8266WebServer();
    Config.read();
    
    Config.write();
}

// Initialize
void IOTServer::begin(){
    Serial.println("IOT-Server: booting...");
    IOTServer::wifi_connected=IOTServer::setupWiFi(Config.getWifiApSsid(),Config.getWifiApPassword(),
    Config.getWifiStationSsid(),Config.getWifiStationPassword());
    Serial.println("IOT-Server: starting WebServer...");
    // add internal Pages to _webMenu
    _webMenu.push_back(WebMenuEntry("index","Home","/index.html",false));
    _webMenu.push_back(WebMenuEntry("devicesetup","Setup","/device/setup.html"));
    // query for additional pages
    if(on_init_webmenu!=NULL)
    {
        on_init_webmenu(&_webMenu);
    }
}

bool IOTServer::setupWiFi(char *ap_name, char *ap_password, char *sta_name, char *sta_password)
{

    wifi_connected = false;

    int connect_tries = 3;

    WiFi.softAP(ap_name, ap_password);
    WiFi.mode(WIFI_STA);
    WiFi.hostname(ap_name);

    WiFi.begin(sta_name,
               sta_password);

    while (--connect_tries && WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        WiFi.begin(sta_name,
                   sta_password);
        Serial.println("WiFi failed, retrying.");
    }

    if (connect_tries <= 0)
    {
        Serial.println("WiFi: setting AP mode");
        WiFi.mode(WIFI_AP);
        WiFi.softAP(ap_name, ap_password);
        return false;
    }
    else
    {
        Serial.printf("WiFi: connected to %s, IP address: %s\r\n",
                      ap_name,
                      WiFi.localIP().toString().c_str());
    }

    wifi_connected = true;
    return true;
}

// delete all Settings and reboot
void IOTServer::factory_reset(){
    IOT_Config tmpconf;
    tmpconf.write();
    delay(1000);
    ESP.restart();
}
