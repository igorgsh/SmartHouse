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

void SigmaEEPROM::ReadBoardId() {
	byte v= Read8(addrBoardId);
	Config.BoardId = v;
	Config.ip[0] = Read8(addrBoardIp);
	Config.ip[1] = Read8(addrBoardIp+1);
	Config.ip[2] = Read8(addrBoardIp+2);
	Config.ip[3] = Read8(addrBoardIp+3);

	Config.mqttIp[3] = Read8(addrMqttIp);

}

bool SigmaEEPROM::UpdateUnits(byte numberOfUnits, Unit** units) {
	bool res = false;
	byte nUnits = Read8(addrNumberUnits);

	Config.Log->Debug("Update Units");
	Config.Log->append("numberOfUnits=").append(numberOfUnits).append(" nUnits=").append(nUnits).Debug();
	if (nUnits != numberOfUnits) {
		res = true;
	}
	else {
		uint16_t currentPtr = addrStartUnits;
		
		for (int i = 0; !res && i < numberOfUnits; i++) {
			uint16_t uId = Read16(currentPtr);
			byte uType = Read8(currentPtr+2);
			Config.Log->append("uid=").append(uId).append("; uType=").append(uType).Debug();
			Config.Log->append("Unitid=").append(units[i]->Id).append("; UnitType=").append(units[i]->Type).Debug();
			if (uId != units[i]->Id || uType != units[i]->Type) {
				Config.Log->Debug("Point 2");
				res = true;
			}
			else {
				Config.Log->Debug("Point 5");
				Config.Log->append("i=").append(i).Debug();
				Unit* u = Config.CreateTypedUnit(uType);
				u->Id = uId;
				u->ReadFromEEPROM(currentPtr);
				if (!u->Compare(units[i])) {
					Config.Log->Debug("Point 7");
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
		Config.Log->Debug("Point 3");
		SigmaEEPROM::WriteUnits(numberOfUnits, units);
	}
	return res;
}

void SigmaEEPROM::WriteUnits(byte numberOfUnits, Unit** units) {
	uint16_t currentPtr = addrStartUnits;
	for (int i = 0; i < numberOfUnits; i++) {
		Config.Log->append("currptr=").append(currentPtr).append("; UType=").append(units[i]->Type).Debug();
		units[i]->WriteToEEPROM(currentPtr);

		//Unit* u = Config.CreateTypedUnit(units[i]->Type);
		//u->ReadFromEEPROM(currentPtr);
		//u->print("Read from eeprom ",D_DEBUG);

		currentPtr += units[i]->UnitStoredSize();

	}
	Write8(addrNumberUnits, numberOfUnits);
	Write16(addrStartActions, currentPtr);
}

void SigmaEEPROM::ReadUnits() {

	byte nUnits = Read8(addrNumberUnits);
	Config.Log->append("Read Units:").append(nUnits).Debug();
	
	Unit** units = Config.CreateUnits(nUnits);
	int currPtr = addrStartUnits;
	for (int i = 0; i < nUnits; i++) {
		units[i] = ReadUnit(currPtr);
		currPtr += units[i]->UnitStoredSize();
	}
}

Unit* SigmaEEPROM::ReadUnit(int curPtr) {

	byte tp = Read8(curPtr + 2);
	Unit* u = NULL;
	u = Config.CreateTypedUnit(tp);
	if (u != NULL) {
		u->ReadFromEEPROM(curPtr);
	}
	return u;
}


Action* SigmaEEPROM::ReadAction(int curPtr) {

	Action* a = NULL;
	a = new Action();
	if (a != NULL) {
		a->ReadFromEEPROM(curPtr);
	}
	return a;
}


void SigmaEEPROM::ReadActions() {

	byte nActions = Read8(addrNumberActions);

	Action** actions = Config.CreateActions(nActions);

	int currPtr = Read16(addrStartActions);
	for (int i = 0; i < nActions; i++) {
		actions[i] = ReadAction(currPtr);
		currPtr += actions[i]->ActionStoredSize();
		actions[i]->print("Action from EEPROM:", D_DEBUG);
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
