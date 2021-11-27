# SmartHouse
Smart House development

History.
V 3.0
- switch to the SafeString and char*
- refactoring of application
- support of Shift Register In/Out
- The initial values are taken from the last state

V 2.1 
- bug fixes
- multi-board actions
- "Switch Off for All" functionality


Description



SmartHouse “SmartSigma”

Common functionality

SmartSigma (hereinafter referred to as SmartSigma) currently supports the following functionality:

The ability to turn on lighting (and indeed any electrical appliance) from switches of the “bell button" type (button without latching)
The ability to turn on lighting through a web interface
Support 1-wire specification
1-wire temperature sensor support
Support for lighting on / off schedules and electrical appliances
Power Meter Support
Support for equipment on / off scenarios triggered by the following events:
Schedule with recurring events, including astronomical ones (dawn-sunset-twilight-sunrise -...)
change of state of any of the items of equipment
reaching a value by the sensor (for example, when a certain temperature is reached
Virtual Button Support
Saving State Change History

SmartSigma Composition
(See SH_Common.png)

Fig. 1.

The general SmartSigma diagram is shown in Fig. 1.

Homeserver

HomeServer is a control computer that allows you to:
centralized management of individual controllers
web management interface hosting
checking the survivability of the controllers and rebooting them, in the event of a hang
maintaining controller configuration
saving the history of changes in values

As HomeServer, it is recommended to use the Raspberry Pi. But any computer with Windows / Linux / ... can be used (Node.js support required).
OS: Linux / Windows / ... (Node.js support required)
CPU, Memory: no fundamental requirements
Storage: 50 Mb
GPIO: It is desirable to have a GPIO or other outputs capable of controlling a 3-10V DC relay by the number of controllers used. In the absence of such equipment, the verification of the survivability of the controllers cannot be implemented.
Network: Ethernet interface from 10Mbps
DBMS: Any SQL DB (MySQL, PostgreSQL, ...) is highly desirable. You can add an external database server. In case of absence, the history of changes in the values ​​will not be saved.

IoBroker is installed as a UI on HomeServer (http://iobroker.net/). The general interface looks like this:

(see House3.png)


Fig. 2.

Appearance can be changed in any style. For examples and details - please visit the author's website http://iobroker.net/

The interface supports touch screen devices. Active elements are clickable.


Controllers

SmartSigma supports multi-controller on-the-fly architecture. Adding a new controller does not require a system restart.
Arduino Mega + W5100 Ethernet shield is selected as the controller. All sensors and actuators are connected to the terminals of the device. External memory is not used.


Supported Hardware


Buttons and switches
Switches of the “bell button" type are supported - without locking. The following states are recognized for this button:
short press: 0.12-0.7 seconds (presses up to 0.12sec are considered “noise”)
long press: 0.7-2 sec.
extra long press: 2 sec.

Relay
Relays controlled by both “zero” and “unit” are supported

1-wire bus

SmartSigma is a 1-bus controller

1-wire bus temperature sensor

It supports work with the DS18B20 sensor in both passive and active modes.


Voltage / current / consumption meter
It works with the PZEM-004t sensor, which allows measuring current voltage, current, power, and accumulation of consumption
