schedule("00 23,5 * 6-9 *", function() {
    log ("Start Far Zone");
    var pump;
    pump = getState(gardenPump);
    if (pump.value!=1) {
        setState(gardenPump, 1, false);
    }
    
    setState(gardenZoneFar, 1, false);
    }
)

schedule("20 23,5 * 6-9 *", function() {
    log ("Stop Far Zone");
    setState(gardenZoneFar, 0, false);
    }
)

schedule("30 23,5 * 6-9 *", function() {
    log ("Start Middle Zone");
    var pump;
    pump = getState(gardenPump);
    if (pump.value!=1) {
        setState(gardenPump, 1, false);
    }
    
    setState(gardenZoneMiddle, 1, false);
    }
)

schedule("50 23,5 * 6-9 *", function() {
    log ("Stop Middle Zone");
    setState(gardenZoneMiddle, 0, false);
    }
)


schedule("00 0,6 * 6-9 *", function() {
    log ("Start Nearest Zone");
    var pump;
    pump = getState(gardenPump);
    if (pump.value!=1) {
        setState(gardenPump, 1, false);
    }
    
    setState(gardenZoneNear, 1, false);
    }
)

schedule("03 0,6 * 6-9 *", function() {
    log ("Stop Nearest Zone");
    setState(gardenZoneNear, 0, false);
    }
)


schedule("10 0,6 * 6-9 *", function() {
    log ("Start Garden Zone");
    var pump;
    pump = getState(gardenPump);
    if (pump.value!=1) {
        setState(gardenPump, 1, false);
    }
    
    setState(gardenZoneGarden, 1, false);
    }
)

schedule("50 0,6 * 6-9 *", function() {
    log ("Stop Garden Zone");
    setState(gardenZoneGarden, 0, false);
    }
)

