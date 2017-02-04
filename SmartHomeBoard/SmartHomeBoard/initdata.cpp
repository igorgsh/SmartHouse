#include "initdata.h"
#include "ext_global.h"
#include <Ethernet.h>


void InitializeServer() {

	if (Ethernet.begin(Config.mac) == 0) {
		SerialLog(D_ERROR,"Failed to configure Ethernet using DHCP");
		// no point in carrying on, so do nothing forevermore:
		// try to congifure using IP address instead of DHCP:
		Ethernet.begin(Config.mac, Config.ip);
	}
	//Server.begin();
	SerialLog_(D_INFO,"Server is at ");
	SerialLog(D_INFO,Ethernet.localIP());
}


