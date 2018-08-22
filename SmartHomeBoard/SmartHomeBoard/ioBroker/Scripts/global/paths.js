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

var pingGrp = "ping.0";
var RouterPing = pingGrp + '.192_168_0_1';

var SunPrefix = prefixTopic + '.Config.Environment';
var SunsetVar = SunPrefix + '.Sunset';
var SunriseVar = SunPrefix + '.Sunrise';

var gardenPump = prefixRelays + ".R0118";
var gardenZoneNear = prefixRelays + ".R0114";
var gardenZoneGarden = prefixRelays + ".R0115";
var gardenZoneMiddle = prefixRelays + ".R0119";
var gardenZoneFar = prefixRelays + ".R0120";
