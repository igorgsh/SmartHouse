 
function checkStates() {
    var NetworkState = getState(RouterPing).val;
    //log("Checkstate");
    if (NetworkState === true) {

        $(pingGrp+'.*').each(function(id,i){
            if (id  != RouterPing) {
                var devName = getObject(id).common.name.substr(6);
                //log("Devname:" + devName);
                var state = getState(id).val;
                if (state === false) {
                    log("Device " + devName + " is down");
                    if (typeof DevConfig[devName] != 'undefined') {
                        log("Ping: Device " + devName +" on pin:" + DevConfig[devName] + " should be reset", 'error');
                        resetDevice(DevConfig[devName], devName);
                    } else {
                        log("Undefined device "+ devName, "debug");
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

schedule ('{"time":{"start":"00:00","end":"23:59","mode":"minutes","interval":5},"period":{"days":1}}', function(){
//schedule ("*/5 * * * *", function(){
    //log("Timer Check 5 is started", 'info');

    //initResetGPIO();
    checkStates();

});




