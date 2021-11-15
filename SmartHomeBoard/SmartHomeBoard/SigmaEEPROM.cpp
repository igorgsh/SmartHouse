// 
// 
// 

#include "SigmaEEPROM.h"
#include "Configuration.h"
#include "eeprom.h"
#include "Board.h"

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
			byte uType = Read8(currentPtr+1);
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
				delete u;
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

		Unit* u = Config.CreateTypedUnit(units[i]->Type);
		u->ReadFromEEPROM(currentPtr);
//		u->print("Reading unit:", D_DEBUG);
		delete u;

		currentPtr += units[i]->UnitStoredSize();
	}

	Write8(addrNumberUnits, numberOfUnits);
	Write16(addrStartActions, currentPtr);
}

void SigmaEEPROM::ReadUnits() {

	byte nUnits = Read8(addrNumberUnits);

	Unit** units = Config.CreateUnits(nUnits);

	int currPtr = addrStartUnits;
	for (int i = 0; i < nUnits; i++) {
		//Loger::Debug("Curptr=" + String(currPtr));
		units[i] = ReadUnit(currPtr);
//		units[i]->print("Read Units:",D_DEBUG);
		currPtr += units[i]->UnitStoredSize();
	}
}

Unit* SigmaEEPROM::ReadUnit(int curPtr) {

	byte tp = Read8(curPtr + 1);
	Unit* u = NULL;
	u = Config.CreateTypedUnit(tp);
	if (u != NULL) {
		u->ReadFromEEPROM(curPtr);
	}
	else {
		//Board::Reset(10000);
	}
	return u;
}


Action* SigmaEEPROM::ReadAction(int curPtr) {

	Action* a = NULL;
	a = new Action();
	if (a != NULL) {
		a->ReadFromEEPROM(curPtr);
	}
	else {
		//Board::Reset(10000);
	}
	return a;
}


void SigmaEEPROM::ReadActions() {

	byte nActions = Read8(addrNumberActions);

	Action** actions = Config.CreateActions(nActions);

	int currPtr = Read16(addrStartActions);
	for (int i = 0; i < nActions; i++) {
		//Loger::Debug("Curptr=" + String(currPtr));
		actions[i] = ReadAction(currPtr);
//		actions[i]->print("Read Actions:", D_DEBUG);
		currPtr += actions[i]->ActionStoredSize();
	}
}


bool SigmaEEPROM::UpdateActions(byte numberOfActions, Action** actions) {
	
	bool res = false;
	byte nActions = Read8(addrNumberActions);

	if (nActions != numberOfActions) {
		res = true;
	}
	else {
		uint16_t currentPtr = Read16(addrStartActions);

		for (int i = 0; !res && i < numberOfActions; i++) {
			Action* a = new Action;
			a->ReadFromEEPROM(currentPtr);
//			a->print("Read action:", D_DEBUG);
			if (!a->Compare(actions[i])) {
				res = true;
			}
			else {
				res = false;
				currentPtr += a->ActionStoredSize();
			}
			delete a;
		}
	}

	if (res) {
		SigmaEEPROM::WriteActions(numberOfActions, actions);
	}
	return res;
}

void SigmaEEPROM::WriteActions(byte numberOfActions, Action** actions) {
	uint16_t currentPtr = Read16(addrStartActions);
	for (int i = 0; i < numberOfActions; i++) {
		actions[i]->WriteToEEPROM(currentPtr);
		currentPtr += actions[i]->ActionStoredSize();
	}

	Write8(addrNumberActions, numberOfActions);
	//Write16(addrStartActions, currentPtr);
}
