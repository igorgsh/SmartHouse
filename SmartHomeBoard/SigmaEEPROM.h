// SigmaEEPROM.h

#ifndef _SIGMAEEPROM_h
#define _SIGMAEEPROM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
//#include "eeprom.h"
#include "Unit.h"
#include "Action.h"

class SigmaEEPROM  
{

 public:
	 static byte ReadBoardId();
	 static bool UpdateUnits(byte numberOfUnits, Unit** units);
	 static bool UpdateActions(byte numberOfActions, Action** actions);
	 static uint16_t Read16(uint16_t addr);
	 static byte Read8(uint16_t addr);
	 static void Write8(uint16_t addr, byte val);
	 static void Write16(uint16_t addr, uint16_t val);
	 static void ReadUnits();
	 static Unit* ReadUnit(int addr);
	 static void ReadActions();
	 static Action* ReadAction(int addr);

private:
	 static void WriteUnits(byte numberOfUnits, Unit** units);
	 static void WriteActions(byte numberOfActions, Action** actions);
	 //void WriteUnit(Unit* unit);
	 //Unit* ReadUnit(byte numberOfUnits, Unit* unit);

	 //Unit* ReadUnits(byte numberOfUnits, Unit** units);

	 // EEPROM structure
	 //	Byte | Object
	 //	-------------
	 //	0:	BoardId
	 static const byte addrBoardId = 0;
	 //	1: Number Of Units 
	 static const byte addrNumberUnits = 1;
	 //	2: Number of Actions
	 static const byte addrNumberActions = 2;
	 //	3: Address of Start of Units block
	 static const byte addrStartUnits = 10;
	 // 4-5: Address of Start of Actions block
	 static const uint16_t addrStartActions = 4;
	 // Unit structure
	//	Byte | Object
	//	-------------
	//	0: id
	//	2: type
	//	3-...: Unit-related data


	 // ???: Start of Actions block
};


#endif

