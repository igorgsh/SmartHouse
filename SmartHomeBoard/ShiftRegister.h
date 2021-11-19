#pragma once
#include "Unit.h"

//              ,'{"id":207, "type":"S", "DPin":7, "latch":8, "clock":9, "PinsN":16}'
//  , '{"id":901, "type":"R","Pin":0, "lhOn":1, "status":0,"priority":10, "parentId":207, "parentPin":1}'

class ShiftRegister :
    public Unit
{
public:
    byte DPin;
    byte LatchPin;
    byte ClockPin;
    byte pinsNumber;
    bool Compare(const Unit* u);
    //virtual void SetDefault() = 0;
    virtual void UnitLoop(unsigned long timePeriod, bool isParent, bool val) = 0;
    virtual void InitUnit(bool isParent) = 0;
    virtual void FinalInitUnit(bool isParent) = 0;
 

    void ConfigField(const JsonObject& jsonList);
    byte UnitStoredSize() { return 7; }
    void ReadFromEEPROM(uint16_t addr);
    void WriteToEEPROM(uint16_t addr);
    void ProcessUnit(ActionType action) {};
protected:
    byte masks[8] = { 0b11111110,
                     0b11111101,
                     0b11111011,
                     0b11110111,
                     0b11101111,
                     0b11011110,
                     0b10111110,
                     0b01111110 };
};

