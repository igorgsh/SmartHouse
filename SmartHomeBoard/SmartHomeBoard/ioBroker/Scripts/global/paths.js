/* -- do not edit following lines - START --
{
  "debug": false,
  "verbose": false
}
-- do not edit previous lines - END --*/
var prefixTopic = 'mqtt.0';
var prefixCfg = prefixTopic + '.Config.Board_';

var prefixEquip = prefixTopic + '.Equipment';
var prefixLog = prefixTopic + '.Log.Board_';
var prefixRelays = prefixEquip + ".Relays";
var prefixButtons = prefixEquip + ".Buttons";

var prefixVirtual = prefixTopic + '.Virtual';
var prefixVButtons = prefixVirtual + '.Buttons';
var prefixVRelays = prefixVirtual + '.Relays';


var pingGrp = "ping.0";
var RouterPing = pingGrp + '.192_168_0_1';

var EnvPrefix = prefixTopic + '.Config.Environment';
var SunsetVar = EnvPrefix + '.Sunset';
var SunriseVar = EnvPrefix + '.Sunrise';

var gardenPump = prefixRelays + ".R0118";
var gardenZoneNear = prefixRelays + ".R0114";
var gardenZoneGarden = prefixRelays + ".R0115";
var gardenZoneMiddle = prefixRelays + ".R0119";
var gardenZoneFar = prefixRelays + ".R0120";
var gardenWateringDelay = EnvPrefix + ".WateringDelay"; 

var boilers = [ prefixRelays + ".R0018"];

var vbSwitchOff = "mqtt.0.Equipment.Virtual.Buttons.b0001";
