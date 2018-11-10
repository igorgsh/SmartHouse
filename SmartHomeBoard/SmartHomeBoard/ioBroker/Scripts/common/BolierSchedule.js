
schedule ("00 05 * * *", function() {StartBoiler(0);});
schedule ("50 06 * * *", function() {StopBoiler(0);});

// Stop All at 6:55
schedule ("55 06 * * *", function() {StopBoiler(0);});


function StartBoiler(boilerNumber) {
    log ("Start boiler: " + boilerNumber);
    
    setState(boilers[boilerNumber],1,false);
}

function StopBoiler(boilerNumber) {
    log ("Stop boiler: " + boilerNumber);
    
    setState(boilers[boilerNumber],0,false);
}

//StopBoiler(0);