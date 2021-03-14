#include "Unit.h"



void Unit::FillFrom(Unit* u) {
	Id = u->Id;
	Type = u->Type;
}

void const Unit::print(const char* header, DebugLevel level){
	String str0="";
	
	if (header != NULL) {
		str0 = header;
	}
	str0+="Id:";
	str0+=String((unsigned int) Id, DEC);
	str0+=";Type:";
	str0 += String((char)Type);
	str0 += ";status:";
	str0 += String((unsigned int)status, DEC);
	str0 += ";subscription:";
	str0 += (isSubscribed? "true" : "false");
	str0 += " @";
	Loger::Log(level, str0);
}
