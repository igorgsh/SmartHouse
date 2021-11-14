#pragma once
#include "ShiftRegister.h"
class ShiftRegisterOut :
    public ShiftRegister
{
public:
    void UnitLoop();
    void InitUnit();
    void FinalInitUnit();

    void Set(byte parentPin, bool status);
private:
    int nByte;
    byte* States;
    void SetBit(byte x, int n, bool v);
    void Out();
};

