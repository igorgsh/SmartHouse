/* -- do not edit following lines - START --
{
  "debug": false,
  "verbose": false
}
-- do not edit previous lines - END --*/
    var relayOff = 1;


    var DevConfig=[];
    DevConfig["Board_00"]=20;
    DevConfig["Board_01"]=21;
    DevConfig["Board_02"]=26;
    //DevConfig.Phone=19;

    var gpioArray = [];
//    var a=0;
    var gpio = require("gpio");



function resetDevice(pin, devName) {
    log("Reset device:" + devName +"["+pin+"]("+relayOff+")", 'error');
    var gpioPin = gpio.export(pin,{ direction: 'out',
        'ready' : function() {
            
            gpioPin.set(relayOff);
            log("Delay for 10 sec");
            setTimeout(function(){
                log("Start device:"+devName+"("+(1-relayOff)+")", 'error');
                gpioPin.set(1-relayOff);
            }, 10000);
            
        }
    });
}

 


function resetBoard(boardIdstr) {

    var boardName = "Board_"+boardIdstr;
    if (DevConfig[boardName] !== null) {
        resetDevice(DevConfig[boardName], boardName);
    }
}

/*
function initResetGPIO() {
    for(var prop in DevConfig) {
        var pin = DevConfig[prop];
        log("pin=" + pin);
        if (gpioArray[pin]!==null) {
            gpioArray[pin] = gpio.export(pin,{ direction: 'out'}); 
            log("Pin:" + pin + "init");
        }
    }
}

initResetGPIO();
*/