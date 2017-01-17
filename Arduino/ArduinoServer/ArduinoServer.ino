/*
 Name:		ArduinoServer.ino
 Created:	14.01.17 19:02:33
 Author:	Igor Shevchenko
*/

// the setup function runs once when you press reset or power the board
#include <ArduinoJson.h>
#include <EthernetUdp.h>
#include <EthernetServer.h>
#include <EthernetClient.h>
#include <Ethernet.h>
#include <Dns.h>
#include <Dhcp.h>

#include "types.h"


byte mac[] = {
	0x00, 0xAA, 0x22, 0x07, 0x19, 0x69
};
// IP address in case DHCP fails
IPAddress ip(192, 168, 0, 102);

byte serverPort = 80;

//EthernetClient client;
EthernetServer server(serverPort);
EthernetClient client;

#define UNIT_NUMBER	20

ButtonUnit Buttons[UNIT_NUMBER] = { {"Btn0", 10, 0,1},{ "Btn1", 20, 2,3 },{ "Btn2", 30, 0,1 } };
LightUnit Lights[UNIT_NUMBER] = { {"Lgt0", 5, 0, 1},{ "Lgt1", 6, 0, 1 }, {"Lgt2", 7, 0, 1} };
RelayUnit Relays[UNIT_NUMBER] = { { "Rel0", 5, 0, 1 },{ "Rel1", 6, 0, 1 },{ "Rel2", 7, 0, 1 } };

typedef enum {
	GET = 0,
	POST = 1
} RequestType;

typedef struct {
	RequestType type;
	String URL;
	String host;
	String body;
} HttpRequest;

void setup() {
	Serial.begin(115200);

	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP");
		// no point in carrying on, so do nothing forevermore:
		// try to congifure using IP address instead of DHCP:
		Ethernet.begin(mac, ip);
	}
	server.begin();
	Serial.print("Server is at ");
	Serial.println(Ethernet.localIP());

}
HttpRequest request;

void HttpHeader(Client& client, String error, String reason);
int ProcessRequest(Client& client);
void ParseCommand(Client& client);

// the loop function runs over and over again until power down or reset
void loop() {
	client = server.available();
	while (client.available()) {
		if (ProcessRequest(client) == 0) {
			ParseCommand(client);
		} else { //Error. Bad Request
			HttpHeader(client, "418 I'm teapot", "Something wrong");
		}
		client.stop();
		Serial.println("Client disconnected");
		delay(1);
	}
}

ButtonUnit* FindButton(const char* id) {

	ButtonUnit *unit = NULL;

	for (int i = 0; i < UNIT_NUMBER && &(Buttons[i]) != NULL /*&& Buttons[i].Id[0] != 0*/; i++) {
		if (strcmp(Buttons[i].Id, id) == 0) {
			unit = &(Buttons[i]);
			break;
		}
	}
	return unit;

}

void JsonButton(JsonObject& root, ButtonUnit* btn) {

	if (btn != NULL) {
		root["unit"] = "button";
		root["id"] = btn->Id;
		root["pin"] = btn->Pin;
	}

}

void ListHardware(Client& client, String part, String unitId) {

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();

	if (part == "buttons") {
		if (unitId[0] != 0) {
			ButtonUnit* btn = FindButton(unitId.c_str());
			JsonButton(root, btn);
		} else { //All units
			JsonArray& unitArray = root.createNestedArray("units");
			
			for (int i = 0; i < UNIT_NUMBER && &(Buttons[i]) != NULL && Buttons[i].Id[0] != 0; i++) {
				Serial.print("B[i]=");
				Serial.println(Buttons[i].Id);
				root.prettyPrintTo(Serial);
				JsonObject& unitData = unitArray.createNestedObject();
				JsonButton(unitData, &(Buttons[i]));
			}
		}
	}
	HttpHeader(client, "200 Ok", "");
	root.prettyPrintTo(Serial);
	root.printTo(client);
	

}





#define NUMBER_OF_PARTS	5


void UpdateHardware(Client& client, String part, String unitId, String json) {
	//StaticJsonBuffer<200> jsonBuffer;
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.parse(json);

	if (!root.success()) {
		HttpHeader(client, "400 Bad request", "Can't parse JSON");
		return;
	} 
	Serial.println("Point 1");

	if (part.equals("buttons")) {
		if (unitId != "") { //update of existing object
			ButtonUnit *btn = FindButton(unitId.c_str());
			Serial.println("Point 2");
			if (btn == NULL) {//Not Found. Create a new one
				btn = FindButton("");
				if (btn == NULL) {
					HttpHeader(client, "400 Bad request", "Not enough space");
					return;
				}
				Serial.println("Point 3");

			}
			strcpy(btn->Id, root["id"]);
			btn->Pin = root["pin"];
			Serial.print("Id=");
			Serial.println(btn->Id);
			Serial.print("Pin=");
			Serial.println(btn->Pin, DEC);

		}
		else { // empty id. Array inside
			if (root.containsKey("units")) {
				Serial.print("Size=");
				Serial.println(root["units"].size());
				for (unsigned int i = 0; i < root["units"].size(); i++) {

					ButtonUnit* btn;
					//String id = root["units"[i]["id"]];
					btn = FindButton(root["units"][i]["id"]);
					if (btn == NULL) {
						Serial.println("Point 4");
						btn = FindButton("");
					}
					if (btn == NULL) {
						HttpHeader(client, "400 Bad request","No empty space for unit");
						return;
					}
					
					strcpy(btn->Id, root["units"][i]["id"]);
					btn->Pin = root["units"][i]["pin"];
					Serial.print("Id=");
					Serial.println(btn->Id);
					Serial.print("Pin=");
					Serial.println(btn->Pin, DEC);
					
				}
			}
			else {
				HttpHeader(client, "400 Bad request", "Bad formed POST body");
				return;
			}
		}
	}
	HttpHeader(client, "200 Ok", "");


}


void ParseCommand(Client& client) {
	//Next commands accepted
	//GET <empty> - dummy request - OK response
	//GET hardware/xx/yy, where xx is
	//		buttons
	//		lights
	//		relay
	// And yy is id of device (optional)
	String commands[NUMBER_OF_PARTS];

	if (request.URL.length() == 0) {// empty command
		commands[0] = "";
	}
	else {
		int ind;
		String line = request.URL;

		for (int i = 0; i < NUMBER_OF_PARTS; i++) {
			ind = line.indexOf('/');
			if (ind != -1) {
				commands[i] = line.substring(0, ind);
				line = line.substring(ind + 1);
				Serial.print("Command=");
				Serial.println(commands[i]);
				Serial.print("line=");
				Serial.println(line);
			}
			else {
				commands[i] = line;
				commands[i + 1] = "";
				break;
			}
		}
	}

	if (request.type == GET) {
		if (commands[0][0]==0) {// empty command
			HttpHeader(client, "200 OK", "");
			return;
		} else if (commands[0].equals("hardware")) {
			ListHardware(client, commands[1], commands[2]);
			return;
		}
	}
	else if (request.type == POST) {
		if (request.URL.length() == 0) {// empty command
			HttpHeader(client, "400 Bad request", "Non-supported POST with empty URL");
			return;
		}
		else if (commands[0].equals("hardware")) {
			UpdateHardware(client, commands[1],commands[2],request.body);
			return;
		}

	}
	HttpHeader(client, "400 Bad Request", "Just GET and POST supported");
}

int ProcessRequest(Client& client) {
	String line;
	

	// Read type of request
	line = client.readStringUntil('/');
	Serial.println(line);
	if (line.startsWith("GET")) {
		request.type = GET;
	}
	else if (line.startsWith("POST")) {
		request.type = POST;
	}
	else {
		return -1; //Unsupported type
	}
	// Read URL
	line = client.readStringUntil(' ');
	Serial.println(line);
	request.URL = line;

	// skip HTTP/1.1
	line = client.readStringUntil(0x0a);
	Serial.println(line);

	//Read parameters of header
	while (client.available()) {
		line = client.readStringUntil(0x0a);
		Serial.print(line);
		Serial.print("#");
		Serial.print(line.length(), DEC);
		Serial.print("#");
		Serial.print(line[0], HEX);
		Serial.println("#");

		if (line.length() == 1 && line[0]==0x0D) {// Empty line. The body will be next
			request.body = client.readString();
		}
		else {
			if (line.startsWith("Host: ")) {
				request.host = line.substring(6);
			}
		}
	}
	Serial.println("Result:");
	Serial.println((request.type==GET? "Type: GET" : "Type: POST")  );
	Serial.print("URL:");
	Serial.println(request.URL);
	Serial.print("Host:");
	Serial.println(request.host);
	Serial.print("Body:");
	Serial.println(request.body);

	return 0;
}
/*
void HttpResponse(Client& client, String error) {
	client.println("HTTP/1.1 " + error);
	client.println("Content-Type: text/html");
	client.println("Connection: close");  // the connection will be closed after completion of the response
	//client.println("Refresh: 5");  // refresh the page automatically every 5 sec
	client.println();
	client.println("<!DOCTYPE HTML>");
	client.println("<html></html>");
}
*/
void HttpHeader(Client& client, String error, String reason) {
	client.println("HTTP/1.1 " + error);
	client.println("Content-Type: application/json; charset=utf-8");
	client.println("Connection: close");  // the connection will be closed after completion of the response
										  //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
	client.println();

	if (reason != "") {
		StaticJsonBuffer<100> jsonBuffer;
		JsonObject& root = jsonBuffer.createObject();

		root["reason"] = reason;
		root.printTo(client);
	}
}