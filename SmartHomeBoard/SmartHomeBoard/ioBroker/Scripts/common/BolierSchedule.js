
schedule ("05 00 * * *", function() {StartBolier(0);});
schedule ("06 50 * * *", function() {StopBolier(0);});

// Stop All at 6:55
schedule ("06 55 * * *", function() {StopBolier(0);});


function StartBoiler(boilerNumber) {
    log ("Start boiler: " + boilerNumber);
    
    setState(boilers[boilerNumber],1,false);
}

function StopBoiler(boilerNumber) {
    log ("Stop boiler: " + boilerNumber);
    
    setState(boilers[boilerNumber],0,false);
}

//StopBoiler(0);