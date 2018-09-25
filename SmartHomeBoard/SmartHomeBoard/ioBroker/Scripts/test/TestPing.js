
function checkStates0() {
    var NetworkState = getState(RouterPing).val
    log ("State Of Network is: " + NetworkState);
    /*
    $(pingGrp+'.*').each(function(id,i){
        var devName = getObject(id).common.name;
        var state = getState(id).val;
        if (state === false) {
            log("Device " + devName + " is down");
            if (typeof DevConfig[devName] != 'undefined') {
                log("Ping: Device " + devName +" on pin:" + DevConfig[devName] + " should be reset", 'error');
                resetDevice(DevConfig[devName], devName);
            }
        } else { //check Watchdog
            if (checkWatchDog(devName) > 60000) { //watchDog older than 1 minutes
                if (typeof DevConfig[devName] != 'undefined') {
                    log("WatchDog: Device " + devName +" on pin:" + DevConfig[devName] + " should be reset", 'error');
                
                    resetDevice(DevConfig[devName], devName);
                }
            }
        }
    });
*/
}

checkStates0();