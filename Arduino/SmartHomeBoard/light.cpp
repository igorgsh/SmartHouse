#include "light.h"
#include "process.h"
#include "ext_global.h"
#include "mqtt.h"
/*
void DefaultLightValue(LightUnit* light) {
	light->dimValue = 1;
	light->status = LOW;

}

LightUnit* FindLight(const char* id) {

	LightUnit *unit = NULL;

	for (int i = 0; i < NUMBER_OF_LIGHTS && &(Lights[i]) != NULL; i++) {
		if (strcmp(Lights[i].Id, id) == 0) {
			unit = &(Lights[i]);
			break;
		}
		//if (Lights[i].Id[0] == 0)
		//	break;
	}
	return unit;
}

void LightSet(char * id, bool isOn) {
	LightUnit *unit = FindLight(id);

	//Debug((isOn ? "Light ON" : "Light Off"));

	if (unit != NULL) {
		if (unit->status != isOn) {
			unit->status = isOn;
			digitalWrite(unit->Pin, isOn);
			PublishLight(*unit);
			ProcessAction(unit->Id, isOn, isOn, unit->status);
		}
	}
}

void LightSwitch(char * id) {
	LightUnit *unit = FindLight(id);

	if (unit != NULL) {
		LightSet(id, !(unit->status));
	}
}

void LightDim(char * id) {
	
	char buf[50];

	LightUnit *unit = FindLight(id);

	Debug("DIM");
	if (unit != NULL) {
		unit->dimValue++;
		if (unit->dimValue > MAX_DIM_VALUE) {
			unit->dimValue = 1;
		}
		sprintf(buf, "Updated Dim_Value:%u",unit->dimValue);
		Debug(buf);
		for (int i = 0; i < unit->dimValue; i++) {
			sprintf(buf, "Write HIGH into:%u", (unit->Pin + i));
			Debug(buf);
			digitalWrite(unit->Pin + i, HIGH);
		}
		
		for (int i = unit->dimValue; i < MAX_DIM_VALUE; i++) {
			sprintf(buf, "Write LOW into:%u", (unit->Pin + i));
			Debug(buf);
			digitalWrite(unit->Pin + i, LOW);
		}
	}

}
*/