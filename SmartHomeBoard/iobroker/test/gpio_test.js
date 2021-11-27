
    var gpio = require("gpio");
    var gpioPin;

function resetDevice1(pin, devName) {
    log("Reset device:" + devName +"["+pin+"]("+relayOff+")", 'error');
    gpioPin = gpio.export(pin,{ direction: gpio.DIRECTION.OUT, 'ready' : function() {
        log("Point2: " +JSON.stringify(gpioPin));
        gpioPin.set(0);     
        /*   
        log("Delay for 5 sec");
        setTimeout(function(){
            log("Start device:"+devName+"("+(1-relayOff)+")", 'error');
            gpioPin.set(1-relayOff);
        }, 10000);
        */
        
        }});
}

var relayOff = 1;  
log("Point1=" + JSON.stringify(gpioPin));
resetDevice1("16","www")
        log("Delay for 10 sec");
        setTimeout(function(){
            log("TEST");
            log("Test2: " +JSON.stringify(gpioPin));
            gpioPin.set(0);
        }, 10000);

/* 
var gpio = require ("gpio");
gpio.export()
gpio.open(26, "output", function(err) {     
gpio.write(26, 1, function() {          
        gpio.close(26);                     
    });
}); 
*/