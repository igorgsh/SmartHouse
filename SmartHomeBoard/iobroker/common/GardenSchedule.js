

function CheckWateringDelay() {
    var obj = getState(gardenWateringDelay);
    var delayVal = obj.val;
    log("Watering:delay=" + delayVal);
    if (delayVal === true) {
        var ts = getState(gardenWateringDelayTimeStamp);
        var now = new Date().getTime();
        if (now >= ts.val) {
            log("Watering:Reset Delay");
            setState(gardenWateringDelay, false, true);
            return true;
        }
        else {
            return false;
        }
    }
    return true;
}

function StartWateringPump(){
    setState(gardenPump, 1, false);
}

function StartWatering(zoneValve, nm) {
    log ("Watering:Start " + nm);
    if (CheckWateringDelay()) {
        var pump;
        pump = getState(gardenPump);
        if (pump.val!=1) {
            setState(gardenPump, 1, false);
        }

        setState(zoneValve,1, false);
    } else {
        log("Watering is prohibited")
    }
}

function StopWatering(zoneValve, nm) {
    log ("Watering: Stop " + nm);
    setState(zoneValve,0, false);
}


/*
Far and Middle provides 13 liters per 1m2 per hour
1h = 13 l
45m = 10 l
30m = 6.5 l
20m = 4 l
10m = 2.5 l
Норма: 35л. = 3 часа
*/


/* 
    Far, Middle, Nearest: Mon, Thu 40+40m
    Nearest: Mon, Thu,  2*20m
    Garden: Daily 2*20
*/
/*
//Дальняя зона
schedule('{"time":{"exactTime":true,"start":"00:00"},"period":{"weeks":1,"dows":"[1,3]"}}', function() {StartWatering(gardenZoneFar, "Far Zone")});
schedule('{"time":{"exactTime":true,"start":"00:30"},"period":{"weeks":1,"dows":"[1,3]"}}', function() {StopWatering(gardenZoneFar, "Far Zone")});
// перерыв 40 мин
// Middle Zone
schedule('{"time":{"exactTime":true,"start":"01:10"},"period":{"weeks":1,"dows":"[1,3]"}}', function() {StartWatering(gardenZoneMiddle, "Middle Zone")});
schedule('{"time":{"exactTime":true,"start":"01:40"},"period":{"weeks":1,"dows":"[1,3]"}}', function() {StopWatering(gardenZoneMiddle, "Middle Zone")});
// перерыв 40 мин
//Дальняя зона
schedule('{"time":{"exactTime":true,"start":"02:20"},"period":{"weeks":1,"dows":"[1,3]"}}', function() {StartWatering(gardenZoneFar, "Far Zone")});
schedule('{"time":{"exactTime":true,"start":"03:00"},"period":{"weeks":1,"dows":"[1,3]"}}', function() {StopWatering(gardenZoneFar, "Far Zone")});
// перерыв 40 мин
// Middle Zone
schedule('{"time":{"exactTime":true,"start":"03:40"},"period":{"weeks":1,"dows":"[1,3]"}}', function() {StartWatering(gardenZoneMiddle, "Middle Zone")});
schedule('{"time":{"exactTime":true,"start":"04:10"},"period":{"weeks":1,"dows":"[1,3]"}}', function() {StopWatering(gardenZoneMiddle, "Middle Zone")});
// перерыв 40 мин
// Передняя зона
schedule('{"time":{"exactTime":true,"start":"04:50"},"period":{"weeks":1,"dows":"[1,3]"}}', function() {StartWatering(gardenZoneNear, "Near Zone")});
schedule('{"time":{"exactTime":true,"start":"05:10"},"period":{"weeks":1,"dows":"[1,3]"}}', function() {StopWatering(gardenZoneNear, "Near Zone")});
*/

//Дальняя зона
schedule('{"time":{"exactTime":true,"start":"00:00"},"period":{"weeks":1,"dows":"[1]"}}', function() {StartWatering(gardenZoneFar, "Far Zone")});
schedule('{"time":{"exactTime":true,"start":"00:30"},"period":{"weeks":1,"dows":"[1]"}}', function() {StopWatering(gardenZoneFar, "Far Zone")});
// перерыв 40 мин
// Middle Zone
schedule('{"time":{"exactTime":true,"start":"01:10"},"period":{"weeks":1,"dows":"[1]"}}', function() {StartWatering(gardenZoneMiddle, "Middle Zone")});
schedule('{"time":{"exactTime":true,"start":"01:40"},"period":{"weeks":1,"dows":"[1]"}}', function() {StopWatering(gardenZoneMiddle, "Middle Zone")});
// перерыв 40 мин
//Дальняя зона
schedule('{"time":{"exactTime":true,"start":"02:20"},"period":{"weeks":1,"dows":"[1]"}}', function() {StartWatering(gardenZoneFar, "Far Zone")});
schedule('{"time":{"exactTime":true,"start":"03:00"},"period":{"weeks":1,"dows":"[1]"}}', function() {StopWatering(gardenZoneFar, "Far Zone")});
// перерыв 40 мин
// Middle Zone
schedule('{"time":{"exactTime":true,"start":"03:40"},"period":{"weeks":1,"dows":"[1]"}}', function() {StartWatering(gardenZoneMiddle, "Middle Zone")});
schedule('{"time":{"exactTime":true,"start":"04:10"},"period":{"weeks":1,"dows":"[1]"}}', function() {StopWatering(gardenZoneMiddle, "Middle Zone")});
// перерыв 40 мин
// Передняя зона
schedule('{"time":{"exactTime":true,"start":"04:50"},"period":{"weeks":1,"dows":"[1]"}}', function() {StartWatering(gardenZoneNear, "Near Zone")});
schedule('{"time":{"exactTime":true,"start":"05:10"},"period":{"weeks":1,"dows":"[1]"}}', function() {StopWatering(gardenZoneNear, "Near Zone")});

//ежедневный полив
//Дальняя зона
//schedule('{"time":{"exactTime":true,"start":"02:01"},"period":{"weeks":1,"dows":"[0,2,4,5,6]"}}', function() {StartWatering(gardenZoneFar, "Far Zone")});
//schedule('{"time":{"exactTime":true,"start":"02:10"},"period":{"weeks":1,"dows":"[0,2,4,5,6]"}}', function() {StopWatering(gardenZoneFar, "Far Zone")});
// Middle Zone
//schedule('{"time":{"exactTime":true,"start":"02:11"},"period":{"weeks":1,"dows":"[0,2,4,5,6]"}}', function() {StartWatering(gardenZoneMiddle, "Middle Zone")});
//schedule('{"time":{"exactTime":true,"start":"02:20"},"period":{"weeks":1,"dows":"[0,2,4,5,6]"}}', function() {StopWatering(gardenZoneMiddle, "Middle Zone")});
// Передняя зона
//schedule('{"time":{"exactTime":true,"start":"02:21"},"period":{"weeks":1,"dows":"[0,2,4,5,6]"}}', function() {StartWatering(gardenZoneNear, "Near Zone")});
//schedule('{"time":{"exactTime":true,"start":"02:30"},"period":{"weeks":1,"dows":"[0,2,4,5,6]"}}', function() {StopWatering(gardenZoneNear, "Near Zone")});
// перерыв 40 мин


//огород
schedule('{"time":{"exactTime":true,"start":"00:00"},"period":{"weeks":1,"dows":"[0,2,4,5,6]"}}', function() {StartWatering(gardenZoneGarden, "Garden Zone")});
schedule('{"time":{"exactTime":true,"start":"00:30"},"period":{"weeks":1,"dows":"[0,2,4,5,6]"}}', function() {StopWatering(gardenZoneGarden, "Garden Zone")});
// перерыв 
schedule('{"time":{"exactTime":true,"start":"01:30"},"period":{"weeks":1,"dows":"[0,2,4,5,6]"}}', function() {StartWatering(gardenZoneGarden, "Garden Zone")});
schedule('{"time":{"exactTime":true,"start":"02:00"},"period":{"weeks":1,"dows":"[0,2,4,5,6]"}}', function() {StopWatering(gardenZoneGarden, "Garden Zone")});
// перерыв 
schedule('{"time":{"exactTime":true,"start":"03:00"},"period":{"weeks":1,"dows":"[0,2,4,5,6]"}}', function() {StartWatering(gardenZoneGarden, "Garden Zone")});
schedule('{"time":{"exactTime":true,"start":"03:30"},"period":{"weeks":1,"dows":"[0,2,4,5,6]"}}', function() {StopWatering(gardenZoneGarden, "Garden Zone")});

