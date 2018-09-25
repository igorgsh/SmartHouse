
function CheckWateringDelay() {
    //var nm = getObject(zoneValve);
    //log ("Start check delay");
    var obj = getState(gardenWateringDelay);
    var delayVal = obj.val;
    log("delay=" + delayVal);
    if (delayVal === true) {
        var ts = obj.ts;
        var now = new Date().getTime();
        //log ("Delta=" + (now-ts));
        if (now - ts >= 12*3600*1000) {
            log("Reset Delay");
            setState(gardenWateringDelay, false, true);
            return true;
        }
        else {
            return false;
        }
    }
    return true;
}


function StartWatering(zoneValve, nm) {
    log ("Start " + nm);
    if (CheckWateringDelay()) {
        var pump;
        pump = getState(gardenPump);
        if (pump.value!=1) {
            setState(gardenPump, 1, false);
        }

        setState(zoneValve,1, false);
    } else {
        log("Watering is prohibited")
    }
}

function StopWatering(zoneValve, nm) {
    log ("Stop " + nm);
    setState(zoneValve,0, false);
}
 

schedule("00 7,23 * 6-9 *", function() { StartWatering(gardenZoneFar, "Far Zone")});
schedule("19 7,23 * 6-9 *", function() {StopWatering(gardenZoneFar, "Far Zone")});

schedule("20 7,23 * 6-9 *", function() {StartWatering(gardenZoneMiddle, "Middle Zone")});
schedule("39 7,23 * 6-9 *", function() {StopWatering(gardenZoneMiddle, "Middle Zone")});

schedule("40 5,20 * 6-9 *", function() {StartWatering(gardenZoneNear, "Nearest Zone")});
schedule("50 5,20 * 6-9 *", function() {StopWatering(gardenZoneNear, "Nearest Zone")});

schedule("40 7,23 * 6-9 *", function() {StartWatering(gardenZoneNear, "Nearest Zone")});
schedule("50 7,23 * 6-9 *", function() {StopWatering(gardenZoneNear, "Nearest Zone")});



schedule("00 6,20 * 6-9 *", function() {StartWatering(gardenZoneGarden, "Garden Zone")});
schedule("40 6,20 * 6-9 *", function() {StopWatering(gardenZoneGarden, "Garden Zone")});
