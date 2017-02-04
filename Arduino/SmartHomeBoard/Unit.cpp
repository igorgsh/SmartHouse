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