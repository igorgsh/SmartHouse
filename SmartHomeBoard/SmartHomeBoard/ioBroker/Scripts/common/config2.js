/* -- do not edit following lines - START --
{
  "debug": false,
  "verbose": false
}
-- do not edit previous lines - END --*/

var reqConf = prefixBrd + '*.' + reqConfPart;
var cacheConfigState = $(reqConf);
cacheConfigState.on (function(obj) {
    
   var configTopic = obj.id;
   log("Request:" + configTopic);
   var boardIdStr = configTopic.substr(prefixBrd.length, 2);
   var boardId = parseInt(boardIdStr);
   
   configTopic = prefixBrd + boardIdStr + ".Configuration";
   //log ("Config topic:" + configTopic);
   
    var configLength = Configurations[boardId].length;
    setState(configTopic, '{"length":' + configLength +'}');
    log("Config length=" + configLength);
    for (var i=0; i< configLength; i++) {
        //log(JSON.stringify(JSON.parse(Configurations[i])));
        
        setState(configTopic,Configurations[boardId][i]);
    }
    
   log("End Config Request"); 
});

var reqAct = prefixBrd + '*.' + reqActPart;

var cacheActState = $(reqAct);
cacheActState.on (function(obj) {
    log("Request for actions");
    var actTopic = obj.id;
    var boardIdStr = actTopic.substr(prefixBrd.length, 2);
    var boardId = parseInt(boardIdStr);
    log("Board:" + boardIdStr);
    actTopic = prefixBrd + boardIdStr + '.Action';
//   log ("Action topic:" + actTopic);
    var actLength = Actions[boardId].length;
    
    setState(actTopic, '{"length":' + actLength +'}');
    log ("Action length = " + actLength);    
    for (var i=0; i< actLength; i++) {
        setState(actTopic,Actions[boardId][i]);
    }
    
   log("End Action Request"); 
});

