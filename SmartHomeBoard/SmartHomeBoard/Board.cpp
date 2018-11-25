#include <Arduino.h>
#include <avr\wdt.h>
#include "Board.h"
#include "Loger.h"


void Board::Reset(unsigned long timeout) {
	Loger::Debug("RESET!!!!");
	if (timeout > 4000) {
		delay(timeout - 4000);
	}
	wdt_enable(WDTO_4S);
}


