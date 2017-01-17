#include "rest_module.h"

#include "Arduino.h"
#include <ArduinoJson.h>

//#include "definitions.h"
#include "ext_global.h"
#include "button.h"
#include "light.h"
#include "relay.h"
#include "action.h"
#include "configuration.h"


const char* HTTP_CODE_200 = "200 Ok";
const char* HTTP_CODE_400 = (const char*)"400 Bad Request";

const char* REST_COMMAND_CONFIG = (const char*)"configuration";
const char* REST_COMMAND_HW = (const char*)"hardware";
const char* REST_COMMAND_ACTIONS = (const char*)"actions";
const char* REST_COMMAND_BUTTONS = (const char*)"buttons";
const char* REST_COMMAND_LIGHTS = (const char*)"lights";
const char* REST_COMMAND_RELAYS = (const char*)"relays";


/*
void SendPost(char * id, double oldValue, double newValue) {
	char str[100];
	sprintf(str, "Send Post: Unit ""%s"" changed value %u -> %u", id, (int)oldValue, (int)newValue);
	Serial.println(str);
}

void SendLog(char * id, double oldValue, double newValue) {
	char str[100];

	sprintf(str, "Log Record: Unit ""%s"" changed value %u -> %u", id, (int)oldValue, (int)newValue);
	Serial.println(str);
}
*/

void HttpHeader(Client& client, String error, String reason) {
	client.println("HTTP/1.1 " + error);
	client.println(F("Content-Type: application/json; charset=utf-8"));
	client.println(F("Connection: close"));  // the connection will be closed after completion of the response
											 
	client.println();

	if (reason != "") {
		StaticJsonBuffer<100> jsonBuffer;
		JsonObject& root = jsonBuffer.createObject();

		root[F("reason")] = reason;
		root.printTo(client);
	}
}


void ButtonToJson(JsonObject& root, ButtonUnit* btn) {

	if (btn != NULL) {
		root[F("unit")] = "button";
		root[F("id")] = btn->Id;
		root[F("pin")] = btn->Pin;
		root[F("lhOn")] = btn->lhOn;
	}

}

void JsonToButton(JsonObject& root, ButtonUnit* unit) {

	strcpy(unit->Id, root[F("id")]);
	if (root.containsKey(F("pin")))
		unit->Pin = root[F("pin")];
	if (root.containsKey(F("lhOn")))
		unit->lhOn = root[F("lhOn")];
}

void LightToJson(JsonObject& root, LightUnit* unit) {

	if (unit != NULL) {
		root[F("unit")] = F("light");
		root[F("id")] = unit->Id;
		root[F("pin")] = unit->Pin;
		root[F("dimValue")] = unit->dimValue;
		root[F("status")] = unit->status;

	}
}

void JsonToLight(JsonObject& root, LightUnit* unit) {

	strcpy(unit->Id, root[F("id")]);
	if (root.containsKey(F("pin")))
		unit->Pin = root[F("pin")];
	if (root.containsKey(F("dimValue")))
		unit->dimValue = root[F("dimValue")];
	if (root.containsKey(F("status")))
		unit->status = root[F("status")];
}


void RelayToJson(JsonObject& root, RelayUnit* unit) {

	if (unit != NULL) {
		root[F("unit")] = F("relay");
		root[F("id")] = unit->Id;
		root[F("pin")] = unit->Pin;
		root[F("lhOn")] = unit->lhOn;
		root[F("status")] = unit->status;

	}

}

void JsonToRelay(JsonObject& root, RelayUnit* unit) {

	strcpy(unit->Id, root[F("id")]);
	if (root.containsKey(F("pin")))
		unit->Pin = root[F("pin")];
	if (root.containsKey(F("lhOn")))
		unit->lhOn = root[F("lhOn")];
	if (root.containsKey(F("status")))
		unit->status = root[F("status")];
}

void ActionToJson(JsonObject& root, ActionUnit* action) {

	if (action != NULL) {
		root[F("unit")] = F("action");
		root[F("id")] = action->Id;
		root[F("event")] = action->event;
		root[F("action")] = (byte)action->action;
		root[F("originId")] = action->originId;
		root[F("targetId")] = action->targetId;

	}
}


void JsonToAction(JsonObject& root, ActionUnit* unit) {

	strcpy(unit->Id, root[F("id")]);
	if (root.containsKey(F("event")))
		unit->event = root[F("event")];
	if (root.containsKey(F("action")))
		unit->action = (Action)(byte)root[F("action")];
	if (root.containsKey(F("originId")))
		strcpy(unit->originId, root[F("originId")]);
	if (root.containsKey(F("targetId")))
		strcpy(unit->originId, root[F("targetId")]);
}


void ListUnits(Client& client, String part, String unitId) {

	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

	if (part.equals(REST_COMMAND_BUTTONS)) {
		if (unitId[0] != 0) {
			ButtonUnit* btn = FindButton(unitId.c_str());
			ButtonToJson(root, btn);
		}
		else { //All units
			JsonArray& unitArray = root.createNestedArray(REST_COMMAND_BUTTONS);

			for (int i = 0; i < NUMBER_OF_BUTTONS && &(Buttons[i]) != NULL && Buttons[i].Id[0] != 0; i++) {
				JsonObject& unitData = unitArray.createNestedObject();
				ButtonToJson(unitData, &(Buttons[i]));
			}
		}
	}
	else if (part.equals(REST_COMMAND_LIGHTS)) {
		if (unitId[0] != 0) {
			LightUnit* lgt = FindLight(unitId.c_str());
			LightToJson(root, lgt);
		}
		else { //All units
			JsonArray& unitArray = root.createNestedArray(REST_COMMAND_LIGHTS);

			for (int i = 0; i < NUMBER_OF_LIGHTS && &(Lights[i]) != NULL && Lights[i].Id[0] != 0; i++) {
				JsonObject& unitData = unitArray.createNestedObject();
				LightToJson(unitData, &(Lights[i]));
			}
			Debug("End");
		}

	}
	else if (part.equals(REST_COMMAND_RELAYS)) {
		if (unitId[0] != 0) {
			RelayUnit* rel = FindRelay(unitId.c_str());
			RelayToJson(root, rel);
		}
		else { //All units
			JsonArray& unitArray = root.createNestedArray(REST_COMMAND_RELAYS);

			for (int i = 0; i < NUMBER_OF_RELAYS && &(Relays[i]) != NULL && Relays[i].Id[0] != 0; i++) {
				JsonObject& unitData = unitArray.createNestedObject();
				RelayToJson(unitData, &(Relays[i]));
			}
		}
	}
	else if (part.equals(REST_COMMAND_ACTIONS)) {
		if (unitId[0] != 0) {
			ActionUnit* act = FindAction(unitId.c_str());
			ActionToJson(root, act);
		}
		else { //All units
			JsonArray& unitArray = root.createNestedArray(REST_COMMAND_ACTIONS);

			for (int i = 0; i < NUMBER_OF_ACTIONS && &(Actions[i]) != NULL && Actions[i].Id[0] != 0; i++) {
				JsonObject& unitData = unitArray.createNestedObject();
				ActionToJson(unitData, &(Actions[i]));
			}
		}
	}
	HttpHeader(client, HTTP_CODE_200, "");
	root.printTo(client);


}


#define HTTP_END_OF_LINE	0x0a

int ProcessRequest(Client& client, HttpRequest* request) {
	String line;


	// Read type of request
	line = client.readStringUntil('/');
	if (line.startsWith("GET")) {
		request->type = GET;
	}
	else if (line.startsWith("POST")) {
		request->type = POST;
	}
	else {
		return -1; //Unsupported type
	}
	// Read URL
	line = client.readStringUntil(' ');
	Serial.println(line);
	request->URL = line;

	// skip HTTP/1.1
	line = client.readStringUntil(HTTP_END_OF_LINE);

	//Read parameters of header
	while (client.available()) {
		line = client.readStringUntil(HTTP_END_OF_LINE);
		/*
				Serial.print(line);
				Serial.print("#");
				Serial.print(line.length(), DEC);
				Serial.print("#");
				Serial.print(line[0], HEX);
				Serial.println("#");
				*/
		if (line.length() == 1 && line[0] == 0x0D) {// Empty line. The body will be next section
			request->body = client.readString();
		}
		else {
			if (line.startsWith(F("Host: "))) {
				request->host = line.substring(strlen("Host: "));
			}
		}
	}
	/*
	Serial.println("Result:");
	Serial.println((request.type == GET ? "Type: GET" : "Type: POST"));
	Serial.print("URL:");
	Serial.println(request.URL);
	Serial.print("Host:");
	Serial.println(request.host);
	Serial.print("Body:");
	Serial.println(request.body);
	*/
	return 0;
}



void UpdateUnit(Client& client, String part, String unitId, String json) {

	//StaticJsonBuffer<200> jsonBuffer;
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.parse(json);
	bool isNew = false;

	if (!root.success()) {
		HttpHeader(client, HTTP_CODE_400, F("Can't parse JSON"));
		return;
	}

	if (part.equals(REST_COMMAND_BUTTONS)) {
		if (unitId != "") { //update of existing object
			ButtonUnit *unit = FindButton(unitId.c_str());

			if (unit == NULL) {//Not Found. Create a new one
				unit = FindButton("");
				if (unit == NULL) {
					HttpHeader(client, HTTP_CODE_400, F("Not enough space to store new button"));
					return;
				}
				isNew = true;
			}
			if (root[F("id")] != "") {
				if (isNew) {
					DefaultButtonValue(unit);
				}
				JsonToButton(root, unit);
				UpdateConfiguration(unit);
			}
			else {
				HttpHeader(client, HTTP_CODE_400, F("Id is absent"));
			}
		}
	}
	else if (part.equals(REST_COMMAND_LIGHTS)) {
		if (unitId != "") { //update of existing object
			LightUnit *unit = FindLight(unitId.c_str());

			if (unit == NULL) {//Not Found. Create a new one
				unit = FindLight("");
				if (unit == NULL) {
					HttpHeader(client, HTTP_CODE_400, F("Not enough space to store new light"));
					return;
				}
				isNew = true;
			}
			if (root[F("id")] != "") {
				if (isNew) {
					DefaultLightValue(unit);
				}
				JsonToLight(root, unit);
			}
			else {
				HttpHeader(client, HTTP_CODE_400, F("Id is absent"));
			}
		}
	}
	else if (part.equals(REST_COMMAND_RELAYS)) {
		if (unitId != "") { //update of existing object
			RelayUnit *unit = FindRelay(unitId.c_str());

			if (unit == NULL) {//Not Found. Create a new one
				unit = FindRelay("");
				if (unit == NULL) {
					HttpHeader(client, HTTP_CODE_400, F("Not enough space to store new relay"));
					return;
				}
				isNew = true;
			}
			if (root[F("id")] != "") {
				if (isNew) {
					DefaultRelayValue(unit);
				}
				JsonToRelay(root, unit);
			}
			else {
				HttpHeader(client, HTTP_CODE_400, F("Id is absent"));
			}
		}
	}
	else if (part.equals(REST_COMMAND_ACTIONS)) {
		if (unitId != "") { //update of existing object
			ActionUnit *unit = FindAction(unitId.c_str());

			if (unit == NULL) {//Not Found. Create a new one
				unit = FindAction("");
				if (unit == NULL) {
					HttpHeader(client, HTTP_CODE_400, F("Not enough space to store new action"));
					return;
				}
				isNew = true;
			}
			if (root[F("id")] != "") {
				if (isNew) {
					DefaultActionValue(unit);
				}
				JsonToAction(root, unit);
			}
			else {
				HttpHeader(client, HTTP_CODE_400, F("Id is absent"));
			}
		}
	}

	HttpHeader(client, HTTP_CODE_200, "");


}



void ParseCommand(Client& client, HttpRequest request) {
	//Next commands accepted
	//GET <empty> - dummy request - OK response
	//GET zzz/xx/yy, where
	// zzz is hardware | actions
	// xx is enmpty for actions and for hardware are:
	//		buttons
	//		lights
	//		relay
	// And yy is id of device (optional)
	String commands[NUMBER_OF_URL_PARTS];

	if (request.URL.length() == 0) {// empty command
		commands[0] = "";
	}
	else {
		int ind;
		String line = request.URL;

		for (int i = 0; i < NUMBER_OF_URL_PARTS; i++) {
			ind = line.indexOf('/');
			if (ind != -1) {
				commands[i] = line.substring(0, ind);
				line = line.substring(ind + 1);
			}
			else {
				commands[i] = line;
				commands[i + 1] = "";
				break;
			}
		}
	}

	if (request.type == GET) {
		if (commands[0] == "") {// empty command
			HttpHeader(client, HTTP_CODE_200, "");
			return;
		}
		else if (commands[0].equals(REST_COMMAND_HW) || commands[0].equals(REST_COMMAND_ACTIONS)) {
			ListUnits(client, commands[1], commands[2]);
			return;
		}
		else {
			HttpHeader(client, HTTP_CODE_400, F("Unknown command"));
		}

	}
	else if (request.type == POST) {
		if (request.URL.length() == 0) {// empty command
			HttpHeader(client, HTTP_CODE_400, F("Non-supported POST with empty URL"));
			return;
		}
		else if (commands[0].equals(REST_COMMAND_ACTIONS) || commands[0].equals(REST_COMMAND_HW)) {
			UpdateUnit(client, commands[1], commands[2], request.body);
			return;
		}
		else {
			HttpHeader(client, HTTP_CODE_400, F("Unknown command"));
		}

	}
	HttpHeader(client, HTTP_CODE_400, F("Just GET and POST supported"));
}



void ProcessServerRequests() {
	client = server.available();
	while (client.available()) {
		HttpRequest request;

		if (ProcessRequest(client, &request) == 0) {
			ParseCommand(client, request);
		}
		/*
		else { //Error. Bad Request
			HttpHeader(client, "418 I'm teapot", "Something wrong");
		}
		*/
		client.stop();
		Debug("Client disconnected");
		delay(1);
	}


}
