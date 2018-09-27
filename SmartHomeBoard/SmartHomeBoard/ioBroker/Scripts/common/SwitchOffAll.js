$("mqtt.0.Equipment.Virtual.Buttons.B9901").on(function(obj){
    var boardNumber = Configurations.length;
    //log("Boards=" + boardNumber);
    for (var b=0; b < boardNumber; b++) {    
    
        var configLength = Configurations[b].length;
        //log("ConfigLength=" + configLength);
        for (var i=0; i< configLength; i++) {
            if (Configurations[b][i] !== null) {
                var conf = JSON.parse(Configurations[b][i]);
                //log("Conf="+JSON.stringify(conf));
                if (conf.priority != 'undefined') {
                    if (conf.priority<=SwitchOffPriority) {
                        var isApplicable = false;
                        var devName = "mqtt.0.Equipment.";
                        if (conf.type=="R") {
                            isApplicable=true;
                            devName+="Relays.R";
                        } else if (conf.type=="B") {
                            devName+="Relays.B";
                        }
                        devName += (b<10? "0": "")+(conf.id<100? "0"+conf.id : conf.id); 
                        //log("DevName=" + devName);
                        if (isApplicable) {
                           setState(devName,0); 
                        }
                    }
                } 
            }
        }
    }
 
});