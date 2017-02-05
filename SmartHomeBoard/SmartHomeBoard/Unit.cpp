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

void Unit::print(const char* header, Stream& stream) {
	if (header != NULL) {
		stream.print(header);
	}
	stream.print("Id:");
	stream.print(Id, HEX);
	stream.print(";Type:");
	stream.print(Type, HEX);
	stream.print(";Pin:");
	stream.print(Pin, DEC);
	stream.print(";lhOn:");
	stream.print(lhOn, HEX);
	stream.print(";status:");
	stream.print((unsigned int)status, DEC);
	stream.println(" @");
}
