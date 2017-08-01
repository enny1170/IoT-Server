#include <IOT_Config.h>

void IOT_Config::setDeviceName(char *name)
{
    device_name = name;
}
// set DeviceType
void IOT_Config::setDeviceType(char *type)
{
    device_type = type;
}
// set DeviceSerialNumber by default initialized with the ESPChipId
void IOT_Config::setDeviceSerialNumber(char *serial_number)
{
    device_serial = serial_number;
}
// get DeviceName
char* IOT_Config::getDeviceName()
{
    return device_name;
}
// get DeviceType
char* IOT_Config::setDeviceType()
{
    return device_type;
}
// get DeviceSerialNumber by default initialized with the ESPChipId
char* IOT_Config::setDeviceSerialNumber()
{
    return device_serial;
}

void IOT_Config::setWifiApSsid(char *ssid)
{
    wifi_ap_ssid = ssid;
}

void IOT_Config::setWifiApPassword(char *password)
{
    wifi_ap_password = password;
}

void IOT_Config::setWifiStationSsid(char *ssid)
{
    wifi_station_ssid = ssid;
}

void IOT_Config::setWifiStationPassword(char *password)
{
    wifi_station_password = password;
}

void IOT_Config::setMqttServer(char *server)
{
    mqtt_host = server;
}

void IOT_Config::setMqttPort(int port)
{
    mqtt_port = port;
}

void IOT_Config::setMqttPassword(char *password)
{
    mqtt_password = password;
}

void IOT_Config::setMqttUser(char *user)
{
    mqtt_user = user;
}

void IOT_Config::setMqttDeviceName(char *deviceName)
{
    mqtt_device_name = deviceName;
}

char* IOT_Config::getWifiApSsid()
{
    return wifi_ap_ssid;
}

char* IOT_Config::getWifiApPassword()
{
    return wifi_ap_password;
}

char *IOT_Config::getWifiStationSsid()
{
    return wifi_station_ssid;
}

char *IOT_Config::getWifiStationPassword()
{
    return wifi_station_password;
}

char *IOT_Config::getMqttServer()
{
    return mqtt_host;
}

int IOT_Config::getMqttPort()
{
    return mqtt_port;
}

char *IOT_Config::getMqttUser()
{
    return mqtt_user;
}

char *IOT_Config::getMqttDeviceName()
{
    return mqtt_device_name;
}

char *IOT_Config::getMqttPassword()
{
    return mqtt_password;
}

String IOT_Config::getRawConfiguration()
{
    if (SPIFFS.begin())
    {
        if (_rawConfiguration == NULL)
        {
            if (SPIFFS.exists("/configuration.json"))
            {
                //     //file exists, reading and loading
                Serial.println("reading config file");
                File configFile = SPIFFS.open("/configuration.json", "r");
                if (configFile)
                {
                    _rawConfiguration = configFile.readString();
                    configFile.close();
                    Serial.printf("content: ");
                    Serial.println(_rawConfiguration);
                    return _rawConfiguration;
                }
                else
                {
                    Serial.println("error opening config file");
                }
            }
            else
            {
                Serial.println("file does not exist");
                return "{}";
            }
        }
        else
        {
            return _rawConfiguration;
        }
    }
    else
    {
        Serial.println("failed to mount FS");
        return "{}";
    }
    //end read
}

void IOT_Config::write()
{
    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();
    json[PARAM_WIFI_AP_SSID] = wifi_ap_ssid;
    json[PARAM_WIFI_AP_PASSWORD] = wifi_ap_password;
    json[PARAM_WIFI_AP_ENABLED] = wifi_ap_enabled;

    json[PARAM_WIFI_STATION_SSID] = wifi_station_ssid;
    json[PARAM_WIFI_STATION_PASSWORD] = wifi_station_password;
    json[PARAM_WIFI_STATION_ENABLED] = wifi_station_enabled;

    json[PARAM_MQTT_ENABLED] = mqtt_enabled;
    json[PARAM_MQTT_HOST] = mqtt_host;
    json[PARAM_MQTT_PORT] = mqtt_port;
    json[PARAM_MQTT_USER] = mqtt_user;
    json[PARAM_MQTT_PASSWORD] = mqtt_password;
    json[PARAM_MQTT_DEVICE_NAME] = mqtt_device_name;

    json[PARAM_DEVICE_NAME] = device_name;
    json[PARAM_DEVICE_TYPE] = device_type;
    json[PARAM_DEVICE_SERIAL] = device_serial;
    //
    File configFile = SPIFFS.open("/configuration.json", "w");
    if (!configFile)
    {
        Serial.println("failed to open config file for writing");
    }
    //
    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
};

bool IOT_Config::isWifiApConfigurationValid()
{

    return (
        wifi_ap_password != NULL &&
        strlen(wifi_ap_password) > 7 &&
        wifi_ap_ssid != NULL &&
        strlen(wifi_ap_ssid) > 2);
}

bool IOT_Config::isWifiStationConfigurationValid()
{
    return (
        wifi_station_password != NULL &&
        strlen(wifi_station_password) > 7 &&
        wifi_station_ssid != NULL &&
        strlen(wifi_station_ssid) > 2);
}

bool IOT_Config::isWifiApEnabled()
{
    return wifi_ap_enabled;
}

bool IOT_Config::isWifiStationEnabled()
{
    return wifi_station_enabled;
}

bool IOT_Config::isMqttEnabled()
{
    return mqtt_enabled;
}

bool IOT_Config::isMqttConfigurationValid()
{
    if (mqtt_host == NULL)
        return false;
    if (mqtt_device_name == NULL)
        return false;
    if (mqtt_port < 1000)
        return false;
    if (mqtt_port > 30000)
        return false;
    return true;
}

void IOT_Config::writeConfiguration(const char *&configuration)
{
    if (SPIFFS.begin())
    {
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.parseObject(configuration);
        File configFile = SPIFFS.open("/configuration.json", "w");
        if (!configFile)
        {
            Serial.println("failed to open config file for writing");
            return;
        }
        //
        Serial.println("Writing configuration");
        json.printTo(Serial);
        json.printTo(configFile);
        configFile.close();
        Serial.println("Done");
        _rawConfiguration = configuration;
    }
    else
    {
        Serial.println("failed to open spiffs for writing");
    }
}

void IOT_Config::writeConfiguration(String &configuration)
{
    if (SPIFFS.begin())
    {
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.parseObject(configuration);
        File configFile = SPIFFS.open("/configuration.json", "w");
        if (!configFile)
        {
            Serial.println("failed to open config file for writing");
            return;
        }
        //
        Serial.println("Writing configuration");
        json.printTo(Serial);
        json.printTo(configFile);
        configFile.close();
        Serial.println("Done");
        _rawConfiguration = configuration;
    }
    else
    {
        Serial.println("failed to open spiffs for writing");
    }
}

void IOT_Config::read()
{
    getRawConfiguration();
    if (_rawConfiguration)
    {
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.parseObject(_rawConfiguration);
        if (json.success())
        {
            Serial.println("\nparsed json");
            // reading wifi ap configuration
            readParameter(PARAM_WIFI_AP_SSID, wifi_ap_ssid, json);
            readParameter(PARAM_WIFI_AP_PASSWORD, wifi_ap_password, json);
            // reading wifi station configuration
            readParameter(PARAM_WIFI_STATION_SSID, wifi_station_ssid, json);
            readParameter(PARAM_WIFI_STATION_PASSWORD, wifi_station_password, json);
            // reading mqtt configuration
            readParameter(PARAM_MQTT_ENABLED, mqtt_enabled, json);
            readParameter(PARAM_MQTT_HOST, mqtt_host, json);
            readParameter(PARAM_MQTT_PORT, mqtt_port, json);
            readParameter(PARAM_MQTT_USER, mqtt_user, json);
            readParameter(PARAM_MQTT_PASSWORD, mqtt_password, json);
            readParameter(PARAM_MQTT_DEVICE_NAME, mqtt_device_name, json);
            // reading device configuration
            readParameter(PARAM_DEVICE_NAME, device_name, json);
            readParameter(PARAM_DEVICE_TYPE, device_type, json);
            readParameter(PARAM_DEVICE_SERIAL, device_serial, json);
        }
        else
        {
            Serial.println("failed to load json config");
        }
        // check for defaults
        if(wifi_station_ssid==new char[255])
        {
            setDefaults();
        }
    }
    else
    {
        Serial.println("failed to read configuration");
        return;
    }
    //end read
}

void IOT_Config::readParameter(String parameterName, char *variable, JsonObject &json)
{
    if (json.containsKey(parameterName))
    {
        const char *value = json[parameterName];
        if (value != NULL)
        {
            strcpy(variable, value);
        }
    }
}

void IOT_Config::readParameter(String parameterName, int variable, JsonObject &json)
{
    if (json.containsKey(parameterName))
    {
        const char *value = json[parameterName];
        if (value != NULL)
        {
            variable = atoi(value);
        }
    }
}
IOT_Config::IOT_Config()
{
    setDefaults();
}

void IOT_Config::setDefaults()
{
    char esp_id[32];
    // create an unique ID for the AP SSID and MQTT ID
    sprintf(esp_id, "CSIOT_%08x", ESP.getChipId());
    setWifiApSsid(esp_id);
    setWifiApPassword((char*)"password");
    setMqttDeviceName(esp_id);
}
