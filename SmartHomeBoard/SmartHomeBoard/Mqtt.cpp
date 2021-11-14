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
#include "Board.h"
#include "Configuration.h"

extern Configuration Config;


void callbackFunc(char* topic, uint8_t* payload, unsigned int length) {
<<<<<<< HEAD
	String strTopic;
	String strPayload;

	strTopic = topic;
	strPayload = (char*)payload;
	
	MqttClient.Callback(strTopic, strPayload, length);
=======
	char* pl = (char*)payload;
	pl[length] = 0;
	//Config.MqttClient->Callback(topic, pl, length);
	Config.MqttClient->PutBuffer(topic, pl, length);
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
}

Mqtt::Mqtt() : PubSubClient(Configuration::MqttServer(), Configuration::MqttPort, callbackFunc, *(Config.EthClient)) {
	Serial.println("MQTT!");
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
<<<<<<< HEAD
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
=======
			Config.Log->Debug(F1("MqttReconnect"));
			if (connect(Config.BoardName)) {
				sprintf(topicBuff, MQTT_CONFIG_RESPONSE, Config.BoardId);
				Subscribe(topicBuff);
				sprintf(topicBuff, MQTT_ACTIONS_RESPONSE, Config.BoardId);
				Subscribe(topicBuff);
				//sprintF1(topic, MQTT_RESET_BOARD, Config.BoardId);
				//Subscribe(topic);
				res = true;
			}
			else {
				Config.Log->Error(F1("MQTT connection Failed"));
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
				res = false;
			}
		}
	}
	return res;
}
uint16_t Mqtt::GetUnitId(const char* str, int offset) {
	return atoi(str + offset);
}

<<<<<<< HEAD
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
=======
void Mqtt::Callback() {

	if (lenCB > 0) {
		
		Config.Log->append(F1("[")).append(topicCB).append(F1("]:")).append(payLoadCB).append(F1("#")).Debug();
		sprintf(topicBuff, MQTT_CONFIG_RESPONSE, Config.BoardId);
		if (strcmp(topicCB, topicBuff) == 0) {
			if (Config.isConfigRequested) {
				Config.UpdateConfig(payLoadCB);
			}
		}
		else {
			sprintf(topicBuff, MQTT_ACTIONS_RESPONSE, Config.BoardId);

			if (strcmp(topicCB, topicBuff) == 0) {
				if (Config.isActionRequested) {
					Config.UpdateActions(payLoadCB);
				}
			}
			else {
				int v = atoi(payLoadCB);
				

				if (strncmp(topicCB,MQTT_BUTTONS,strlen(MQTT_BUTTONS))) {
					Config.UpdateButton(GetUnitId(topicCB, strlen(MQTT_BUTTONS) + 2), v) ;
				}
				else {
					if (strncmp(topicCB,MQTT_RELAYS,strlen(MQTT_RELAYS))) {
						Config.UpdateRelay(GetUnitId(topicCB, strlen(MQTT_RELAYS) + 2), v);					}
					else {
						if (strncmp(topicCB,MQTT_1WIREBUS,strlen(MQTT_1WIREBUS))) {
							Config.UpdateOneWireBus(GetUnitId(topicCB, strlen(MQTT_1WIREBUS) + 2), v);
						}
						else {
							if (strncmp(topicCB,MQTT_1WIRETHERMO,strlen(MQTT_1WIRETHERMO))) {
								Config.UpdateOneWireThermo(GetUnitId(topicCB, strlen(MQTT_1WIRETHERMO) + 2), v);
							}
							else {
								if (strncmp(topicCB,MQTT_POWERMETER, strlen(MQTT_POWERMETER))) {
									Config.UpdatePowerMeter(GetUnitId(topicCB, strlen(MQTT_POWERMETER) + 2), v);
								}
								else {
									if (strncmp(topicCB,MQTT_CONTACTOR, strlen(MQTT_CONTACTOR))) {
										Config.UpdateContactor(GetUnitId(topicCB, strlen(MQTT_CONTACTOR) + 2), v);
									}
									else {
										if (strncmp(topicCB, MQTT_RESET_BOARD, strlen(MQTT_RESET_BOARD))) {
											Config.Log->Info(F1("Reset"));

											if (v > 0) {
												Board::Reset(10000);
											}
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
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
<<<<<<< HEAD
	Loger::LogMessage = F("Init MQTT");
	Loger::Debug();
=======
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
	long connectTry = 0;
	bool res = false;
	for (int i = 0; i < 7; i++) {
		sprintf(topicLog[i], MQTT_LOG, Config.BoardId, LOG_END[i]);
	}

	while (!res && connectTry <= MQTT_TRY_COUNT) {
<<<<<<< HEAD
		Loger::LogMessage = F("Mqtt connect attempt=");
		Loger::LogMessage += connectTry;
		Loger::Debug();
=======
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
		res = MqttReconnect();
		delay(MQTT_INITIAL_RETRY_DELAY);
		connectTry++;
	}
	
	if (!res) {
<<<<<<< HEAD
		Loger::LogMessage = F("Too many attempt of MQTT reconnect");
		Loger::Debug();
=======
		Config.Log->Error(F1("Too many attempt of MQTT reconnect"));
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
	}
}
void Mqtt::MqttLoop() {

	static long lastConnected = 0;

	if (connected()) {
		//Config.Log->Debug("Point1");
		bool res = loop();
		if (!res) {
<<<<<<< HEAD
			Loger::LogMessage = F("Failed loop");
			Loger::Error();
=======
			Config.Log->Error(F1("Failed loop"));
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
		}
		lastConnected = millis();
		//Config.Log->Debug("Point2");
		if (lenCB != 0) {
			Callback();
			lenCB = 0;
		}
	}
	else {
		if (lastConnected + millis() <= MQTT_RETRY_TIME) {
<<<<<<< HEAD
			Loger::LogMessage = F("Trying to reconnect MQTT");
			Loger::Debug();
=======
			Config.Log->Debug(F1("Trying to reconnect MQTT"));
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
			bool res = MqttReconnect();
			if (res) {
				Config.Init();
			}
		}
	}
}

<<<<<<< HEAD
void Mqtt::PublishLog(DebugLevel level, const String &message) {
	if (connected()) {
		char topic[MQTT_TOPIC_LENGTH];
		sprintf(topic, MQTT_LOG, Config.BoardId, LOG_END[level]);
		String strTopic = topic;
		Publish(strTopic, message);
=======
void Mqtt::PublishLog(DebugLevel level, const char* message) {

	if (connected()) {
		publish(topicLog[level], message);
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
	}
}


void Mqtt::GetConfiguration() {

<<<<<<< HEAD
	uint8_t rnd = random(0, 1000);
	char topic[MQTT_TOPIC_LENGTH];
=======
	uint8_t rnd = random(100, 999);
	char strRnd[3];
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
	Config.IsConfigReady = false;
	sprintf(strRnd, "%3u", rnd);
	if (connected()) {
<<<<<<< HEAD
		sprintf(topic, MQTT_CONFIG_REQUEST, Config.BoardId);
		String strTopic = topic;
		String strPayload;
		strPayload = rnd;
		Publish(strTopic, strPayload);
=======
		sprintf(topicBuff, MQTT_CONFIG_REQUEST, Config.BoardId);
		Publish(topicBuff, strRnd);
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
		Config.isConfigRequested = true;
	}
	else {
		Config.isConfigRequested = false;
	}
}

void Mqtt::WatchDog() {
<<<<<<< HEAD
	char topic[MQTT_TOPIC_LENGTH];
	sprintf(topic, MQTT_WATCH_DOG, Config.BoardId);
	String strTopic = topic;
	String strPayload;
	strPayload = Config.counter60;
	Publish(strTopic, strPayload);
=======

	//uint8_t rnd = random(0, 1000);
	sprintf(topicBuff, MQTT_WATCH_DOG, Config.BoardId);
	Publish(topicBuff, String(Config.counter60).c_str());
}

void Mqtt::PutBuffer(const char* topic, const char* payload, unsigned int length)
{
	strcpy(topicCB, topic);
	strcpy(payLoadCB, payload);
	lenCB = length;
	//Config.Log->append("Buffer:(").append(lenCB).append(")[").append(topicCB).append("]:").append(payLoadCB).Debug();
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
}


void Mqtt::PublishUnit(const Unit* unit) {
	if (connected()) {
<<<<<<< HEAD
		//if (unit->isSubscribed) {
		char topic[MQTT_TOPIC_LENGTH];
=======
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
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
<<<<<<< HEAD
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
=======
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
			}
			if (unitType != 0) {
				sprintf(topicBuff, "%s%s%c%04d", unitPrefix, MQTT_SEPARATOR, unit->Type, unit->Id);
				sprintf(payload, "%u", unit->status);
				Publish(topicBuff, payload);
			}
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
		}
	}
}

<<<<<<< HEAD
bool Mqtt::Publish(const String &topic, const String &payload) {
	if (connected()) {
		Loger::LogMessage = F("Publish:[");
		Loger::LogMessage += topic;
		Loger::LogMessage += F("]");
		Loger::LogMessage += payload;
		Loger::Debug();
		return publish(topic.c_str(), payload.c_str());
=======
bool Mqtt::Publish(const char* topic, const char* payload) {
	Config.Log->append(F1("Publish [")).append(topic).append(F1("]:")).append(payload).Debug();
	if (connected()) {
		return publish(topic, payload);
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
	}
	else {
		return false;
	}
}

void Mqtt::GetActions() {
<<<<<<< HEAD
	int rnd =  random(0, 1000);
=======
	char buf[10];
	sprintf(buf, "%lu", random(0, 1000));
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
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
<<<<<<< HEAD
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
=======
		//char topic[MQTT_TOPIC_LENGTH];
		const char* unitPrefix = NULL;
		if (Config.units[unitNumber]->Type == UnitType::POWER_METER) {
			PowerMeter::MqttTopic(Config.units[unitNumber]->Id, topicBuff, PM_VOLTAGE);
			Subscribe(topicBuff);
			PowerMeter::MqttTopic(Config.units[unitNumber]->Id, topicBuff, PM_CURRENT);
			Subscribe(topicBuff);
			PowerMeter::MqttTopic(Config.units[unitNumber]->Id, topicBuff, PM_POWER);
			Subscribe(topicBuff);
			PowerMeter::MqttTopic(Config.units[unitNumber]->Id, topicBuff, PM_ENERGY);
			Subscribe(topicBuff);
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
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
<<<<<<< HEAD
				sprintf(topic, "%s%s%c%04d", unitPrefix, MQTT_SEPARATOR, Config.units[unitNumber]->Type, Config.units[unitNumber]->Id);
				String strTopic;
				strTopic = topic;
				Subscribe(strTopic);
=======
				sprintf(topicBuff, "%s%s%c%04d", unitPrefix, MQTT_SEPARATOR, Config.units[unitNumber]->Type, Config.units[unitNumber]->Id);
				Subscribe(topicBuff);
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
			}
		}
	}
}


void Mqtt::SubscribeUnits() {
	if (connected()) {
<<<<<<< HEAD
		bool isSubscriptionSuccess = true;
		Loger::Debug(F("Subscribing Units..."));
=======
//		bool isSubscriptionSuccess = true;
		Config.Log->Debug(F1("Subscribing Units..."));
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
		for (int i = 0; i < Config.numberUnits; i++) {
			SubscribeUnit(i);
		}
		/*
		delay(MQTT_RESUBSCRIPTION_DELAY);
		for (int i = 0; i < Config.numberUnits; i++) {

			MqttClient.MqttLoop();
			for (int j = 0; j < MQTT_RESUBSCRIBE_TRY_COUNT && !Config.units[i]->isSubscribed; j++) {
				SubscribeUnit(i);
				delay(MQTT_RESUBSCRIPTION_DELAY);
				MqttClient.MqttLoop();
			}
			if (!Config.units[i]->isSubscribed) {
				Log.append("Subscription Failed. Unit id=").append(Config.units[i]->Id).Debug();
				isSubscriptionSuccess = false;
			}
		}
		if (!isSubscriptionSuccess) {
<<<<<<< HEAD
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
=======
			Log.Error(F1("Some units are not subscribed"));
		}
		*/
		Config.Log->append(F1("End subscription:")).append(Config.numberUnits).Debug();
	}
}

void Mqtt::Subscribe(const char* topic) {
	Config.Log->append(F1("Subscription:")).append(topic).Debug();
>>>>>>> 1ec5f3fb062a15470b96ea082aff7a6990f76516
	if (connected()) {
		subscribe(topic.c_str());
	}
}
