
var reqSt = prefixBrd + '*.' + reqState;
var cacheReqSt = $(reqSt);
cacheReqSt.on (function(obj) {

    var reqTopic = obj.id;
    var b = obj.state.val.replace(/\//g, '.')
    var objName = "mqtt.0." + b;
    var v = '';
    if (existsState(objName)) {
        v = getState(objName).val.toString();
    }
    var msg = {
        "topic" : reqTopic.replace('mqtt.0.', '').replace(/\./g, '/'),//obj.state.val.replace('/', '.');
        "message" : v
    };
    sendTo("mqtt.0", "sendMessage2Client", msg);    
});
