/*
 Name:		Test_RESTful.ino
 Created:	14.01.17 17:23:02
 Author:	Igor Shevchenko
*/

// the setup function runs once when you press reset or power the board
#include <SPI.h>
#include <Ethernet.h>
#include <aREST.h>
#include <avr/wdt.h>


byte mac[] = {
	0x00, 0xAA, 0x22, 0x07, 0x19, 0x69
};
// IP address in case DHCP fails
IPAddress ip(192, 168, 0, 103);

byte serverPort = 80;

//EthernetClient client;
EthernetServer server(serverPort);
aREST rest = aREST();

// Variables to be exposed to the API
int temperature;
int humidity;
struct {
	char *name;
	int number;
} str_test;

//String str = "{\"address\": \"\",\"description\" : \"link button not pressed\",\"type\" : \"101\"}";
String str = "{address: ,description : link button not pressed,type : 101}";

void setup() {
	// Start Serial
	Serial.begin(115200);

	// Init variables and expose them to REST API
	temperature = 24;
	humidity = 40;
	str_test.name = "MyName";
	str_test.number = 15;

	rest.variable("temperature", &temperature);
	rest.variable("humidity", &humidity);
	//rest.variable("test.name", str_test.name);
	rest.variable("test/_number", &(str_test.number));
	rest.variable("str", &str);
	
	
	// Function to be exposed
	rest.function("led", ledControl);

	// Give name & ID to the device (ID should be 6 characters long)
	rest.set_id("008");
	rest.set_name("sigma_shig");

	// Start the Ethernet connection and the server
	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP");
		// no point in carrying on, so do nothing forevermore:
		// try to congifure using IP address instead of DHCP:
		Ethernet.begin(mac, ip);
	}
	server.begin();
	Serial.print("server is at ");
	Serial.println(Ethernet.localIP());

	// Start watchdog
	//wdt_enable(WDTO_4S);
}

void loop() {

	// listen for incoming clients
	EthernetClient client = server.available();
	rest.handle(client);
	
	//wdt_reset();

}

// Custom function accessible by the API
int ledControl(String command) {

	// Get state from command
	int state = command.toInt();

	digitalWrite(13, state);
	return 1;
}
