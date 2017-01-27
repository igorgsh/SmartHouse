//Подключаем библиотеки
#include <DHT.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h> //https://github.com/knolleary/pubsubclient
//#include <dht.h>          //https://github.com/RobTillaart/Arduino/tree/master/libraries/DHTlib
//Настройки сети

byte mac[] = { 0xAB, 0xBC, 0xCD, 0xDE, 0xEF, 0x31 };
byte ip[] = { 192, 168, 0, 111 }; //IP-адрес платы arduino
byte mqttserver[] = { 192, 168, 0, 32 }; //IP-адрес сервера ioBroker
EthernetClient ethClient;
void callback(char* topic, byte* payload, unsigned int length);
PubSubClient client(mqttserver, 1883, callback, ethClient);
//Глобальные переменные
#define LED_pin 13
unsigned int send_interval = 10; //интервал отправки показаний на сервер по-умолчанию 10 секунд
unsigned long last_time = 0; //текущее время для таймера
#define DHT_PIN 22
DHT dht( DHT_PIN, DHT11);
char buff[20];
//Функция обработки входящих соединений - прием данных по подписке
void callback(char* topic, byte* payload, unsigned int length) {
	Serial.println("");
	Serial.println("-------");
	Serial.println("New callback of MQTT-broker");
	//преобразуем тему(topic) и значение (payload) в строку
	payload[length] = '\0';
	String strTopic = String(topic);
	String strPayload = String((char*)payload);
	//Исследуем что "прилетело" от сервера по подписке:
	//Изменение интервала опроса
	if (strTopic == "example1/send_interval") {
		int tmp = strPayload.toInt();
		if (tmp == 0) {
			send_interval = 10;
		}
		else {
			send_interval = strPayload.toInt();
		}
	}
	//Управление светодиодом
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
	//стартуем сетевое подключение
	Ethernet.begin(mac, ip);
	Serial.print("IP: ");
	Serial.println(Ethernet.localIP());
	//Инициализируем порты ввода-вывода, прописываем начальные значения
	pinMode(LED_pin, OUTPUT);
	digitalWrite(LED_pin, LOW); //при светодиод отключен
	dht.begin();
}
void loop() {
	//Если соединение MQTT неактивно, то пытаемся установить его и опубликовать/подписаться
	if (!client.connected()) {
		Serial.print("Connect to MQTT-boker...  ");
		//Подключаемся и публикуемся/подписываемся
		if (client.connect("example1")) {
			Serial.println("success");
			//Значение с датчиков
			dtostrf(dht.readHumidity(), 5, 2, buff);
			Serial.print("Humidity=");
			Serial.println(buff);
			client.publish("example1/hum", buff);
			dtostrf(dht.readTemperature(), 5, 2, buff);
			Serial.print("Temp=");
			Serial.println(buff);
			client.publish("example1/temp", buff);
			//Подписываемся на интервал опроса
			client.subscribe("example1/send_interval");
			//Подписываемся на переменную управления светодиодом
			client.subscribe("example1/led");
		}
		else {
			//Если не подключились, ждем 10 секунд и пытаемся снова
			Serial.print("Failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 10 seconds");
			delay(10000);
		}
		//Если соединение активно, то отправляем данные на сервер с заданным интервалом времени
	}
	else {
		if (millis() > (last_time + send_interval * 1000)) {
			last_time = millis();
			//Значение с датчиков
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
	//Проверка входящих соединений по подписке
	client.loop();
}