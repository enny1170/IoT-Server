#include <IOT_Sensor.h>

// Constructor
IOTServer::IOTServer(){
    base();
    SetDefaults();
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
    IOTServer::wifi_connected=IOTServer::setupWiFi();
    Serial.println("IOT-Server: starting OTA-Server...");
    IOTServer::setupOTA();
    Serial.println("IOT-Server: starting MDNS...");
    MDNS.addService("http","tcp",80);
    Serial.println("IOT-Server: starting WebServer...");
    // Configure AsyncWebSocket
    _ws.onEvent(onEvent);
    _webServer.addHandler(&_ws);
    events.onConnect([](AsyncEventSourceClient *client){
        client->send("hello",NULL,millis(),1000);
    });
    _webServer.addHandler(&events);
    // start SPIFFS
    SPIFFS.begin();
    // register Request Handlers
    setupWebServerRequests(&_webServer);
    // Starting Webserver
    _webServer.begin()
    // add internal Pages to _webMenu
    _webMenu.push_back(WebMenuEntry("index","Home","/index.html",false));
    _webMenu.push_back(WebMenuEntry("devicesetup","Setup","/device/setup.html"));
    // query for additional pages
    if(on_init_webmenu!=NULL)
    {
        on_init_webmenu(&_webMenu);
    }
    // start additional Services is Wifi in Station mode
    if(wifi_connected){
        Serial.println("IOT-Server: starting additional Services");
        // mqtt, syslog ...
    }    
}

void IOTServer::setupOTA(){
    // Start the server
      //Send OTA events to the browser
      ArduinoOTA.onStart([]() { _events.send("Update Start", "ota"); });
      ArduinoOTA.onEnd([]() { _events.send("Update End", "ota"); });
      ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      char p[32];
      sprintf(p, "Progress: %u%%\n", (progress/(total/100)));
      _events.send(p, "ota");
      });
      ArduinoOTA.onError([](ota_error_t error) {
        if(error == OTA_AUTH_ERROR) _events.send("Auth Failed", "ota");
        else if(error == OTA_BEGIN_ERROR) _events.send("Begin Failed", "ota");
        else if(error == OTA_CONNECT_ERROR) _events.send("Connect Failed", "ota");
        else if(error == OTA_RECEIVE_ERROR) _events.send("Recieve Failed", "ota");
        else if(error == OTA_END_ERROR) _events.send("End Failed", "ota");
      });
      ArduinoOTA.setHostname(esp_id);
      ArduinoOTA.begin();
}

bool IOTServer::setupWiFi()
{

    wifi_connected = false;

    int connect_tries = 3;

    WiFi.softAP(Params_Get("ap_ssid"),Params_Get("ap_passwd"));
    WiFi.mode(WIFI_STA);
    WiFi.hostname(esp_id);

    WiFi.begin(Params_Get("st_ssid",Params_Get("st_passwd"));

    while (--connect_tries && WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        WiFi.begin(Params_Get("st_ssid",Params_Get("st_passwd"));
        Serial.println("WiFi failed, retrying.");
    }

    if (connect_tries <= 0)
    {
        Serial.println("WiFi: setting AP mode");
        WiFi.mode(WIFI_AP);
        WiFi.softAP(Params_Get("ap_ssid"),Params_Get("ap_passwd"));
        saveWiFiMac();
        return false;
    }
    else
    {
        Serial.printf("WiFi: connected to %s, IP address: %s\r\n",
                      ap_name,
                      WiFi.localIP().toString().c_str());
    }
    saveWiFiMac();
    wifi_connected = true;
    return true;
}

void IOTServer::SetDefaults(){
    // initialize defaultConfiguration
    Key="Wifi";
    // create an unique ID for the AP SSID 
    sprintf(esp_id, "CSIOT_%08x", ESP.getChipId());
    // Generate needed Config Items
    Params_Add("ap_enabled","1");
    Params_Add("st_ssid","");
    Params_Add("st_passwd","");
    Params_Add("ap_ssid",esp_id);
    Params_Add("ap_passwd","password");
    Params_Add("dhcp_enabled","1");
    Params_add("ip","0.0.0.0");
    Params_Add("mask","0.0.0.0");
    Params_Add("gateway","0.0.0.0");
    // additional Parameters to control depending services
    Params_Add("mqtt_enabled","0");
    Params_Add("syslog_enabled","0");
    if(write){
        WriteRawConfigFile(confFileName);
    }
}

// delete all Settings and reboot
void IOTServer::factory_reset(){
    SetDefaults(true);
    delay(1000);
    ESP.restart();
}

void IOTServer::saveWiFiMac(){
    uint8_t[6] _mac_array;
    WiFi.macAddress(_mac_array);
    for(i = 0; i < sizeof(_mac_array);i++){
        sprintf(wifi_mac,"%s02x:",wifi_mac,_mac_array[i]);
    }
}

void IOTServer::onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
    if(type == WS_EVT_CONNECT){
      Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
      client->printf("Hello Client %u :)", client->id());
      client->ping();
    } else if(type == WS_EVT_DISCONNECT){
      Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
    } else if(type == WS_EVT_ERROR){
      Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
    } else if(type == WS_EVT_PONG){
      Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
    } else if(type == WS_EVT_DATA){
      AwsFrameInfo * info = (AwsFrameInfo*)arg;
      String msg = "";
      if(info->final && info->index == 0 && info->len == len){
        //the whole message is in a single frame and we got all of it's data
        Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);
  
        if(info->opcode == WS_TEXT){
          for(size_t i=0; i < info->len; i++) {
            msg += (char) data[i];
          }
        } else {
          char buff[3];
          for(size_t i=0; i < info->len; i++) {
            sprintf(buff, "%02x ", (uint8_t) data[i]);
            msg += buff ;
          }
        }
        Serial.printf("%s\n",msg.c_str());
  
        if(info->opcode == WS_TEXT)
          client->text("I got your text message");
        else
          client->binary("I got your binary message");
      } else {
        //message is comprised of multiple frames or the frame is split into multiple packets
        if(info->index == 0){
          if(info->num == 0)
            Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
        }
  
        Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);
  
        if(info->opcode == WS_TEXT){
          for(size_t i=0; i < info->len; i++) {
            msg += (char) data[i];
          }
        } else {
          char buff[3];
          for(size_t i=0; i < info->len; i++) {
            sprintf(buff, "%02x ", (uint8_t) data[i]);
            msg += buff ;
          }
        }
        Serial.printf("%s\n",msg.c_str());
  
        if((info->index + len) == info->len){
          Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
          if(info->final){
            Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
            if(info->message_opcode == WS_TEXT)
              client->text("I got your text message");
            else
              client->binary("I got your binary message");
          }
        }
      }
    }
  }
  
  void IOTServer::setupWebServerRequests(AsyncWebServer server){
  // *****************************************************************************************************
  // Confiugre Webserver Requests
  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });
  server.rewrite("/","/index.html");
  // Status Page
  server.on("/state.html",HTTP_GET,[](AsyncWebServerRequest *request)
  {
    Serial.println("Handle Request for state.html");
    String activeClient=request->client()->remoteIP().toString();
    char *tmp= new char[activeClient.length()+1];
    strcpy(tmp, activeClient.c_str());
    //String page=htmlH.BuildStatus(tmp);
    AsyncResponseStream *response=request->beginResponseStream("text/html");
    response->print(htmlHead);
    response->print(htmlNav);
    //response->print(page);
    response->print(htmlFooter);
    request->send(response);
  });
  // Switch Page
  server.on("/switch.html",HTTP_GET,[](AsyncWebServerRequest *request)
  {
    Serial.println("Handle Request for switch.html");
    String page;
   /*  if(digitalRead(relaysPin)==HIGH)
    {
      page=htmlH.BuildSwitch(true,ActiveOrder);
    }
    else
    {
      page=htmlH.BuildSwitch(false,ActiveOrder);
    } */
    AsyncResponseStream *response=request->beginResponseStream("text/html");
    response->print(htmlHead);
    response->print(htmlNav);
    response->print(page);
    response->print(htmlFooter);
    request->send(response);
  });

/*   server.on("/switch.html",HTTP_POST,[](AsyncWebServerRequest *request)
  {
    Serial.println("Handle Post Request for switch.html");
    int params=request->params();
    bool newState=false;
    char tmpOrder[20]={0};
    for(int i=0;i<params;i++)
    {
      // Get the Params
      AsyncWebParameter* p=request->getParam(i);
      if(p->isPost())
      {
        if(p->name()=="submit")
        {
            if(p->value()=="1")
            {
              newState=true;
            }
            else
            {
              newState=false;
            }
        }
        if(p->name()=="newOrder")
        {
          strncpy(tmpOrder,p->value().c_str(),20);
        }
        Serial.printf("POST[%s]: %s\n",p->name().c_str(),p->value().c_str());
      }
    }
    // decide what is to do
    if(!newState)
    {
      digitalWrite(relaysPin,LOW);
      strcpy(ActiveOrder,"");
      AppendLogFile(request->client()->remoteIP(),(char*)"Web",(char*)"Switch Off");
    }
    else
    {
      digitalWrite(relaysPin,HIGH);
      strcpy(ActiveOrder,tmpOrder);
      char abuffer[100]={0};
      strcpy(abuffer,"Switch ON for Order# ");
      strcat(abuffer,tmpOrder);
      Serial.println(abuffer);
      AppendLogFile(request->client()->remoteIP(),(char*)"Web",abuffer);
    }
    String page;
    if(digitalRead(relaysPin)==HIGH)
    {
      page=htmlH.BuildSwitch(true,ActiveOrder);
    }
    else
    {
      page=htmlH.BuildSwitch(false,ActiveOrder);
    }
    AsyncResponseStream *response=request->beginResponseStream("text/html");
    response->print(htmlHead);
    response->print(htmlNav);
    response->print(page);
    response->print(htmlFooter);
    request->send(response);
  });
 */
  // Log Page
/*   server.on("/log.html",HTTP_GET,[](AsyncWebServerRequest *request)
  {
    Serial.println("Handle Request for log.html");
    String page;
    page=htmlH.BuildLog();
    AsyncResponseStream *response=request->beginResponseStream("text/html");
    response->print(htmlHead);
    response->print(htmlNav);
    response->print(page);
    response->print(htmlFooter);
    request->send(response);
  });

  server.on("/log.html",HTTP_POST,[](AsyncWebServerRequest *request)
  {
    Serial.println("Handle POST Request for log.html");
    if(SPIFFS.exists("/log.txt"))
    {
      File f = SPIFFS.open("/log.txt", "w");
      f.close();
    }
    AppendLogFile(request->client()->remoteIP(),(char*)"Web",(char*)"truncate Log...");
    String page;
    page=htmlH.BuildLog();
    AsyncResponseStream *response=request->beginResponseStream("text/html");
    response->print(htmlHead);
    response->print(htmlNav);
    response->print(page);
    response->print(htmlFooter);
    request->send(response);
  });
 */
  server.serveStatic("/files", SPIFFS, "/");
  

  server.onNotFound([](AsyncWebServerRequest *request){
    Serial.printf("NOT_FOUND: ");
    if(request->method() == HTTP_GET)
      Serial.printf("GET");
    else if(request->method() == HTTP_POST)
      Serial.printf("POST");
    else if(request->method() == HTTP_DELETE)
      Serial.printf("DELETE");
    else if(request->method() == HTTP_PUT)
      Serial.printf("PUT");
    else if(request->method() == HTTP_PATCH)
      Serial.printf("PATCH");
    else if(request->method() == HTTP_HEAD)
      Serial.printf("HEAD");
    else if(request->method() == HTTP_OPTIONS)
      Serial.printf("OPTIONS");
    else
      Serial.printf("UNKNOWN");
    Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

    if(request->contentLength()){
      Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
      Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
    }

    int headers = request->headers();
    int i;
    for(i=0;i<headers;i++){
      AsyncWebHeader* h = request->getHeader(i);
      Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
    }

    int params = request->params();
    for(i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isFile()){
        Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
      } else if(p->isPost()){
        Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      } else {
        Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }

    request->send(404);
  });

  server.onFileUpload([](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final){
    if(!index)
      Serial.printf("UploadStart: %s\n", filename.c_str());
    Serial.printf("%s", (const char*)data);
    if(final)
      Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index+len);
  });
  
  server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    if(!index)
      Serial.printf("BodyStart: %u\n", total);
    Serial.printf("%s", (const char*)data);
    if(index + len == total)
      Serial.printf("BodyEnd: %u\n", total);
  });
  // End of Webserver Configuration
  // *************************************************************************

  }