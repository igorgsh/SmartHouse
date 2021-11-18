function CheckMotionLight() {
    log("Point 1");
    var mon = ZMroot + "." + monitorParking;
    var objMon = getState(mon);
    var tm= new Date();
    var tDiff = tm.getTime() - objMon.ts;
    log("diff=" + tDiff + "; t1=" + tm.getTime() + "; t2=" + objMon.ts);
    if (tDiff > 5 * 60 * 1000) {
        log("Point 4");
    }
};

function LightOn(monitor) {
    var sched = false;

    switch (monitor) {
        case monitorParking:
             setState(lightParking,1);
             sched = true;
        break;
    }
    if (sched) {
       //schedule() 
        var tm= new Date();
        var tm1 = new Date(tm.getTime() + 1 * 60 * 1000);
        var schedObj = {"time":{"exactTime":true,"start":"13:21"},"period":{"once":"15.10.2021"}};

        schedObj.time.start = tm1.getHours() + ":" +tm1.getMinutes();
        schedObj.period.once = tm1.getDate() + "." + (tm1.getMonth()+1) + "." + tm1.getFullYear(); 
        log(JSON.stringify(schedObj));
 
        schedule(schedObj, function() {CheckMotionLight()});
    }
};

function CheckLightOn(monitor) {
    log("Point 3");
    if (!isAstroDay()) { //switch light on
        log("Point 2");
        LightOn(monitor);
    }
};

$(ZMMonitors).on(function(obj){ 
    log("obj_val:" + obj.state.val);
    log("ts=" + obj.state.ts );
    log("Now=" + Date.now());
    log("obj=" + obj.name);
    // Check is light should switch on
    var zmObject = JSON.parse(obj.state.val);
    log("monitor="+ zmObject.monitor);
    
    CheckLightOn(zmObject.monitor);
});


//CheckLightOn(6);
//schedule('{"time":{"start":"00:00","end":"23:59","mode":"minutes","interval":3},"period":{"days":1}}', function() {CheckMotionLight()});
CheckMotionLight();






