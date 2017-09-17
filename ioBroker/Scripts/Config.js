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
             '{"id":13, "type":"B","Pin":13,"lhOn":0,"status":0}' //WC 1
            ,'{"id":12, "type":"B","Pin":12,"lhOn":0,"status":0}' //Lobby1
			
            ,'{"id":22, "type":"R","Pin":22,"lhOn":1,"status":0}' 	//L1.1
            ,'{"id":23, "type":"R","Pin":23,"lhOn":1,"status":0}'
			,'{"id":24, "type":"R","Pin":24,"lhOn":1,"status":0}'	//L1.2
            ,'{"id":25, "type":"R","Pin":25,"lhOn":1,"status":0}'
            ,'{"id":26, "type":"R","Pin":26,"lhOn":1,"status":0}'	//L1.5
            ,'{"id":27, "type":"R","Pin":27,"lhOn":1,"status":0}'
			,'{"id":28, "type":"R","Pin":28,"lhOn":1,"status":0}'	//L1.6
			,'{"id":29, "type":"R","Pin":29,"lhOn":1,"status":0}'
            ,'{"id":30, "type":"R","Pin":30,"lhOn":1,"status":0}'	//L1.3
			,'{"id":31, "type":"R","Pin":31,"lhOn":1,"status":0}'
            ,'{"id":32, "type":"R","Pin":32,"lhOn":1,"status":0}'	//L1.4
			,'{"id":33, "type":"R","Pin":33,"lhOn":1,"status":0}'
            ,'{"id":34, "type":"R","Pin":34,"lhOn":1,"status":0}'	//L1.8
			,'{"id":35, "type":"R","Pin":35,"lhOn":1,"status":0}'
            ,'{"id":36, "type":"R","Pin":36,"lhOn":1,"status":0}'	//L1.7
			,'{"id":37, "type":"R","Pin":37,"lhOn":1,"status":0}'
            ,'{"id":38, "type":"R","Pin":38,"lhOn":1,"status":0}'	//L3.2
			,'{"id":39, "type":"R","Pin":39,"lhOn":1,"status":0}'
            ,'{"id":40, "type":"R","Pin":40,"lhOn":1,"status":0}'	//L3.1
			,'{"id":41, "type":"R","Pin":41,"lhOn":1,"status":0}'
            ,'{"id":42, "type":"R","Pin":42,"lhOn":1,"status":0}'	//L3.4
			,'{"id":43, "type":"R","Pin":43,"lhOn":1,"status":0}'
            ,'{"id":44, "type":"R","Pin":44,"lhOn":1,"status":0}'	//L3.3
			,'{"id":45, "type":"R","Pin":45,"lhOn":1,"status":0}'
            ,'{"id":46, "type":"R","Pin":46,"lhOn":1,"status":0}'	//L3.5
            ,'{"id":47, "type":"R","Pin":47,"lhOn":1,"status":0}'
            ,'{"id":48, "type":"R","Pin":48,"lhOn":1,"status":0}'	//L3.6
            ,'{"id":49, "type":"R","Pin":49,"lhOn":1,"status":0}'
			,'{"id":50, "type":"R","Pin":50,"lhOn":1,"status":0}'	//L3.7
			,'{"id":51, "type":"R","Pin":51,"lhOn":1,"status":0}'
			,'{"id":52, "type":"R","Pin":52,"lhOn":1,"status":0}'	//L3.8
			,'{"id":53, "type":"R","Pin":53,"lhOn":1,"status":0}'

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
	
/*
	ACT_NO_ACTION = 0,
	ACT_RELAY_ON = 7,
	ACT_RELAY_OFF = 8,
	ACT_RELAY_SWITCH = 9,
	ACT_SENSOR_READY = 10

	BTN_OFF = 0,
	BTN_ON = 1,
	BTN_LONG = 2,
	BTN_SHORT_LONG = 3,
	BTN_EXTRA_LONG = 4

*/	
var Actions = [
        [
         '{"id":1,"originId":13,"originType":"B","event":1,"targetId":48,"targetAction":9}' 
         ,'{"id":2,"originId":13,"originType":"B","event":1,"targetId":46,"targetAction":9}'
         ,'{"id":3,"originId":13,"originType":"B","event":2,"targetId":46,"targetAction":9}'
//         ,'{"id":3,"originId":2,"originType":"B","event":1,"targetId":5,"targetAction":9}'
//         ,'{"id":4,"originId":1,"originType":"B","event":4,"targetId":3,"targetAction":8}'
//         ,'{"id":5,"originId":1,"originType":"B","event":4,"targetId":4,"targetAction":8}'
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
