#pragma once

typedef enum {
	D_OFF = 0,
	D_FATAL = 1,
	D_ERROR = 2,
	D_WARN = 3,
	D_INFO = 4,
	D_DEBUG = 5,
	D_ALL = 6
} DebugLevel;

extern DebugLevel dLevel;

#define SerialLog2(LEVEL, X, Y) {if (LEVEL <= dLevel) {Serial.println((X),Y);}};
#define SerialLog2_(LEVEL, X, Y) {if (LEVEL <= dLevel) {Serial.print((X),Y);}};
#define SerialLog(LEVEL, X) {if (LEVEL <= dLevel) {Serial.println((X));}};
#define SerialLog_(LEVEL,X) {if (LEVEL <= dLevel) {Serial.print((X));}};
#define Debug(X)	SerialLog(D_DEBUG, X)
#define Debug_(X)	SerialLog_(D_DEBUG, X)
#define Debug2(X,Y)	SerialLog_(D_DEBUG,X); SerialLog(D_DEBUG, Y);
#define Debug3(X,Y,Z)	SerialLog_(D_DEBUG,X); SerialLog2(D_DEBUG, Y, Z);
