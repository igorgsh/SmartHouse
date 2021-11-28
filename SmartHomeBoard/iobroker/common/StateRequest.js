var reqSt = prefixConfig + '.Common.' + reqState;
var cacheReqSt = $(reqSt);
cacheReqSt.on (function(obj) {
    //var reqTopic = obj.id;
    var b = obj.state.val.replace(/\//g, '.');
    var objName = "mqtt.0." + b;
    //log ("Requested state for:" + objName);
    var v = '';
    if (existsState(objName)) {
        v = getState(objName).val.toString();        
    }
    var msg = {
        "topic" : objName.replace('mqtt.0.', '').replace(/\./g, '/'),//obj.state.val.replace('/', '.');
        "message" : v
    };
    //log("Send msg=" + JSON.stringify(msg));
    sendTo("mqtt.0", "sendMessage2Client", msg);    
});