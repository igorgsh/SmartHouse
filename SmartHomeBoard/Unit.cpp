#include "Unit.h"
#include "Configuration.h"

extern Configuration Config;

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
		Config.Log->append(header);
	}
	Config.Log->append(F1("Id:")).append((unsigned int) Id);
	Config.Log->append(F1(";Type:")).append((char)Type);
	Config.Log->append(F1(";status:")).append((unsigned int)status);
	Config.Log->append(F1(" @"));
	Config.Log->Log(level);
}
