
var logTp = prefixLog + '*.*';
var cacheLog = $(logTp);
cacheLog.on (function(obj) {

    var topic = obj.id;
    var board = topic.substring(prefixLog.length-6);
    var ext = topic.substring(prefixLog.length+3);

    log(board + "#" + obj.state.val);
});