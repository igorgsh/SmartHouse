/* -- do not edit following lines - START --
{
  "debug": false,
  "verbose": false
}
-- do not edit previous lines - END --*/
    const relayOff = true;

    var DevConfig=[];
    DevConfig["Board_00"]=20;
    DevConfig["Board_01"]=21;
    DevConfig["Board_02"]=16;
    
function resetDevice(pin, devName) {
    var pinState = rpiPath + pin + ".state";

    log("Reset device:" + devName + "[" + pin + "](" + pinState + ")", 'error');
    sendTo('telegram', "Reset device:" + devName);
    setState(pinState, relayOff);
    log("Delay for 5 sec");
    setTimeout(function () {
        log("Start device:" + devName + "(" + pinState + ")", 'error');
        setState(pinState, !relayOff);
    }, 5000);
}


function resetBoard(boardIdstr) {

    var boardName = "Board_"+boardIdstr;
    if (DevConfig[boardName] !== null) {
        resetDevice(DevConfig[boardName], boardName);
    }   
}


