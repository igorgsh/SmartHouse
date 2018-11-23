// SigmaEEPROM.h

#ifndef _SIGMAEEPROM_h
#define _SIGMAEEPROM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "eeprom.h"
#include "Unit.h"

class SigmaEEPROM  
{

 public:
	 static byte ReadBoardId();
	 static bool UpdateUnits(byte numberOfUnits, Unit** units);
	 static uint16_t Read16(uint16_t addr);
	 static byte Read8(uint16_t addr);
	 static void Write8(uint16_t addr, byte val);
	 static void Write16(uint16_t addr, uint16_t val);

private:
	 static void WriteUnits(byte numberOfUnits, Unit** units);
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
	//	3-...: Start of Units block
	 static const byte addrStartUnits = 3;
	// Unit structure
	//	Byte | Object
	//	-------------
	//	0-1: id
	//	2: type
	//	3-...: Unit-related data


	 // ???: Start of Actions block
	 uint16_t addrStartActions = 0;
};


#endif

