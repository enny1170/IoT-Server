///////////////////////////////////////////////////////////
//  ConfigBase.cpp
//  Implementation of the Class ConfigBase
//  Created on:      10-Aug-2017 23:42:46
//  Original author: e.ziehe
///////////////////////////////////////////////////////////

#include "ConfigBase.h"


ConfigBase::ConfigBase(){

}



ConfigBase::~ConfigBase(){
    ParamsList.clear();
}





char ConfigBase::GetKey(){

	return Key;
}

bool ConfigBase::ParseConfigFile(char* filename){
    return ParseConfigString( GetRawConfigFile(filename));
}


String ConfigBase::GetRawConfigFile(char* filename){

    String _configData;

    if (SPIFFS.begin())
    {
        if (_rawConfiguration == NULL)
        {
            if (SPIFFS.exists("filename"))
            {
                //file exists, reading and loading
                Serial.println("reading config file");
                File configFile = SPIFFS.open("filename", "r");
                if (configFile)
                {
                    _configData = configFile.readString();
                    configFile.close();
                    Serial.printf("content: ");
                    Serial.println(_configData);
                    return _configData;
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


void ConfigBase::SetKey(char newVal){

	Key = newVal;
}


/**
 * Build a Json String from Configuration Object, has to be overwritten in Your
 * implementation. Have in your mind if isRoot so we have to build a valid
 * Document, Is not we have to build as String with Key
 */
String ConfigBase::BuildConfigString(bool isRoot){
    DynamicJsonBuffer jsonBuffer;
    JsonObject &json 
    if(isRoot){
        &jason=JsonBuffer.createNestedObject(Key);
    }
    else{
        &jason=JasonBuffer.createObject();
    }

    for(auto &itr:ParamsList)
    {
        json[itr:first.c_str()]=itr.second.c_str();
    }

    String output;
    root.printTo(output);
    return output;
  }

  	/** 
	 * Parse the Content of a Json String and setup ParamsList
	 */
	bool ConfigBase::ParseConfigString(String jsonString){
        bool retVal=false;
        DynamicJsonBuffer jasonBuffer;
        JsonObject& data = jsonBuffer.parseObject(json);
        try{
              // Test if parsing succeeds.
              if (!root.success()) {
                Serial.println("parseObject() failed");
                retVal=false;
                return retVal;
              }
              else{
                ParamsList.clear()
                for(auto &itr:data){
                    Params_Add(itr.first,itr.second);
                }
                retVal=true;
                return retVal;
              }
        }
        catch(exception){
            retVal=false;
            Serial.println("Error during reading of JsonObject");
        }
        return retVal;
    }
    

/**
 * Writes the _rawConfiguration Json String to SPIFS File
 */
void ConfigBase::WriteRawConfigFile(char filename){
    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();
    for(auto &itr:ParamsList)
    {
        json[itr:first.c_str()]=itr.second.c_str();
    }
	File configFile = SPIFFS.open("filename", "w");
    if (!configFile)
    {
        Serial.println("failed to open config file for writing");
    }
    //
    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
}

void ConfigBase::Params_Add(String key, String newValue){
    if(ParamsList.count(key.c_str())!=1){
        ParamsList.emplace(key.c_str(),value.c_str());
    }
    else
    {
        ParamsList[key]=newValue.c_str();
    }
}

void ConfigBase::Params_Add(char * key, char * newValue){
    if(ParamsList.count(key!=1){
        ParamsList.emplace(key,newValue);
    }
    else{
        ParamsList[key]=newValue);
    }
}

void ConfigBase::Params_Add(const char * key,char * newValue){
    if(ParamsList.count(key)!=1){
        ParamsList.emplace(key,newValue);
    }
    else{
        ParamsList[key]=newValue);
    }
}
char* ConfigBase::Params_Get(char* key){
    return ParamsList[key];
}

char* ConfigBase::Params_Get(const char * key){
    return ParamsList[key];
}

/**
 * This Method can be used to Set DefaultConfig shoul be overwritten in your Implementation
 */
virtual void ConfigBase::SetDefaults(bool write=false){
    // Content has to be set in your implementation
}
     