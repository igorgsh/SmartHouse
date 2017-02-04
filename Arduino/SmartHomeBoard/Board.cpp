#include <Arduino.h>
#include <avr\wdt.h>
#include "Board.h"


void Board::Reset(unsigned long timeout) {
	if (timeout > 4000) {
		delay(timeout - 4000);
	}
	wdt_enable(WDTO_4S);
}


