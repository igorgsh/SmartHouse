/* -- do not edit following lines - START --
{
  "debug": false,
  "verbose": false
}
-- do not edit previous lines - END --*/
var prefixTopic = 'mqtt.0';
var prefixCfg = prefixTopic + '.Config';

var watchDogPart = 'WatchDog';

function checkWatchDog(devName) {
    var objName = prefixCfg + "." + devName + "." + watchDogPart;
    var obj = $(objName);
    if (obj!='undefined') {
        var st = obj.getState();
        if (st !== null) {
            var now = Date.now();
            var diff = now -st.ts;
            return diff;
        }
    }
    return 0;
}