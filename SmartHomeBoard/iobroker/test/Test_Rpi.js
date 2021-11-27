function resetDevice1(pin, devName) {

    var pinState = "rpi2.0.gpio." + pin + ".state";

    log("Reset device:" + devName + "[" + pin + "](" + pinState + ")", 'error');

    // set pin to HIGH (reset)

    setState(pinState, relayOff);
    log("Delay for 5 sec");
    setTimeout(function () {
        log("Start device:" + devName + "(" + pinState + ")", 'error');
        setState(pinState, !relayOff);
    }, 5000);
}

var relayOff = true;
resetDevice1("16", "www");
