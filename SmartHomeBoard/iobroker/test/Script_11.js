
function resetDevice1(pin, devName) {
    var isPinReady = false;
    log("Reset device:" + devName +"["+pin+"]("+relayOff+")", 'debug');
    var gpioPin = gpio.export(pin,{ direction: 'out', 'ready' : function(err) {
        isPinReady=true;
        log("ready 1:" +err +"#"); }, 'timeout':10});
    log("point1");
    for (var i=0; i<500 && !isPinReady; i++) 
    {
        log("Attempt:"+i);
        setTimeout(function(){

        },100);
    }
    
    gpioPin.set(1);
    log("Delay for 10 sec");
    /*
    setTimeout(function(){
        log("Start device:"+devName+"("+(1-relayOff)+")", 'error');
        var gpioPin = gpio.export(pin,{ direction: 'out', 'ready' : function(err) {
            log("ready 2:" +err +"#");
        }});
            gpioPin.set(0);
    }, 1000);
   */
}

//var gpioPin26 = gpio.export("26",{ direction: 'out', 'ready' : function(err) {


resetDevice1("26","www")