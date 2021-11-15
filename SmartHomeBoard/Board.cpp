#include <Arduino.h>
#include <avr\wdt.h>
#include "Board.h"
#include "Loger.h"
#include "Configuration.h"

extern Configuration Config;

void Board::Reset(unsigned long timeout) {
	Config.Log->Info(F1("RESET!!!"));
	if (timeout > 4000) {
		delay(timeout - 4000);
	}
	wdt_enable(WDTO_4S);
}


