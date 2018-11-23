// 
// 
// 

#include "SigmaEEPROM.h"
#include "Configuration.h"

extern Configuration Config;

uint16_t SigmaEEPROM::Read16(uint16_t addr) {
	uint16_t res;
	res = (((uint16_t)EEPROM.read(addr)) << 8 & 0xFF00) + EEPROM.read(addr + 1);
	return res;
}

byte SigmaEEPROM::Read8(uint16_t addr) {
	return EEPROM.read(addr);
}

void SigmaEEPROM::Write16(uint16_t addr, uint16_t val) {
	EEPROM.write(addr, (byte)((val >> 8) & 0x00FF));
	EEPROM.write(addr + 1, (byte)((val & 0x00FF)));

}
void SigmaEEPROM::Write8(uint16_t addr, byte val) {
	EEPROM.write(addr, val);
}

byte SigmaEEPROM::ReadBoardId() {
	return Read8(addrBoardId);
}

bool SigmaEEPROM::UpdateUnits(byte numberOfUnits, Unit** units) {
	bool res = false;
	byte nUnits = Read8(addrNumberUnits);

	if (nUnits != numberOfUnits) {
		res = true;
	}
	else {
		uint16_t currentPtr = addrStartUnits;
		
		for (int i = 0; !res && i < numberOfUnits; i++) {
			byte uId = Read8(currentPtr);
			byte uType = Read8(currentPtr);
			if (uId != units[i]->Id || uType != units[i]->Type) {
				res = true;
			}
			else {
				Unit* u = Config.CreateTypedUnit(uType);
				u->Id = uId;
				u->ReadFromEEPROM(currentPtr);
				if (!u->Compare(units[i])) {
					res = true;
				}
				else {
					res = false;
					currentPtr += u->UnitStoredSize();
				}

			}
		}
	}
	if (res) {
		SigmaEEPROM::WriteUnits(numberOfUnits, units);
	}
	return res;
}

void SigmaEEPROM::WriteUnits(byte numberOfUnits, Unit** units) {
	uint16_t currentPtr = addrStartUnits;

	for (int i = 0; i < numberOfUnits; i++) {
		units[i]->WriteToEEPROM(currentPtr);
		currentPtr += units[i]->UnitStoredSize();
	}

	Write8(addrNumberUnits, numberOfUnits);
}
