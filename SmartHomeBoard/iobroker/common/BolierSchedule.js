
// 1 этаж
// Ежедневно 23:00 - 01:00 
schedule ('{"time":{"exactTime":true,"start":"23:00"},"period":{"days":1}}', function(){StartBoiler(0);});
schedule ('{"time":{"exactTime":true,"start":"01:00"},"period":{"days":1}}', function(){StopBoiler(0);});


// Sat-Sun add 2 hour 
// дополнительно 16:00-18:00

schedule ('{"time":{"exactTime":true,"start":"16:00"},"period":{"weeks":1,"dows":"[0,6]"}}', function(){StartBoiler(0);});
schedule ('{"time":{"exactTime":true,"start":"18:00"},"period":{"weeks":1,"dows":"[0,6]"}}', function(){StopBoiler(0);});



// Stop All at 6:55 and 22:55

schedule ('{"time":{"exactTime":true,"start":"06:55"},"period":{"days":1}}', function(){StopBoiler(0);});
schedule ('{"time":{"exactTime":true,"start":"06:55"},"period":{"days":1}}', function(){StopBoiler(1);});
schedule ('{"time":{"exactTime":true,"start":"06:55"},"period":{"days":1}}', function(){StopBoiler(2);});

schedule ('{"time":{"exactTime":true,"start":"22:55"},"period":{"days":1}}', function(){StopBoiler(0);});
schedule ('{"time":{"exactTime":true,"start":"22:55"},"period":{"days":1}}', function(){StopBoiler(1);});
schedule ('{"time":{"exactTime":true,"start":"22:55"},"period":{"days":1}}', function(){StopBoiler(2);});


//2 этаж 
//главный бойлер
//Ежедневно 01:00 - 03:00

schedule ('{"time":{"exactTime":true,"start":"01:00"},"period":{"days":1}}', function(){StartBoiler(2);});
schedule ('{"time":{"exactTime":true,"start":"03:00"},"period":{"days":1}}', function(){StopBoiler(2);});


//первый бойлер 
// Fri, Sat, Sun 03:00-05:00

schedule ('{"time":{"exactTime":true,"start":"03:00"},"period":{"weeks":1,"dows":"[0,5,6]"}}', function(){StartBoiler(1);});
schedule ('{"time":{"exactTime":true,"start":"05:00"},"period":{"weeks":1,"dows":"[0,5,6]"}}', function(){StopBoiler(1);});


function StartBoiler(boilerNumber) {
    log ("Start boiler: " + boilerNumber);
    
    setState(boilers[boilerNumber],1,false);
}

function StopBoiler(boilerNumber) {
    log ("Stop boiler: " + boilerNumber);
    
    setState(boilers[boilerNumber],0,false);
}

$(BoilerSwitch).on(function(obj){
    //log("obj_val:" + obj.state.val);
    if (obj.state.val === 0 ) { // turn off all
        //log("Switch off 1");
        //log("Switch off 2");
        setState(boilers[1], 0, false);
        setState(boilers[2], 0, false);
    } else if (obj.state.val == 1 ){
        //log("Switch off 2");
        //log("Switch ON 1");

        setState(boilers[2], 0, false);
        setState(boilers[1], 1, false);
    } else if (obj.state.val == 2 ){
       //log("Switch off 1");
        //log("Switch ON 2");
        setState(boilers[1], 0, false);
        setState(boilers[2], 1, false);
    }
});    
