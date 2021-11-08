/*
schedule ("00 00 * * *", function() {Charging(1);});
schedule ("00 05 * * *", function() {Charging(0);});
schedule ("00 08 * * *", function() {Charging(1);});
schedule ("00 13 * * *", function() {Charging(0);});
schedule ("00 16 * * *", function() {Charging(1);});
schedule ("00 21 * * *", function() {Charging(0);});
*/
 
schedule('{"time":{"exactTime":true,"start":"00:00"},"period":{"days":1}}', function() {Charging(1);});
schedule('{"time":{"exactTime":true,"start":"05:00"},"period":{"days":1}}', function() {Charging(0);});

schedule('{"time":{"exactTime":true,"start":"08:00"},"period":{"days":1}}', function() {Charging(1);});
schedule('{"time":{"exactTime":true,"start":"13:00"},"period":{"days":1}}', function() {Charging(0);});

schedule('{"time":{"exactTime":true,"start":"16:00"},"period":{"days":1}}', function() {Charging(1);});
schedule('{"time":{"exactTime":true,"start":"21:00"},"period":{"days":1}}', function() {Charging(0);});


function Charging(onOff) {
    if (onOff === 0) {
        log ("Stop charging ");
    } else {
        log ("Start charging ");
        
    }
    
    setState(accumRelay,onOff,false);
}
