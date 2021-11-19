#pragma once
#include "ShiftRegister.h"
class ShiftRegisterIn :
    public ShiftRegister
{
public:
    void UnitLoop(unsigned long timePeriod, bool isParent, bool val);
    void InitUnit(bool isParent);
    void FinalInitUnit(bool isParent);

private:
    int nByte;
    Unit** children;
    int childrenCnt = 0;

};

