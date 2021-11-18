
function checkStates01() {
    var dev = pingGrp + '.192_168_0_91';
    var NetworkState = getState(dev).val;
    log ("State Of Network is: " + NetworkState);
    
    $(pingGrp+'.*').each(function(id,i){
        var devName = getObject(id).common.name.substr(6);
        log("Device="+devName+"#");
    });
}

checkStates01();