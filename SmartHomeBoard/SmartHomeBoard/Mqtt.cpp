// 
// 
// 
/*
Name:		Mqtt
Created:	11.01.17 22:04:42
Author:	Igor Shevchenko
*/

#include "Mqtt.h"
//#include "Loger.h"
#include "definitions.h"
#include "utils.h"
#include "Board.h"
#include "PowerMeter.h"

extern Mqtt MqttClient;

void callbackFunc(char* topic, uint8_t* payload, unsigned int length) {
	String strTopic;
	String strPayload;

	strTopic = topic;
	strPayload = (char*)payload;
	
	MqttClient.Callback(strTopic, strPayload, length);
}

Mqtt::Mqtt() : PubSubClient(Configuration::MqttServer(), Configuration::MqttPort, callbackFunc, EthClient) {
	mqttWaiting = MQTT_INITIAL_RETRY_DELAY;

}

void Mqtt::SubscribeBoardTopic(const char *templ, byte boardId) {
	char topic[MQTT_TOPIC_LENGTH];
	sprintf(topic, templ, boardId);
	String strTopic = topic;
	Subscribe(strTopic);
}

bool Mqtt::MqttReconnect() {
	
	bool res = false;

	if (Config.IsEthernetConnection) {
		if (!connected()) {
			Loger::LogMessage = F("MqttReconnect");
			Loger::Debug();
			if (connect(Config.BoardName.c_str())) {

				SubscribeBoardTopic(MQTT_CONFIG_RESPONSE, Config.BoardId);
				SubscribeBoardTopic(MQTT_ACTIONS_RESPONSE, Config.BoardId);
				res = true;
			}
			else {
				Loger::LogMessage = F("Failed, rc=");
				Loger::LogMessage += MqttClient.state();
				Loger::Error();
				res = false;
			}
		}
	}
	return res;
}

int Mqtt::GetUnitId(const String &topic, int prefixLen) {
	int i = 0;
	String strId = topic.substring(prefixLen);
	i = strId.toInt();
	return i;
}

void Mqtt::Callback(const String& topic, const String& payLoad, unsigned int length) {

	if (length > 0) {

		if (topic.endsWith(MQTT_CONFIGURATION))
			if (Config.isConfigRequested) {
				Config.UpdateConfig(payLoad);
			}
	}
	else {
		if (topic.endsWith(MQTT_ACTIONS)) {
			if (Config.isActionRequested) {
				Config.UpdateActions(payLoad);
			}
		}
		else {
			if (topic.startsWith(MQTT_BUTTONS)) {
				Config.UpdateButton(GetUnitId(topic, strlen(MQTT_BUTTONS) + 2), payLoad);
			}
			else {
				if (topic.startsWith(MQTT_RELAYS)) {
					Config.UpdateRelay(GetUnitId(topic, strlen(MQTT_RELAYS) + 2), payLoad);
				}
				else {
					if (topic.startsWith(MQTT_1WIREBUS)) {
						Config.UpdateOneWireBus(GetUnitId(topic, strlen(MQTT_1WIREBUS) + 2), payLoad);
					}
					else {
						if (topic.startsWith(MQTT_1WIRETHERMO)) {
							Config.UpdateOneWireThermo(GetUnitId(topic, strlen(MQTT_1WIRETHERMO) + 2), payLoad);
						}
						else {
							if (topic.startsWith(MQTT_POWERMETER)) {
								Config.UpdatePowerMeter(GetUnitId(topic, strlen(MQTT_POWERMETER) + 2), payLoad);
							}
							else {
								if (topic.startsWith(MQTT_CONTACTOR)) {
									Config.UpdateContactor(GetUnitId(topic, strlen(MQTT_CONTACTOR) + 2), payLoad);
								}
								else {
									if (topic.startsWith(MQTT_SHIFT_REGISTER_OUT)) {
										Config.UpdateShiftRegisterOut(GetUnitId(topic, strlen(MQTT_SHIFT_REGISTER_OUT) + 2), payLoad);
									}
									else {
										if (topic.startsWith(MQTT_SHIFT_REGISTER_IN)) {
											Config.UpdateShiftRegisterIn(GetUnitId(topic, strlen(MQTT_SHIFT_REGISTER_IN) + 2), payLoad);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}




void Mqtt::InitMqtt(void) {
	Loger::LogMessage = F("Init MQTT");
	Loger::Debug();
	long connectTry = 0;
	bool res = false;
	
	while (!res && connectTry <= MQTT_TRY_COUNT) {
		Loger::LogMessage = F("Mqtt connect attempt=");
		Loger::LogMessage += connectTry;
		Loger::Debug();
		res = MqttReconnect();
		delay(MQTT_INITIAL_RETRY_DELAY);
		connectTry++;
	}
	
	if (!res) {
		Loger::LogMessage = F("Too many attempt of MQTT reconnect");
		Loger::Debug();
	}
}
void Mqtt::MqttLoop() {

	static long lastConnected = 0;

	if (connected()) {
		bool res = loop();
		if (!res) {
			Loger::LogMessage = F("Failed loop");
			Loger::Error();
		}
		lastConnected = millis();
	}
	else {
		if (lastConnected + millis() <= MQTT_RETRY_TIME) {
			Loger::LogMessage = F("Trying to reconnect MQTT");
			Loger::Debug();
			bool res = MqttReconnect();
			if (res) {
				Config.Init();
			}
		}
	}
}

void Mqtt::PublishLog(DebugLevel level, const String &message) {
	if (connected()) {
		char topic[MQTT_TOPIC_LENGTH];
		sprintf(topic, MQTT_LOG, Config.BoardId, LOG_END[level]);
		String strTopic = topic;
		Publish(strTopic, message);
	}
}


void Mqtt::GetConfiguration() {

	uint8_t rnd = random(0, 1000);
	char topic[MQTT_TOPIC_LENGTH];
	Config.IsConfigReady = false;
	if (connected()) {
		sprintf(topic, MQTT_CONFIG_REQUEST, Config.BoardId);
		String strTopic = topic;
		String strPayload;
		strPayload = rnd;
		Publish(strTopic, strPayload);
		Config.isConfigRequested = true;
	}
	else {
		Config.isConfigRequested = false;
	}
}

void Mqtt::WatchDog() {
	char topic[MQTT_TOPIC_LENGTH];
	sprintf(topic, MQTT_WATCH_DOG, Config.BoardId);
	String strTopic = topic;
	String strPayload;
	strPayload = Config.counter60;
	Publish(strTopic, strPayload);
}


void Mqtt::PublishUnit(const Unit* unit) {
	if (connected()) {
		//if (unit->isSubscribed) {
		char topic[MQTT_TOPIC_LENGTH];
		char payload[MQTT_PAYLOAD_LENGTH];
		const char* unitPrefix;
		byte unitType = 0;
		if (unit->Type == ::POWER_METER) {
			PowerMeter* p = (PowerMeter*)unit;
			p->PublishAll();
		}
		else {
			switch (unit->Type) {
			case UnitType::BUTTON: {
				unitPrefix = MQTT_BUTTONS;
				unitType = unit->Type;
				break;
			}
			case UnitType::RELAY: {
				unitPrefix = MQTT_RELAYS;
				unitType = unit->Type;
				break;
			}
			case UnitType::ONE_WIRE_BUS: {
				unitPrefix = MQTT_1WIREBUS;
				unitType = unit->Type;
				break;
			}
			case UnitType::ONE_WIRE_THERMO: {
				unitPrefix = MQTT_1WIRETHERMO;
				unitType = unit->Type;
				break;
			}
			case UnitType::VIRTUAL_BUTTON: {
				unitPrefix = MQTT_VIRTUAL_BUTTONS;
				unitType = UnitType::BUTTON;
				break;
			}
			case UnitType::CONTACTOR: {
				unitPrefix = MQTT_CONTACTOR;
				unitType = UnitType::CONTACTOR;
				break;
			}
			case UnitType::SHIFT_OUT: {
				unitPrefix = MQTT_SHIFT_REGISTER_OUT;
				unitType = UnitType::SHIFT_OUT;
				break;
			}
			case UnitType::SHIFT_IN: {
				unitPrefix = MQTT_SHIFT_REGISTER_IN;
				unitType = UnitType::SHIFT_IN;
				break;
			}
			}
			if (unitType != 0) {
				sprintf(topic, "%s%s%c%04d", unitPrefix, MQTT_SEPARATOR, unit->Type, unit->Id);
				sprintf(payload, "%u", unit->status);
				String strTopic = topic;
				String strPayload = payload;
				Publish(strTopic, strPayload);
			}
		}
	}
}

bool Mqtt::Publish(const String &topic, const String &payload) {
	if (connected()) {
		Loger::LogMessage = F("Publish:[");
		Loger::LogMessage += topic;
		Loger::LogMessage += F("]");
		Loger::LogMessage += payload;
		Loger::Debug();
		return publish(topic.c_str(), payload.c_str());
	}
	else {
		return false;
	}
}

void Mqtt::GetActions() {
	int rnd =  random(0, 1000);
	char topic[MQTT_TOPIC_LENGTH];
	Config.IsConfigReady = false;
	Config.IsActionsReady = false;
	if (connected()) {
		sprintf(topic, MQTT_ACTIONS_REQUEST, Config.BoardId);
		Config.isActionRequested = true;
		String strTopic=topic;
		String strPayload;
		strPayload = rnd;
		Publish(strTopic, strPayload);
	}
	else {
		Config.isActionRequested = true;
	}
}

void Mqtt::SubscribeUnit(int unitNumber) {
	if (connected()) {
		char topic[MQTT_TOPIC_LENGTH];
		const char* unitPrefix = NULL;
		if (Config.units[unitNumber]->Type == UnitType::POWER_METER) {
			String strTopic;
			strTopic.reserve(MQTT_TOPIC_LENGTH);
			PowerMeter::MqttTopic(Config.units[unitNumber]->Id, topic, PM_VOLTAGE);
			strTopic = topic;
			Subscribe(strTopic);
			PowerMeter::MqttTopic(Config.units[unitNumber]->Id, topic, PM_CURRENT);
			strTopic = topic;
			Subscribe(strTopic);
			PowerMeter::MqttTopic(Config.units[unitNumber]->Id, topic, PM_POWER);
			strTopic = topic;
			Subscribe(strTopic);
			PowerMeter::MqttTopic(Config.units[unitNumber]->Id, topic, PM_ENERGY);
			strTopic = topic;
			Subscribe(strTopic);
		}
		else {
			switch (Config.units[unitNumber]->Type) {
			case UnitType::BUTTON: {
				unitPrefix = MQTT_BUTTONS;
				break;
			}
			case UnitType::RELAY: {
				unitPrefix = MQTT_RELAYS;
				break;
			}
			case UnitType::ONE_WIRE_BUS: {
				unitPrefix = MQTT_1WIREBUS;
				break;
			}
			case UnitType::ONE_WIRE_THERMO: {
				unitPrefix = MQTT_1WIRETHERMO;
				break;
			}
			case UnitType::CONTACTOR: {
				unitPrefix = MQTT_CONTACTOR;
				break;
			}
			case UnitType::VIRTUAL_BUTTON: {
				unitPrefix = MQTT_VIRTUAL_BUTTONS;
				break;
			}
			case UnitType::SHIFT_OUT: {
				unitPrefix = MQTT_SHIFT_REGISTER_OUT;
				break;
			}
			case UnitType::SHIFT_IN: {
				unitPrefix = MQTT_SHIFT_REGISTER_IN;
				break;
			}

			}
			if (unitPrefix != NULL) {
				sprintf(topic, "%s%s%c%04d", unitPrefix, MQTT_SEPARATOR, Config.units[unitNumber]->Type, Config.units[unitNumber]->Id);
				String strTopic;
				strTopic = topic;
				Subscribe(strTopic);
			}
		}
	}
}


void Mqtt::SubscribeUnits() {
	if (connected()) {
		bool isSubscriptionSuccess = true;
		Loger::Debug(F("Subscribing Units..."));
		for (int i = 0; i < Config.numberUnits; i++) {
			SubscribeUnit(i);
		}
		delay(MQTT_RESUBSCRIPTION_DELAY);
		for (int i = 0; i < Config.numberUnits; i++) {

			MqttClient.MqttLoop();
			for (int j = 0; j < MQTT_RESUBSCRIBE_TRY_COUNT && !Config.units[i]->isSubscribed; j++) {
				SubscribeUnit(i);
				delay(MQTT_RESUBSCRIPTION_DELAY);
				MqttClient.MqttLoop();
			}
			if (!Config.units[i]->isSubscribed) {
				isSubscriptionSuccess = false;
			}
		}
		if (!isSubscriptionSuccess) {
			Loger::Error(F("Some units are not subscribed"));
		}
		Loger::Debug(F("End subscription"));
	}
}

void Mqtt::Subscribe(const String &topic) {
	Loger::LogMessage = F("Subscribe [");
	Loger::LogMessage += topic;
	Loger::LogMessage += F("]");
	Loger::Debug();
	if (connected()) {
		subscribe(topic.c_str());
	}
}
