#include <Arduino.h>
#include <avr\wdt.h>
#include "Board.h"
#include "Loger.h"
#include "Configuration.h"

extern Configuration Config;

void Board::Reset(unsigned long timeout) {
<<<<<<< HEAD
	Loger::LogMessage = F("RESET!!!!");
	Loger::Debug();
=======
	Config.Log->Info(F1("RESET!!!"));
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
	if (timeout > 4000) {
		delay(timeout - 4000);
	}
	
	wdt_enable(WDTO_4S);
}


