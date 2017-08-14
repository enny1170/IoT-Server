///////////////////////////////////////////////////////////
//  ConfigBase.h
//  Implementation of the Class ConfigBase
//  Created on:      10-Aug-2017 23:42:46
//  Original author: e.ziehe
///////////////////////////////////////////////////////////

#ifndef _CONFIGBASE_H_
#define _CONFIGBASE_H_
#include <FS.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson
#include <Arduino.h>     //https://github.com/arduino/Arduino
#include <ESP8266WiFi.h>
#include <unordered_map>

/**
 * Minimum Implementation for JSON Configuration
 */
class ConfigBase
{

public:
	ConfigBase();
	virtual ~ConfigBase();
	char* Key;
	/**
	 * This is a Hashtable of Params as String, you can Add by using Params_Add an
	 * take the Value by Params_Get. Also Iteration is possible by fallowing Code
	 * for(auto (and)itr : ParamsList)
	 * {
	 *   Printf("key: %s, value: %s",itr.first,itr.second);
	 * }
	 */
	std::unordered_map<const char*,const char*> ParamsList;
	//std::list<String> ParamsList;

	virtual char* GetKey();
	virtual String GetRawConfigFile(char* filename);
	virtual void SetKey(char* newVal);
	/**
	 * Build a Json String from Configuration Object, has to be overwritten in Your
	 * implementation. Have in your mind if isRoot so we have to build a valid
	 * Document, Is not we have to build as String with Key
	 */
	virtual String BuildConfigString(bool isRoot = false);
	/**
	 * Writes the _rawConfiguration Json String to SPIFS File
	 */
	virtual void WriteRawConfigFile(char* filename);

	/** 
	 * Parse the Content of a Json String and setup ParamsList
	 */
	bool ParseConfigString(String jsonString);
	bool ParseConfigFile(char* filename);
	//void Params_Add(String key, String newValue);
	//void Params_Add(char * key,char * newValue);
	void Params_Add(const char *key, const char * newValue);
	//void Params_Add(const char *key, char * newValue);
	//char* Params_Get(String key);
	//char* Params_Get(char * key);
	const char* Params_Get(const char * key);
	
	/**
	 * This Method can be used to Set DefaultConfig shoul be overwritten in your Implementation
	 */
	virtual void SetDefaults(bool write=false);
private:
    String _rawConfiguration;
};
#endif 
