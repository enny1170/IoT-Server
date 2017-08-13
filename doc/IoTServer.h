///////////////////////////////////////////////////////////
//  IoTServer.h
//  Implementation of the Class IoTServer
//  Created on:      10-Aug-2017 13:59:28
//  Original author: e.ziehe
///////////////////////////////////////////////////////////

#if !defined(EA_EB72C618_8B15_4106_967A_D40DD17F27E4__INCLUDED_)
#define EA_EB72C618_8B15_4106_967A_D40DD17F27E4__INCLUDED_

class IoTServer
{

public:
	IoTServer();
	virtual ~IoTServer();
	char* IpAddress;

	char* GetIpAddress();
	void SetIpAddress(char* newVal);

};
#endif // !defined(EA_EB72C618_8B15_4106_967A_D40DD17F27E4__INCLUDED_)
