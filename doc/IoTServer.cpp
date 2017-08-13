///////////////////////////////////////////////////////////
//  IoTServer.cpp
//  Implementation of the Class IoTServer
//  Created on:      10-Aug-2017 13:59:28
//  Original author: e.ziehe
///////////////////////////////////////////////////////////

#include "IoTServer.h"


IoTServer::IoTServer(){

}



IoTServer::~IoTServer(){

}





char* IoTServer::GetIpAddress(){

	return IpAddress;
}


void IoTServer::SetIpAddress(char* newVal){

	IpAddress = newVal;
}