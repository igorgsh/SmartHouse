#pragma once
class Relay
{
public:
	Relay(int pin, bool on);
	~Relay();
	void connect();
	void disconnect();
private:
	int pin;
	bool on;
	bool status;
};

