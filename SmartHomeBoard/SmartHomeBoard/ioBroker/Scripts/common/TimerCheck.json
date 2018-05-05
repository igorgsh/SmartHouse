/* -- do not edit following lines - START --
{
  "debug": false,
  "verbose": false
}
-- do not edit previous lines - END --*/

function checkStates() {
    var NetworkState = getState(RouterPing).val;
    
    if (NetworkState === true) {

        $(pingGrp+'.*').each(function(id,i){
            if (id  != RouterPing) {
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
            }
        });
    } else { // No Network
        log ("Network is down");
    }
}


schedule ("*/5 * * * *", function(){
//    log("Timer Check 5 is started", 'info');

    //initResetGPIO();
    checkStates();

});
