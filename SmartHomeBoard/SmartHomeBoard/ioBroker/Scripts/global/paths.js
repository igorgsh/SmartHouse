/* -- do not edit following lines - START --
{
  "debug": false,
  "verbose": false
}
-- do not edit previous lines - END --*/
var prefixTopic = 'mqtt.0';
var prefixConfig = prefixTopic + '.Config';
var prefixBrd = prefixConfig + '.Board_';


var prefixEquip = prefixTopic + '.Equipment';
var prefixLog = prefixTopic + '.Log.Board_';
var prefixRelays = prefixEquip + ".Relays";
var prefixButtons = prefixEquip + ".Buttons";
var prefixPowerMeter = prefixEquip + '.PowerMeter';

var prefixVirtual = prefixEquip + '.Virtual';
var prefixVButtons = prefixVirtual + '.Buttons';
var prefixVRelays = prefixVirtual + '.Relays';

var prefixOthers = prefixTopic + '.Others';
var prefixHeating = prefixOthers + '.Heating'; 

var watchDogPart = 'WatchDog';
var reqConfPart = 'ConfigurationRequest';

var reqActPart = 'ActionRequest';


var pingGrp = "ping.0";
var RouterPing = pingGrp + '.192_168_0_1';

var EnvPrefix = prefixTopic + '.Config.Environment';
var SunsetVar = EnvPrefix + '.Sunset';
var SunriseVar = EnvPrefix + '.Sunrise';

var gardenPump = prefixRelays + ".R0120";
var gardenZoneNear = prefixRelays + ".R0114";
var gardenZoneGarden = prefixRelays + ".R0115";
var gardenZoneMiddle = prefixRelays + ".R0119";
var gardenZoneFar = prefixRelays + ".R0116";
var gardenWateringDelay = EnvPrefix + ".WateringDelay"; 
var gardenWateringDelayLeft = EnvPrefix + ".WateringDelayLeft";
var gardenWateringDelayTime = EnvPrefix + ".WateringDelayTime";
var gardenWateringDelayTimeStamp = EnvPrefix + ".WateringDelayTimeStamp";


var geoPump = "broadlink2.0.GeoPump.state";
var hvacPump1 = prefixRelays + ".R0117";
var hvacPump2 = prefixRelays + ".R0121";

var boilers = [ prefixRelays + ".R0018", prefixRelays + ".R0055", prefixRelays + ".R0056" ];

var vbSwitchOff = prefixVButtons + ".b0001";
var vbSwitchCooling = prefixVButtons + ".b0002";
//var vbGateLock = prefixVButtons + ".b0002";

var accumRelay = prefixRelays + ".R0102";

var gateLock = prefixRelays + ".R0149";
var BoilerSwitch = EnvPrefix + ".BoilerSwitch";
var intercomOpen = prefixRelays + ".R0148";

var ZMroot = prefixTopic + ".ZM";
var ZMMonitors = ZMroot + ".*";
var monitorParking = 6;
var monitorGate = 1;
var monitorMainDoor = 5;
var lightParking = prefixRelays + ".R0144";


var illumination = prefixRelays + ".R0036";

var totalHeatingPower = prefixHeating + '.Power';
var totalHeatingConsumption = prefixHeating + '.Consumption';






