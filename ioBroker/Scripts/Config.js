var prefixTopic = 'mqtt.0.Arduino_';
var prefixCfg = prefixTopic + '*.Config';

var reqConfPart = 'ToServer.ConfigurationRequest';
var reqConf = prefixCfg + '.' + reqConfPart;

var reqActPart = 'ToServer.ActionRequest';
var reqAct = prefixCfg + '.' + reqActPart;

// Rules: 
// 1. All buses units should be leaded Bus Definition first.
var Configurations = [
//         '{"id":1,"type":"B","Pin":23,"lhOn":0,"status":0}'
//        ,'{"id":2,"type":"R","Pin":22,"lhOn":0}'
//        ,'{"id":3,"type":"1","Pin":40,"cycle":120}' 
//        ,'{"id":4,"type":"T","Pin":40,"address":"28C46F33080000C6"}' 
//        ,'{"id":5,"type":"T","Pin":40,"address":"284F32340800002B"}' 
//        ,'{"id":6,"type":"Z","Pin":24}'
//        ,'{"id":7,"type":"D","Pin":25, "parent":6}'
        [
             '{"id":1, "type":"B","Pin":22,"lhOn":0,"status":0}'
            ,'{"id":2, "type":"B","Pin":23,"lhOn":0,"status":0}'
            ,'{"id":3, "type":"R","Pin":24,"lhOn":1,"status":0}'
            ,'{"id":4, "type":"R","Pin":25,"lhOn":1,"status":0}'
            ,'{"id":5, "type":"R","Pin":26,"lhOn":1,"status":0}'
        ]
        ,[
         '{"id":1,"type":"B","Pin":23,"lhOn":0,"status":0}'
//        ,'{"id":2,"type":"R","Pin":22,"lhOn":0}'
//        ,'{"id":3,"type":"1","Pin":40,"cycle":120}' 
//        ,'{"id":4,"type":"T","Pin":40,"address":"28C46F33080000C6"}' 
//        ,'{"id":5,"type":"T","Pin":40,"address":"284F32340800002B"}' 
//        ,'{"id":6,"type":"Z","Pin":24}'
//        ,'{"id":7,"type":"D","Pin":25, "parent":6}'
        ]
    ];
var Actions = [
        [
         '{"id":1,"originId":1,"originType":"B","event":1,"targetId":3,"targetAction":9}'
         ,'{"id":2,"originId":1,"originType":"B","event":2,"targetId":4,"targetAction":9}'
         ,'{"id":3,"originId":2,"originType":"B","event":1,"targetId":5,"targetAction":9}'
        ]
        ,[
         '{"id":1,"board":"00","originId":1,"originType":"B","event":1,"targetId":2,"targetAction":9}'
//        ,'{"id":2,"board":"00","originId":1,"originType":"B","event":3,"targetId":2,"targetAction":9}'

        ]
    ];

var cacheConfigState = $(reqConf);
cacheConfigState.on (function(obj) {
   log("Request for config");
   var configTopic = obj.id;
   var boardId = parseInt(configTopic.substr(prefixTopic.length, 2));
   log("Board:" + boardId);
   configTopic = configTopic.replace(reqConfPart, 'ToCU.Configuration');
   log ("Config topic:" + configTopic);
    var configLength = Configurations[boardId].length;
    setState(configTopic, '{"length":' + configLength +'}');
    
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
   var boardId = parseInt(actTopic.substr(prefixTopic.length, 2));
   log("Board:" + boardId);
   actTopic = actTopic.replace(reqActPart, 'ToCU.Action');
//   log ("Action topic:" + actTopic);
    var actLength = Actions[boardId].length;
    
    setState(actTopic, '{"length":' + actLength +'}');
    
    for (var i=0; i< actLength; i++) {
        setState(actTopic,Actions[boardId][i]);
    }
    
   log("End Action Request"); 
});
