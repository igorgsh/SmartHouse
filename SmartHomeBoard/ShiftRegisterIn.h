#pragma once
#include "ShiftRegister.h"
class ShiftRegisterIn :
    public ShiftRegister
{
public:
    void UnitLoop();
    void InitUnit();
    void FinalInitUnit();

private:
    int nByte;
    Unit** children;
    int childrenCnt = 0;

};

