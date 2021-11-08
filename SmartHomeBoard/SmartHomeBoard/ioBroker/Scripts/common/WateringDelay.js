$(gardenWateringDelay).on(function(obj){
    if (obj.state.val===true) {
        //log("Delay in watering!");
        var now = new Date();
        now.setMilliseconds(0);
        var delayInterval = 12;
        var DelayTime = new Date(now.getTime() + delayInterval*3600*1000);
        var ts = DelayTime.getTime();
        setState(gardenWateringDelayTimeStamp, ts);
    }
});

$(gardenWateringDelayTimeStamp).on(function(obj){

    var DelayDate = new Date(obj.state.val);
    setState(gardenWateringDelayTime,formatTimeStamp(DelayDate));
}); 



$(gardenWateringDelayTime).on(function(obj){
    var dt = parseTimeStamp(obj.state.val);
    setState(gardenWateringDelayTimeStamp, dt.getTime());
});

function parseTimeStamp(ts) {
    
    var parts = ts.split(" ");
    var dtParts = parts[0].split("-");
    var timeParts = parts[1].split(":");
    
    var res = new Date(dtParts[0],dtParts[1]-1,dtParts[2], timeParts[0], timeParts[1], timeParts[2] );
    return res;
    
}

function formatTimeStamp(dt) {
    var res = "";

    res += dt.getFullYear() + "-";
    var m = dt.getMonth() + 1;
    if (m<10) {
        res+="0";
    }
    res +=m + "-";
    
    var d = dt.getDate();
    if (d<10) {
        res+="0";
    }
    res +=d + " ";


    
    var h=dt.getHours();
    if (h<10) {
        res+="0";
    }
    res += h + ":";

    var mm=dt.getMinutes();
    if (mm<10) {
        res+="0";
    }
    res += mm + ":";
    
    var s=dt.getSeconds();
    if (s<10) {
        res+="0";
    }
    res += s;

    return res;
}