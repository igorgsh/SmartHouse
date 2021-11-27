#pragma once
#include "ShiftRegister.h"

class ShiftRegisterOut :
    public ShiftRegister
{
public:
    void UnitLoop(unsigned long timePeriod, bool isParent, bool val);
    void InitUnit(bool isParent);
    void FinalInitUnit(bool isParent);

    void Set(byte parentPin, bool status);
private:
    bool* ChildStates;
//    byte SetBit(byte x, int n, bool v);
    void Out();
};

