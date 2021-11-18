schedule ('{"time":{"exactTime":true,"start":"10:00"},"period":{"days":1}}', function(){ChangeCooling(1);});
schedule ('{"time":{"exactTime":true,"start":"10:30"},"period":{"days":1}}', function(){ChangeCooling(0);});

schedule ('{"time":{"exactTime":true,"start":"12:00"},"period":{"days":1}}', function(){ChangeCooling(1);});
schedule ('{"time":{"exactTime":true,"start":"12:30"},"period":{"days":1}}', function(){ChangeCooling(0);});

schedule ('{"time":{"exactTime":true,"start":"14:00"},"period":{"days":1}}', function(){ChangeCooling(1);});
schedule ('{"time":{"exactTime":true,"start":"14:30"},"period":{"days":1}}', function(){ChangeCooling(0);});

schedule ('{"time":{"exactTime":true,"start":"16:00"},"period":{"days":1}}', function(){ChangeCooling(1);});
schedule ('{"time":{"exactTime":true,"start":"16:30"},"period":{"days":1}}', function(){ChangeCooling(0);});

//schedule ('{"time":{"exactTime":true,"start":"05:00"},"period":{"days":1}}', function(){ChangeCooling(1);});
//schedule ('{"time":{"exactTime":true,"start":"06:30"},"period":{"days":1}}', function(){ChangeCooling(0);});

//schedule ('{"time":{"exactTime":true,"start":"08:00"},"period":{"days":1}}', function(){ChangeCooling(1);});
//schedule ('{"time":{"exactTime":true,"start":"08:30"},"period":{"days":1}}', function(){ChangeCooling(0);});

function ChangeCooling(mode){
    var isCooling = getState(vbSwitchCooling);
    log ("Cooling=" + isCooling.val + " mode=" + mode);
    if (isCooling.val == 1) { //Allow
        if (mode == 1) {//Switch On
            setState(geoPump,true, false);
            setState(hvacPump1,1, false);
            setState(hvacPump2,1, false);
        } else { //switch off
            setState(geoPump,false, false);
        }
    } else {
        setState(geoPump,false, false);
    }
}

$(vbSwitchCooling).on(function(obj){
    log ("Cooling!!!=" + obj.state.val);
    ChangeCooling(obj.state.val);
});

//ChangeCooling(1);
//ChangeCooling(0);


