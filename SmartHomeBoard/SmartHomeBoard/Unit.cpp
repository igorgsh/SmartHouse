#include "Unit.h"



void Unit::FillFrom(const Unit* u) {
	Id = u->Id;
	Type = u->Type;
}

void const Unit::print(const char* header, DebugLevel level){
	String str0=F("");
	
	if (header != NULL) {
		str0 = header;
	}
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
}
