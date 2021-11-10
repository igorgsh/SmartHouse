#include "Unit.h"


/*
bool Unit::Compare(Unit* u) {
	if (u == NULL) return false;
	Loger::Debug("Compare Unit:" + String(Id == u->Id) + ":" +String(Type == u->Type) +":" + String(Pin == u->Pin) + ":" + String(lhOn == u->lhOn) + "#");
	return (
		Id == u->Id &&
		Type == u->Type &&
		Pin == u->Pin &&
		lhOn == u->lhOn &&
		status == u->status
		);
}
*/

void Unit::FillFrom(const Unit* u) {
	Id = u->Id;
	Type = u->Type;
}

void const Unit::print(const char* header, DebugLevel level){
	
	if (header != NULL) {
		Log.append(header);
	}
	Log.append(F("Id:")).append((unsigned int) Id);
	Log.append(F(";Type:")).append((char)Type);
	Log.append(F(";status:")).append((unsigned int)status);
	Log.append(F(";subscription:")).append(isSubscribed? "true" : "false");
	Log.append(F(" @"));
	Log.Log(level);
}
