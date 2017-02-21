#include "ArduinoServer.h"
#include "Definitions.h"
#include "Configuration.h"
#include "HttpRequest.h"

extern Configuration Config;


ArduinoServer::ArduinoServer()
{
	server = new EthernetServer(serverPort);
}


ArduinoServer::~ArduinoServer()
{
	//delete server;
}

void ArduinoServer::begin() {

	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP");
		// no point in carrying on, so do nothing forevermore:
		// try to congifure using IP address instead of DHCP:
		IPAddress ip(192, 168, 0, 101);
		Ethernet.begin(mac, ip);
	}
	server->begin();
	Debug2("Server is at: ",Ethernet.localIP());

}

bool ArduinoServer::loop() {
	bool result = true;
	EthernetClient client = server->available();
	while (client.available()) {
		if (ProcessRequest(client) != 0) {
			 //Error. Bad Request
			PrintErrorPage(client, "418 I'm a teapot", "Something wrong");
		}
		client.stop();
		Debug("Client disconnected");
		delay(1);
	}
	return result;
}


int ArduinoServer::ProcessRequest(Client& client) {
	String line;

	// Read type of request
	line = client.readStringUntil('/');
	Debug2("TYPE=",line);
	HttpRequest request;

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
	Debug2("URL=",line);
	request.URL = line;

	// skip HTTP/1.1
	line = client.readStringUntil(0x0a);
	Debug2("Line=", line);
	//Serial.println(line);

	//Read parameters of header
	while (client.available()) {
		line = client.readStringUntil(0x0a);
		Debug2("Line=", line);

		if (line.length() == 1 && line[0] == 0x0D) {// Empty line. The body will be next
			request.body = client.readString();
		}
		else if (line.startsWith("Host: ")) {
			request.host = line.substring(6);
		}
	}
	request.ParseParameters();
	Debug2("Result:", (request.type == GET ? "Type: GET" : "Type: POST"));
	Debug2("URL:",request.URL);
	Debug2("Host:",request.host);
	Debug2("Body:",request.body);
	Debug2("NumberOfParm:", request.NumberParms);

	ParseCommand(client, request);

	return 0;
}

void ArduinoServer::PrintErrorPage(Client& client, String error, String reason) {
	
	HttpHeader(client, error);
	client.println(error);
	client.println(reason);
}

void ArduinoServer::HttpHeader(Client& client, String error) {

	client.println("HTTP/1.1 " + error);
	client.println("Content - Type: text / html; charset = utf - 8");
	client.println("Connection: close");  // the connection will be closed after completion of the response
										  //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
	client.println();
}

//#define NUMBER_OF_PARTS	5


void ArduinoServer::ParseCommand(Client& client, HttpRequest request) {

	Debug("Start ParseCommand");
	if (request.URL.startsWith("main.html")) {
		PrintMainPage(client, request);
	}
	else if (request.URL.startsWith("temptable.html")) {
		PrintTemperaturePage(client, request);
		}
		else {
		PrintErrorPage(client, "404 Page not found", "This page is anavailable");
		}
	Debug("End ParseCommand");
}

void ArduinoServer::PrintMainPage(Client& client, HttpRequest request) {

	Debug("Start PrintMainPage");
	if (request.NumberParms != 0) {
		Debug("Point1");
		int ind = request.getIndexOfParmKey("desTemp");
		if (ind!=-1) {
			Config.cThermo.setDesiredTemp(request.getParmValue(ind).toFloat());
			Debug2("Desired=", Config.cThermo.getDesiredTemp());
		}
	}

	HttpHeader(client,"200 Ok");
	client.println("<html><head><title>Главная</title>");
	client.println("<style>");
	client.println("div{ display:inline; }");
	client.println(".text1{ font: 2.5em normal; }");
	client.println(".desiredTemp{ font: 3em bold; color:green; }");
	client.println(".currentTemp{ font: 5em bold; color:blue; }");
	client.println("#formsubmit{visibility: hidden;	}");
	client.println("</style>");
	client.println("<script type = 'text/javascript'>");
	client.print("var currTemp=");
	client.print(Config.cThermo.getTemp());
	client.println(";");
	client.print("var desTemp =");
	client.print(Config.cThermo.getDesiredTemp());
	client.println(";");
	client.println("var isEdit = 0;");
	client.println("var dt = 'Пн, 20 фев. 12:34';");
	client.println("var submitCounter = 0;");
	/*
	client.println("setInterval(function() {if (isEdit == 1) {submitCounter++;}");
	client.println("if (isEdit == 0 || submitCounter >= 10) {window.location.reload(1);}");
	client.println("}, 5000);");
	*/
	client.println("window.onload = function(){");
	client.println("document.getElementById('desiredTemp').innerHTML = desTemp; ");
	client.println("document.getElementById('currTemp').innerHTML = currTemp;");
	client.println("document.getElementById('desTemp').value = desTemp; ");
	client.println("document.getElementById('currTime').innerHTML = dt;}");
	client.println("function chngTemp(c){desTemp += c;");
	client.println("document.getElementById('desTemp').value = desTemp; ");
	client.println("document.getElementById('desiredTemp').innerHTML = desTemp;");
	client.println("document.getElementById('formsubmit').style.visibility = 'visible';");
	client.println("submitCounter = 0;isEdit = 1;}");
	client.println("</script>");
	client.println("</head>");
	client.println("<body bgcolor = '#E6E6FA'>");
	client.println("<form  method = 'post' >");
	client.println("<div id = 'currTime'></div><br/>");
	client.println("<div>Текущая температура:</div>");
	client.println("<div id='currTemp' class='currentTemp'></div><div class='currentTemp'>ºС</div><br/>");
	client.println("<div>Установлена:</div><div id='desiredTemp' class='desiredTemp'></div><div class='desiredTemp'>ºС</div>");
	client.println("<input type='button' onclick='chngTemp(1)' value = '+'>");
	client.println("<input type='button' onclick='chngTemp(-1)' value='-'>");
	client.println("<input type='hidden' name='desTemp' id='desTemp' value=0.0>");
	client.println("<input id='formsubmit' name='formsubmit' type='submit' value='Ok'><br/>");
	client.println("<a href='timetable.html'>Расписание</a>&nbsp;<a href='temptable.html'>Параметры котла</a>");
	client.println("</form></body></html>");
	Debug("End PrintMainPage");

}

/*


function form_submit() {
isEdit=1;
submitCounter=0;
alert("Hello!");
}
*/


void ArduinoServer::PrintTemperaturePage(Client& client, HttpRequest request) {

}
