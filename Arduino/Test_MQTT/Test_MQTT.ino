//���������� ����������
#include <DHT.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h> //https://github.com/knolleary/pubsubclient
//#include <dht.h>          //https://github.com/RobTillaart/Arduino/tree/master/libraries/DHTlib
//��������� ����

byte mac[] = { 0xAB, 0xBC, 0xCD, 0xDE, 0xEF, 0x31 };
byte ip[] = { 192, 168, 0, 111 }; //IP-����� ����� arduino
byte mqttserver[] = { 192, 168, 0, 32 }; //IP-����� ������� ioBroker
EthernetClient ethClient;
void callback(char* topic, byte* payload, unsigned int length);
PubSubClient client(mqttserver, 1883, callback, ethClient);
//���������� ����������
#define LED_pin 13
unsigned int send_interval = 10; //�������� �������� ��������� �� ������ ��-��������� 10 ������
unsigned long last_time = 0; //������� ����� ��� �������
#define DHT_PIN 22
DHT dht( DHT_PIN, DHT11);
char buff[20];
//������� ��������� �������� ���������� - ����� ������ �� ��������
void callback(char* topic, byte* payload, unsigned int length) {
	Serial.println("");
	Serial.println("-------");
	Serial.println("New callback of MQTT-broker");
	//����������� ����(topic) � �������� (payload) � ������
	payload[length] = '\0';
	String strTopic = String(topic);
	String strPayload = String((char*)payload);
	//��������� ��� "���������" �� ������� �� ��������:
	//��������� ��������� ������
	if (strTopic == "example1/send_interval") {
		int tmp = strPayload.toInt();
		if (tmp == 0) {
			send_interval = 10;
		}
		else {
			send_interval = strPayload.toInt();
		}
	}
	//���������� �����������
	if (strTopic == "example1/led") {
		if (strPayload == "off" || strPayload == "0" || strPayload == "false") digitalWrite(LED_pin, LOW);
		if (strPayload == "on" || strPayload == "1" || strPayload == "true") digitalWrite(LED_pin, HIGH);
	}
	Serial.print("[" + strTopic +"]");
	Serial.print(" ");
	Serial.println(strPayload);
	Serial.println("-------");
	Serial.println("");
}
void setup() {
	Serial.begin(115200);
	Serial.println("Start...");
	//�������� ������� �����������
	Ethernet.begin(mac, ip);
	Serial.print("IP: ");
	Serial.println(Ethernet.localIP());
	//�������������� ����� �����-������, ����������� ��������� ��������
	pinMode(LED_pin, OUTPUT);
	digitalWrite(LED_pin, LOW); //��� ��������� ��������
	dht.begin();
}
void loop() {
	//���� ���������� MQTT ���������, �� �������� ���������� ��� � ������������/�����������
	if (!client.connected()) {
		Serial.print("Connect to MQTT-boker...  ");
		//������������ � �����������/�������������
		if (client.connect("example1")) {
			Serial.println("success");
			//�������� � ��������
			dtostrf(dht.readHumidity(), 5, 2, buff);
			Serial.print("Humidity=");
			Serial.println(buff);
			client.publish("example1/hum", buff);
			dtostrf(dht.readTemperature(), 5, 2, buff);
			Serial.print("Temp=");
			Serial.println(buff);
			client.publish("example1/temp", buff);
			//������������� �� �������� ������
			client.subscribe("example1/send_interval");
			//������������� �� ���������� ���������� �����������
			client.subscribe("example1/led");
		}
		else {
			//���� �� ������������, ���� 10 ������ � �������� �����
			Serial.print("Failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 10 seconds");
			delay(10000);
		}
		//���� ���������� �������, �� ���������� ������ �� ������ � �������� ���������� �������
	}
	else {
		if (millis() > (last_time + send_interval * 1000)) {
			last_time = millis();
			//�������� � ��������
			dtostrf(dht.readHumidity(), 5, 2, buff);
			Serial.print("Humidity=");
			Serial.println(buff);
			client.publish("example1/hum", buff);
			dtostrf(dht.readTemperature(), 5, 2, buff);
			Serial.print("Temp=");
			Serial.println(buff);
			client.publish("example1/temp", buff);
		}
	}
	//�������� �������� ���������� �� ��������
	client.loop();
}