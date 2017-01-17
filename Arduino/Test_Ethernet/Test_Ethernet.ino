/*
 Name:		Test_Ethernet.ino
 Created:	14.01.17 16:59:49
 Author:	Igor Shevchenko
*/
/*
#include <EthernetUdp.h>
#include <EthernetServer.h>
#include <EthernetClient.h>
#include <Dns.h>
#include <Dhcp.h>
*/
#include <Ethernet.h>

byte mac[] = {
	0x00, 0xAA, 0x22, 0x07, 0x19, 0x69
};

byte serverPort = 80;

//EthernetClient client;
EthernetServer server(serverPort);


void printIPAddress()
{
	Serial.print("My IP address: ");
	for (byte thisByte = 0; thisByte < 4; thisByte++) {
		// print the value of each byte of the IP address:
		Serial.print(Ethernet.localIP()[thisByte], DEC);
		Serial.print(".");
	}

	Serial.println();
}

// the setup function runs once when you press reset or power the board
void setup() {
	// Open serial communications and wait for port to open:
	Serial.begin(9600);
	// this check is only needed on the Leonardo:
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}
	Serial.println("Start");

	// start the Ethernet connection:
	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP");
		// no point in carrying on, so do nothing forevermore:
		exit(-1);
	}
	// print your local IP address:
	printIPAddress();

	server.begin();
	Serial.print("server is at ");
	Serial.println(Ethernet.localIP());
}

// the loop function runs over and over again until power down or reset
void loop() {
	// listen for incoming clients
	EthernetClient client = server.available();
	if (client) {
		Serial.println("new client");
		// an http request ends with a blank line
		boolean currentLineIsBlank = true;
		while (client.connected()) {
			if (client.available()) {
				char c = client.read();
				Serial.write(c);
				// if you've gotten to the end of the line (received a newline
				// character) and the line is blank, the http request has ended,
				// so you can send a reply
				if (c == '\n' && currentLineIsBlank) {
					// send a standard http response header
					client.println("HTTP/1.1 200 OK");
					client.println("Content-Type: text/html");
					client.println("Connection: close");  // the connection will be closed after completion of the response
					client.println("Refresh: 5");  // refresh the page automatically every 5 sec
					client.println();
					client.println("<!DOCTYPE HTML>");
					client.println("<html>");
					// output the value of each analog input pin
					for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
						int sensorReading = analogRead(analogChannel);
						client.print("analog input ");
						client.print(analogChannel);
						client.print(" is ");
						client.print(sensorReading);
						client.println("<br />");
					}
					client.println("</html>");
					break;
				}
				if (c == '\n') {
					// you're starting a new line
					currentLineIsBlank = true;
				}
				else if (c != '\r') {
					// you've gotten a character on the current line
					currentLineIsBlank = false;
				}
			}
		}
		// give the web browser time to receive the data
		delay(1);
		// close the connection:
		client.stop();
		Serial.println("client disconnected");
	}
}
