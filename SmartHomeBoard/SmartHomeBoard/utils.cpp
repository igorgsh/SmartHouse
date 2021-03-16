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

 void PrintIP(IPAddress addr, String& dest) {
	 dest+=addr[0];
	 dest += '.';
	 dest+=addr[1];
	 dest += '.';
	 dest+=addr[2];
	 dest += '.';
	 dest+=addr[3];
}


