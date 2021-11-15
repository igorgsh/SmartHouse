#pragma once
#include "ShiftRegister.h"
class ShiftRegisterIn :
    public ShiftRegister
{
public:
    void InitUnit();
    void UnitLoop();
};

