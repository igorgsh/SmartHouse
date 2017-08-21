// 
// 
// 

#include "utils.h"
// ����������, ����������� ��������� ������,
// ����� ������������� �����
extern int __bss_end;
extern void *__brkval;

// �������, ������������ ���������� ���������� ��� (RAM)
int memoryFree()
{
	int freeValue;
	if ((int)__brkval == 0)
		freeValue = ((int)&freeValue) - ((int)&__bss_end);
	else
		freeValue = ((int)&freeValue) - ((int)__brkval);
	return freeValue;
}


