var prefixTopic = 'mqtt.0.Arduino_';
var prefixCfg = prefixTopic + '*.Config';

var reqConfPart = 'ToServer.ConfigurationRequest';
var reqConf = prefixCfg + '.' + reqConfPart;

var reqActPart = 'ToServer.ActionRequest';
var reqAct = prefixCfg + '.' + reqActPart;

// Rules: 
// 1. All buses units should be leaded Bus Definition first.
//         '{"id":1,"type":"B","Pin":23,"lhOn":0,"status":0}'
//        ,'{"id":2,"type":"R","Pin":22,"lhOn":0}'
//        ,'{"id":3,"type":"1","Pin":40,"cycle":120}' 
//        ,'{"id":4,"type":"T","Pin":40,"address":"28C46F33080000C6"}' 
//        ,'{"id":5,"type":"T","Pin":40,"address":"284F32340800002B"}' 
//        ,'{"id":6,"type":"Z","Pin":24}'
//        ,'{"id":7,"type":"D","Pin":25, "parent":6}'
// 2. Pins 4, 10, 13, 50,51,52,53 should be skipped
var Configurations = [
    [

        ///////             '{"id":13, "type":"B","Pin":13,"lhOn":0,"status":0}' // 
        '{"id":12, "type":"B","Pin":12,"lhOn":0,"status":0}' // 
        , '{"id":11, "type":"B","Pin":11,"lhOn":0,"status":0}' // �������
        /////////            //,'{"id":10, "type":"B","Pin":10,"lhOn":0,"status":0}' // 
        , '{"id":9, "type":"B","Pin":9,"lhOn":0,"status":0}' // �������� ����

        , '{"id":7, "type":"B","Pin":7,"lhOn":0,"status":0}' // ������� 
        , '{"id":6, "type":"B","Pin":6,"lhOn":0,"status":0}' // �� 1
        , '{"id":5, "type":"B","Pin":5,"lhOn":0,"status":0}' // ������� �������
        /////            ,'{"id":4, "type":"B","Pin":4,"lhOn":0,"status":0}' // 
        , '{"id":3, "type":"B","Pin":3,"lhOn":0,"status":0}' // �����, ����� �����

        , '{"id":14, "type":"B","Pin":14,"lhOn":0,"status":0}' // ???
        , '{"id":15, "type":"B","Pin":15,"lhOn":0,"status":0}' // ������� �������
        , '{"id":16, "type":"B","Pin":16,"lhOn":0,"status":0}' // ����� ����� ������������
        , '{"id":17, "type":"B","Pin":17,"lhOn":0,"status":0}' // �����
        , '{"id":19, "type":"B","Pin":19,"lhOn":0,"status":0}' // �������� �����
        , '{"id":20, "type":"B","Pin":20,"lhOn":0,"status":0}' // �������� ������


        , '{"id":22, "type":"R","Pin":22,"lhOn":1,"status":0}' 	//L1.1 ������� 1. ������
        , '{"id":23, "type":"R","Pin":23,"lhOn":1,"status":0}'   //L2.1 
        , '{"id":24, "type":"R","Pin":24,"lhOn":1,"status":0}'	//L1.2 ������� 1. �����
        , '{"id":25, "type":"R","Pin":25,"lhOn":1,"status":0}'   //L2.2 �����, ����� ������ � ������
        , '{"id":26, "type":"R","Pin":26,"lhOn":1,"status":0}'	//L1.3 
        , '{"id":27, "type":"R","Pin":27,"lhOn":1,"status":0}'   //L2.3 ����� �����
        , '{"id":28, "type":"R","Pin":28,"lhOn":1,"status":0}'	//L1.4 ��������
        , '{"id":29, "type":"R","Pin":29,"lhOn":1,"status":0}'   //L2.4
        , '{"id":30, "type":"R","Pin":30,"lhOn":1,"status":0}'	//L1.5 ������ ????
        , '{"id":31, "type":"R","Pin":31,"lhOn":1,"status":0}'   //L2.5
        , '{"id":32, "type":"R","Pin":32,"lhOn":1,"status":0}'	//L1.6 �������� ��� ������
        , '{"id":33, "type":"R","Pin":33,"lhOn":1,"status":0}'   //L2.6 ������� ������� ????
        , '{"id":34, "type":"R","Pin":34,"lhOn":1,"status":0}'	//L1.7 ������� ��������
        , '{"id":35, "type":"R","Pin":35,"lhOn":1,"status":0}'   //L2.7
        , '{"id":36, "type":"R","Pin":36,"lhOn":1,"status":0}'	//L1.8
        , '{"id":37, "type":"R","Pin":37,"lhOn":1,"status":0}'   //L2.8
        , '{"id":38, "type":"R","Pin":38,"lhOn":1,"status":0}'	//L3.5 ��1 ���
        , '{"id":39, "type":"R","Pin":39,"lhOn":1,"status":0}'   //L3.4 �����
        , '{"id":40, "type":"R","Pin":40,"lhOn":1,"status":0}'	//L3.6 ��1 ����
        , '{"id":41, "type":"R","Pin":41,"lhOn":1,"status":0}'   //L3.3 ������� �����
        , '{"id":42, "type":"R","Pin":42,"lhOn":1,"status":0}'	//L3.7 ����� �� ��� ������
        , '{"id":43, "type":"R","Pin":43,"lhOn":1,"status":0}'   //L3.2 ������� ������
        , '{"id":44, "type":"R","Pin":44,"lhOn":1,"status":0}'	//L3.8 ����� ��-������
        , '{"id":45, "type":"R","Pin":45,"lhOn":1,"status":0}'   //L3.1

    ]
    , [
        '{"id":112, "type":"B","Pin":12,"lhOn":0,"status":0}' // �������� �����
        , '{"id":111, "type":"B","Pin":11,"lhOn":0,"status":0}' // �������� ��
        , '{"id":109, "type":"B","Pin":9,"lhOn":0,"status":0}' // C�2
        , '{"id":108, "type":"B","Pin":8,"lhOn":0,"status":0}'// ������ �������
        , '{"id":107, "type":"B","Pin":7,"lhOn":0,"status":0}'// ����� �������
        , '{"id":106, "type":"B","Pin":6,"lhOn":0,"status":0}'// ������� �������

        , '{"id":122, "type":"R","Pin":22,"lhOn":1,"status":0}' 	//L4.1 ��2 ��� ���������
        , '{"id":123, "type":"R","Pin":23,"lhOn":1,"status":0}'   //L6.1 �������� �����
        , '{"id":124, "type":"R","Pin":24,"lhOn":1,"status":0}'	//L4.2
        , '{"id":125, "type":"R","Pin":25,"lhOn":1,"status":0}' //L6.2 ��������, ������
        , '{"id":126, "type":"R","Pin":26,"lhOn":1,"status":0}'	//L4.3
        , '{"id":127, "type":"R","Pin":27,"lhOn":1,"status":0}'  //L6.3 ����� �����
        , '{"id":128, "type":"R","Pin":28,"lhOn":1,"status":0}'	//L4.4 ������� ������� ����� �������
        , '{"id":129, "type":"R","Pin":29,"lhOn":1,"status":0}'  //L6.4
        , '{"id":130, "type":"R","Pin":30,"lhOn":1,"status":0}'	//L4.5 ������� ������� ����� �����
        , '{"id":131, "type":"R","Pin":31,"lhOn":1,"status":0}'  //L6.5 ����� ������
        , '{"id":132, "type":"R","Pin":32,"lhOn":1,"status":0}'	//L4.6 ����� ������� ������
        , '{"id":133, "type":"R","Pin":33,"lhOn":1,"status":0}'  //L6.6 ��2 ��� ������
        , '{"id":134, "type":"R","Pin":34,"lhOn":1,"status":0}'	//L4.7
        , '{"id":135, "type":"R","Pin":35,"lhOn":1,"status":0}'  //L6.7 ������ ������� ��� ������
        , '{"id":136, "type":"R","Pin":36,"lhOn":1,"status":0}'	//L4.8
        , '{"id":137, "type":"R","Pin":37,"lhOn":1,"status":0}'  //L6.8
        , '{"id":138, "type":"R","Pin":38,"lhOn":1,"status":0}'	//L5.2
        , '{"id":139, "type":"R","Pin":39,"lhOn":1,"status":0}'   //L5.8 - �����
        , '{"id":140, "type":"R","Pin":40,"lhOn":1,"status":0}'	//L5.1
        , '{"id":141, "type":"R","Pin":41,"lhOn":1,"status":0}'   //L5.7
        , '{"id":142, "type":"R","Pin":42,"lhOn":1,"status":0}'	//L5.3
        , '{"id":143, "type":"R","Pin":43,"lhOn":1,"status":0}'   //L5.6
        , '{"id":144, "type":"R","Pin":44,"lhOn":1,"status":0}'	//L5.4
        , '{"id":145, "type":"R","Pin":45,"lhOn":1,"status":0}'   //L5.5
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
        // �����
        '{"id":1,"originId":17,"originType":"B","event":1,"targetId":39,"targetAction":9}'
        , '{"id":2,"originId":17,"originType":"B","event":2,"targetId":32,"targetAction":9}'
        , '{"id":41,"originId":17,"originType":"B","event":4,"targetId":39,"targetAction":8}'
        , '{"id":42,"originId":17,"originType":"B","event":4,"targetId":32,"targetAction":8}'


        //��1            
        , '{"id":3,"originId":6,"originType":"B","event":1,"targetId":38,"targetAction":9}'
        , '{"id":4,"originId":6,"originType":"B","event":1,"targetId":40,"targetAction":9}'
        , '{"id":5,"originId":6,"originType":"B","event":2,"targetId":38,"targetAction":9}'
        , '{"id":6,"originId":6,"originType":"B","event":4,"targetId":38,"targetAction":8}'
        , '{"id":7,"originId":6,"originType":"B","event":4,"targetId":40,"targetAction":8}'

        //������� 1
        , '{"id":8,"originId":7,"originType":"B","event":1,"targetId":22,"targetAction":9}'
        , '{"id":9,"originId":7,"originType":"B","event":1,"targetId":24,"targetAction":9}'
        //������ ����        
        , '{"id":10,"originId":7,"originType":"B","event":2,"targetId":41,"targetAction":9}'
        , '{"id":11,"originId":7,"originType":"B","event":2,"targetId":43,"targetAction":9}'
        , '{"id":12,"originId":7,"originType":"B","event":4,"targetId":22,"targetAction":8}'
        // ��������� ��� 
        , '{"id":13,"originId":7,"originType":"B","event":4,"targetId":24,"targetAction":8}'
        , '{"id":14,"originId":7,"originType":"B","event":4,"targetId":22,"targetAction":8}'
        , '{"id":15,"originId":7,"originType":"B","event":4,"targetId":41,"targetAction":8}'
        , '{"id":16,"originId":7,"originType":"B","event":4,"targetId":43,"targetAction":8}'

        // �����, ����� �����         
        , '{"id":17,"originId":3,"originType":"B","event":1,"targetId":25,"targetAction":9}'
        , '{"id":18,"originId":3,"originType":"B","event":2,"targetId":42,"targetAction":9}'

        //����� ���������
        , '{"id":19,"originId":20,"originType":"B","event":1,"targetId":27,"targetAction":9}'
        , '{"id":20,"originId":20,"originType":"B","event":1,"targetId":44,"targetAction":9}'

        // ����� ��������� ���
        , '{"id":21,"originId":3,"originType":"B","event":4,"targetId":25,"targetAction":8}'
        , '{"id":22,"originId":3,"originType":"B","event":4,"targetId":42,"targetAction":8}'
        , '{"id":23,"originId":3,"originType":"B","event":4,"targetId":27,"targetAction":8}'
        , '{"id":24,"originId":3,"originType":"B","event":4,"targetId":44,"targetAction":8}'

        , '{"id":25,"originId":20,"originType":"B","event":4,"targetId":25,"targetAction":8}'
        , '{"id":26,"originId":20,"originType":"B","event":4,"targetId":42,"targetAction":8}'
        , '{"id":27,"originId":20,"originType":"B","event":4,"targetId":27,"targetAction":8}'
        , '{"id":28,"originId":20,"originType":"B","event":4,"targetId":44,"targetAction":8}'

        //��������
        , '{"id":29,"originId":19,"originType":"B","event":1,"targetId":28,"targetAction":9}'
        // �������
        , '{"id":30,"originId":11,"originType":"B","event":1,"targetId":41,"targetAction":9}'
        , '{"id":31,"originId":11,"originType":"B","event":1,"targetId":43,"targetAction":9}'
        // ������ ����
        , '{"id":32,"originId":11,"originType":"B","event":1,"targetId":22,"targetAction":9}'
        , '{"id":33,"originId":11,"originType":"B","event":1,"targetId":24,"targetAction":9}'
        // ��������� ��� 
        , '{"id":34,"originId":11,"originType":"B","event":4,"targetId":24,"targetAction":8}'
        , '{"id":35,"originId":11,"originType":"B","event":4,"targetId":22,"targetAction":8}'
        , '{"id":36,"originId":11,"originType":"B","event":4,"targetId":41,"targetAction":8}'
        , '{"id":37,"originId":11,"originType":"B","event":4,"targetId":43,"targetAction":8}'

        //������� �������
        , '{"id":38,"originId":15,"originType":"B","event":1,"targetId":33,"targetAction":9}'
        // ��������
        , '{"id":39,"originId":9,"originType":"B","event":1,"targetId":32,"targetAction":9}'

        // �������
        , '{"id":40,"originId":5,"originType":"B","event":1,"targetId":30,"targetAction":9}'
        , '{"id":42,"originId":5,"originType":"B","event":1,"targetId":34,"targetAction":9}'
        , '{"id":43,"originId":5,"originType":"B","event":2,"targetId":30,"targetAction":9}'

        , '{"id":44,"originId":5,"originType":"B","event":4,"targetId":34,"targetAction":8}'
        , '{"id":45,"originId":5,"originType":"B","event":4,"targetId":30,"targetAction":8}'


    ]
    , [
        // ��������             
        // ������ (�����)
        '{"id":101,"originId":112,"originType":"B","event":1,"targetId":131,"targetAction":9}'
        , '{"id":102,"originId":112,"originType":"B","event":1,"targetId":127,"targetAction":9}'
        // �����
        , '{"id":103,"originId":111,"originType":"B","event":1,"targetId":125,"targetAction":9}'
        , '{"id":104,"originId":111,"originType":"B","event":1,"targetId":123,"targetAction":9}'
        , '{"id":124,"originId":111,"originType":"B","event":1,"targetId":129,"targetAction":9}'
        // ��������� ���
        , '{"id":105,"originId":111,"originType":"B","event":4,"targetId":125,"targetAction":8}'
        , '{"id":106,"originId":111,"originType":"B","event":4,"targetId":131,"targetAction":8}'
        , '{"id":107,"originId":111,"originType":"B","event":4,"targetId":127,"targetAction":8}'
        , '{"id":108,"originId":111,"originType":"B","event":4,"targetId":123,"targetAction":8}'
        , '{"id":125,"originId":111,"originType":"B","event":4,"targetId":129,"targetAction":8}'

        , '{"id":109,"originId":112,"originType":"B","event":4,"targetId":125,"targetAction":8}'
        , '{"id":110,"originId":112,"originType":"B","event":4,"targetId":131,"targetAction":8}'
        , '{"id":111,"originId":112,"originType":"B","event":4,"targetId":127,"targetAction":8}'
        , '{"id":112,"originId":112,"originType":"B","event":4,"targetId":123,"targetAction":8}'
        , '{"id":126,"originId":112,"originType":"B","event":4,"targetId":129,"targetAction":8}'


        //��2            
        , '{"id":113,"originId":109,"originType":"B","event":1,"targetId":122,"targetAction":9}'
        , '{"id":114,"originId":109,"originType":"B","event":1,"targetId":133,"targetAction":9}'
        , '{"id":115,"originId":109,"originType":"B","event":2,"targetId":122,"targetAction":9}'
        , '{"id":116,"originId":109,"originType":"B","event":4,"targetId":122,"targetAction":8}'
        , '{"id":117,"originId":109,"originType":"B","event":4,"targetId":133,"targetAction":8}'

        //������ �������
        , '{"id":118,"originId":108,"originType":"B","event":1,"targetId":135,"targetAction":9}'

        //������� �������
        , '{"id":119,"originId":106,"originType":"B","event":1,"targetId":128,"targetAction":9}'
        , '{"id":120,"originId":106,"originType":"B","event":2,"targetId":130,"targetAction":9}'
        , '{"id":121,"originId":106,"originType":"B","event":4,"targetId":128,"targetAction":8}'
        , '{"id":122,"originId":106,"originType":"B","event":4,"targetId":130,"targetAction":8}'

        //����� �������
        , '{"id":123,"originId":107,"originType":"B","event":1,"targetId":132,"targetAction":9}'

    ]
];

var cacheConfigState = $(reqConf);
cacheConfigState.on(function (obj) {
    log("Request for config");
    var configTopic = obj.id;
    var boardId = parseInt(configTopic.substr(prefixTopic.length, 2));
    log("Board:" + boardId);
    configTopic = configTopic.replace(reqConfPart, 'ToCU.Configuration');
    log("Config topic:" + configTopic);
    var configLength = Configurations[boardId].length;
    setState(configTopic, '{"length":' + configLength + '}');

    for (var i = 0; i < configLength; i++) {
        //log(JSON.stringify(JSON.parse(Configurations[i])));

        setState(configTopic, Configurations[boardId][i]);
    }

    log("End Config Request");
});

var cacheActState = $(reqAct);
cacheActState.on(function (obj) {
    log("Request for actions");
    var actTopic = obj.id;
    var boardId = parseInt(actTopic.substr(prefixTopic.length, 2));
    log("Board:" + boardId);
    actTopic = actTopic.replace(reqActPart, 'ToCU.Action');
    //actTopic = actTopic.replace('.','/');
    //   log ("Action topic:" + actTopic);
    var actLength = Actions[boardId].length;

    setState(actTopic, '{"length":' + actLength + '}');

    for (var i = 0; i < actLength; i++) {
        setState(actTopic, Actions[boardId][i]);
    }

    log("End Action Request");
});
