#include "Unit.h"


bool Unit::compare(Unit* u) {
	if (u == NULL) return false;
	return (
		Id == u->Id &&
		Type == u->Type &&
		Pin == u->Pin &&
		lhOn == u->lhOn
		);
}

void Unit::FillFrom(Unit* u) {
	Id = u->Id;
	Type = u->Type;
	Pin = u->Pin;
	lhOn = u->lhOn;
}

void Unit::print(const char* header, DebugLevel level){
	String str0="";
	
	if (header != NULL) {
		str0 = header;
	}
	str0+="Id:";
	str0+=String(Id, DEC);
	str0+=";Type:";
	str0 += String(Type);
	str0 += ";Pin:";
	str0 += String(Pin, DEC);
	str0 += ";lhOn:";
	str0 += String(lhOn, DEC);
	str0 += ";status:";
	str0 += String((unsigned int)status, DEC);
	str0 += " @";
	Loger::Log(level, str0);
}
