$(vbSwitchOff).on(function(obj){
    //log("obj_val:" + obj.state.val);
    if (obj.state.val > 0) { 
        if (obj.state.val == 4) { //switch off all
            var boardNumber = Configurations.length;
            for (var b=0; b < boardNumber; b++) {    
                var configLength = Configurations[b].length;
                for (var i=0; i< configLength; i++) {
                    if (Configurations[b][i] !== null) {
                        var conf = JSON.parse(Configurations[b][i]);
                        if (conf.priority != 'undefined') {
                            if (conf.priority<=SwitchOffPriority) {
                                var isApplicable = false;
                                var devName = "";//prefixEquip;
                                if (conf.type=="R") {
                                    isApplicable=true;
                                    devName = prefixRelays + ".R";
                                } else if (conf.type=="B") {
                                    devName = prefixButtons + ".B";
                                } else if (conf.type=="b") {
                                    devName+="Virtual.Buttons.b";
                                }
                                devName += (b<10? "0": "")+(conf.id<100? "0"+conf.id : conf.id); 
                                //log("DevName=" + devName);
                                if (isApplicable) {
                                    log("Switch off:" + devName);
                                    setState(devName,0); 
                                }
                            } 
                        }
                    }
                }
            }
        }
        setState(vbSwitchOff,0); // back this button to the default state
    }    
});