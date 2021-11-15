#include "Unit.h"
#include "Configuration.h"

extern Configuration Config;


void Unit::FillFrom(const Unit* u) {
	Id = u->Id;
	Type = u->Type;
}

void const Unit::print(const char* header, DebugLevel level){
<<<<<<< HEAD
	String str0=F("");
=======
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
	
	if (header != NULL) {
		Config.Log->append(header);
	}
<<<<<<< HEAD
	str0+=F("Id:");
	str0+=(unsigned int) Id;
	str0+=F(";Type:");
	str0 += (char)Type;
	str0 += F(";status:");
	str0 += (unsigned int)status;
	str0 += F(";subscription:");
	str0 += (isSubscribed? F("true") : F("false"));
	str0 += F("@");
	Loger::Log(level, str0);
=======
	Config.Log->append(F1("Id:")).append((unsigned int) Id);
	Config.Log->append(F1(";Type:")).append((char)Type);
	Config.Log->append(F1(";status:")).append((unsigned int)status);
	Config.Log->append(F1(" @"));
	Config.Log->Log(level);
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
}
