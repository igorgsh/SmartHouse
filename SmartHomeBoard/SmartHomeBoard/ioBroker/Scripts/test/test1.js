/*
var obj = getObject(gardenWateringDelay,true);
//var dt = getDateObject(obj.value);
log("obj=" + JSON.stringify(obj));
log("obj.ts=" + obj.ts);
log("Date=" + formatDate(obj.ts,"YYYY-MM-DD hh:mm"));
log("NewDate=" + formatDate(obj.ts+24*60*60*1000,"YYYY-MM-DD hh:mm"));
log("TodayDate=" + formatDate(new Date().getTime(),"YYYY-MM-DD hh:mm"));

*/

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
        if (now - ts >= 5*3600*1000) {
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

var x = CheckWateringDelay();
log("Result= " + x);
