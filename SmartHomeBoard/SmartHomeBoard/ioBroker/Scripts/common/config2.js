/* -- do not edit following lines - START --
{
  "debug": false,
  "verbose": false
}
-- do not edit previous lines - END --*/

var reqConfPart = 'ConfigurationRequest';
var reqConf = prefixCfg + '*.' + reqConfPart;

var reqActPart = 'ActionRequest';
var reqAct = prefixCfg + '*.' + reqActPart;
//log("reqConf=" +reqConf);
var cacheConfigState = $(reqConf);
cacheConfigState.on (function(obj) {
    
//   log("Request for config");
   var configTopic = obj.id;
   log("Request:" + configTopic);
   var boardIdStr = configTopic.substr(prefixCfg.length, 2);
   var boardId = parseInt(boardIdStr);
   
   configTopic = prefixCfg + boardIdStr + ".Configuration";
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


var cacheActState = $(reqAct);
cacheActState.on (function(obj) {
    log("Request for actions");
    var actTopic = obj.id;
    var boardIdStr = actTopic.substr(prefixCfg.length, 2);
    var boardId = parseInt(boardIdStr);
    log("Board:" + boardIdStr);
    actTopic = prefixCfg + boardIdStr + '.Action';
//   log ("Action topic:" + actTopic);
    var actLength = Actions[boardId].length;
    
    setState(actTopic, '{"length":' + actLength +'}');
    log ("Action length = " + actLength);    
    for (var i=0; i< actLength; i++) {
        setState(actTopic,Actions[boardId][i]);
    }
    
   log("End Action Request"); 
});
