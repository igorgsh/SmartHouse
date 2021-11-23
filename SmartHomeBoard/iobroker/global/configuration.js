/* -- do not edit following lines - START --
{
  "debug": false,
  "verbose": false
}
-- do not edit previous lines - END --*/

var SwitchOffPriority = 15;


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
// 3. Для SoftwareSerial
//  На платах Arduino Mega и Mega2560 некоторые выводы не поддерживают прерывания, 
//  возникающие при изменении уровня сигнала. В силу этого, на данных платах в качестве вывода RX 
//  могут использоваться только следующие выводы: 
// 10, 11, 12, 13, 14, 15, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64), A11 (65), A12 (66), A13 (67), A14 (68), A15 (69).
// 4. HardwarePort 
//  The Arduino Mega has three additional serial ports: 
//  Serial1 on pins 19 (RX) and 18 (TX), 
//  Serial2 on pins 17 (RX) and 16 (TX), 
//  Serial3 on pins 15 (RX) and 14 (TX).
// Types:
//	BUTTON			= 'B',
//	RELAY			= 'R',
//	ONE_WIRE_BUS	= '1',
//	ONE_WIRE_THERMO = 'T',
//	POWER_METER		= 'P',
//	VIRTUAL_BUTTON	= 'b',
//	CONTACTOR		= 'C',
//	SHIFT_OUT		='H',
//	SHIFT_IN		='h'




var Configurations = [
        [ //Board 00
            

            '{"id":2, "type":"B","Pin":2,"lhOn":0,"status":0}' // СУ 1(левый) 
            ,'{"id":3, "type":"B","Pin":3,"lhOn":0,"status":0}' // Кухня, возле мойки
            ,'{"id":5, "type":"B","Pin":5,"lhOn":0,"status":0}' // Крыльцо внешний
            ,'{"id":6, "type":"B","Pin":6,"lhOn":0,"status":0}' // СУ 1(правый)
            ,'{"id":7, "type":"B","Pin":7,"lhOn":0,"status":0}' // Коридор(левый) 
            ,'{"id":8, "type":"B","Pin":8,"lhOn":0,"status":0}' // Коридор(правый) - крыльцо
            ,'{"id":9, "type":"B","Pin":9,"lhOn":0,"status":0}' // Топочная вход
            ,'{"id":11, "type":"B","Pin":11,"lhOn":0,"status":0}' // Галерея
            ,'{"id":12, "type":"B","Pin":12,"lhOn":0,"status":0}' // 

            ,'{"id":14, "type":"B","Pin":14,"lhOn":0,"status":0}' // Сарай
            ,'{"id":15, "type":"B","Pin":15,"lhOn":0,"status":0}' // Красная спальня
            ,'{"id":16, "type":"B","Pin":16,"lhOn":0,"status":0}' // Кухня Возле холодильника
            ,'{"id":17, "type":"B","Pin":17,"lhOn":0,"status":0}' // Чулан

            ,'{"id":18, "type":"R","Pin":18,"lhOn":1,"status":0}' // реле в кладовой (боилер) 
            ,'{"id":56, "type":"R","Pin":56,"lhOn":1,"status":0}' // бойлер левый
            ,'{"id":55, "type":"R","Pin":55,"lhOn":1,"status":0}' // бойлер правый
            

            ,'{"id":19, "type":"B","Pin":19,"lhOn":0,"status":0}' // Стена Кладовка запад
            ,'{"id":20, "type":"B","Pin":20,"lhOn":0,"status":0}' // Стена Кладовка восток

			
            ,'{"id":22, "type":"R","Pin":22,"lhOn":1,"status":0,"priority":10}' 	//L1.1 Коридор 1. Восток
            ,'{"id":23, "type":"R","Pin":23,"lhOn":1,"status":0,"priority":10}'   //L2.1 Вытяжка
		    ,'{"id":24, "type":"R","Pin":24,"lhOn":1,"status":0,"priority":10}'	//L1.2 Коридор 1. Запад
            ,'{"id":25, "type":"R","Pin":25,"lhOn":1,"status":0,"priority":10}'   //L2.2 Кухня, между мойкой и плитой
            ,'{"id":26, "type":"R","Pin":26,"lhOn":1,"status":0,"priority":10}'	//L1.3 Кухня Восток
            ,'{"id":27, "type":"R","Pin":27,"lhOn":1,"status":0,"priority":10}'   //L2.3 Кухня Север
		    ,'{"id":28, "type":"R","Pin":28,"lhOn":1,"status":0,"priority":10}'	//L1.4 Кладовка
		    ,'{"id":29, "type":"R","Pin":29,"lhOn":1,"status":0,"priority":10}'   //L2.4 Сарай
            
            ,'{"id":30, "type":"R","Pin":30,"lhOn":1,"status":0,"priority":10}'	//L1.5 Тамбур 
		    ,'{"id":31, "type":"R","Pin":31,"lhOn":1,"status":0,"priority":10}'   //L2.5 ????
            ,'{"id":32, "type":"R","Pin":32,"lhOn":1,"status":0,"priority":10}'	//L1.6 Топочная над входом
		    ,'{"id":33, "type":"R","Pin":33,"lhOn":1,"status":0,"priority":10}'   //L2.6 Красная спальня 
            ,'{"id":34, "type":"R","Pin":34,"lhOn":1,"status":0,"priority":20}'	//L1.7 НЕ РАБОТАЕТ!!!!
	        ,'{"id":35, "type":"R","Pin":35,"lhOn":1,"status":0,"priority":10}'   //L2.7
            ,'{"id":36, "type":"R","Pin":36,"lhOn":1,"status":0,"priority":10}'	//L1.8 Крыльцо наружный
		    ,'{"id":37, "type":"R","Pin":37,"lhOn":1,"status":0,"priority":10}'   //L2.8
            
            ,'{"id":38, "type":"R","Pin":38,"lhOn":1,"status":0,"priority":10}'	//L3.5 СУ1 Душ
		    ,'{"id":39, "type":"R","Pin":39,"lhOn":1,"status":0,"priority":10}'   //L3.4 Чулан
            ,'{"id":40, "type":"R","Pin":40,"lhOn":1,"status":0,"priority":10}'	//L3.6 СУ1 Вход
		    ,'{"id":41, "type":"R","Pin":41,"lhOn":1,"status":0,"priority":10}'   //L3.3 Галерея Запад
            ,'{"id":42, "type":"R","Pin":42,"lhOn":1,"status":0,"priority":10}'	//L3.7 Кухня юг над мойкой
		    ,'{"id":43, "type":"R","Pin":43,"lhOn":1,"status":0,"priority":10}'   //L3.2 Галерея Восток
            ,'{"id":44, "type":"R","Pin":44,"lhOn":1,"status":0,"priority":10}'	//L3.8 Кухня юг-восток
		    ,'{"id":45, "type":"R","Pin":45,"lhOn":1,"status":0,"priority":10}'   //L3.1

/*
           //,'{"id":62, "type":"P","SerialRX":63,"SerialTX":54,"Factor":3}' // Power meter 
           ,'{"id":64, "type":"P","SerialRX":64,"SerialTX":57,"Factor":1}' // Power meter 3 
           ,'{"id":65, "type":"P","SerialRX":65,"SerialTX":58,"Factor":1}' // Power meter 3 
           ,'{"id":66, "type":"P","SerialRX":66,"SerialTX":59,"Factor":1}' // Power meter 3 
*/
        ]
        ,[ //Board 01
        
            '{"id":112, "type":"B","Pin":12,"lhOn":0,"status":0,"priority":10}' // Гостиная Север
            ,'{"id":111, "type":"B","Pin":11,"lhOn":0,"status":0,"priority":10}' // Гостиная Юг
            ,'{"id":109, "type":"B","Pin":9,"lhOn":0,"status":0,"priority":10}' // CУ2
            ,'{"id":108, "type":"B","Pin":8,"lhOn":0,"status":0,"priority":10}'// Желтая спальня
            ,'{"id":107, "type":"B","Pin":7,"lhOn":0,"status":0,"priority":10}'// Синяя спальня
            ,'{"id":106, "type":"B","Pin":6,"lhOn":0,"status":0,"priority":10}'// Зеленая спальня
            ,'{"id":105, "type":"B","Pin":5,"lhOn":0,"status":0,"priority":10}'// Террасса
            ,'{"id":148, "type":"B","Pin":48,"lhOn":0,"status":0,"priority":10}'// Калитка (сигнал с домофона)

    	    ,'{"id":103, "type":"R","Pin":3,"lhOn":0,"status":0,"priority":20}'   //2 Блок Реле 1 - калитка
    	    ,'{"id":149, "type":"R","Pin":49,"lhOn":0,"status":0,"priority":20}'   //2 Блок Реле 1 - калитка


            ,'{"id":102, "type":"R","Pin":2,"lhOn":1,"status":0,"priority":20}' 	//переноска для зарядки аккума
            ,'{"id":122, "type":"R","Pin":22,"lhOn":1,"status":0,"priority":10}' 	//L4.1 СУ2 над раковиной
            ,'{"id":123, "type":"R","Pin":23,"lhOn":1,"status":0,"priority":10}'   //L6.1 Гостиная запад
            ,'{"id":124, "type":"R","Pin":24,"lhOn":1,"status":0,"priority":10}'	//L4.2
            ,'{"id":125, "type":"R","Pin":25,"lhOn":1,"status":0,"priority":10}' //L6.2 Гостиная, восток
            ,'{"id":126, "type":"R","Pin":26,"lhOn":1,"status":0,"priority":10}'	//L4.3
            ,'{"id":127, "type":"R","Pin":27,"lhOn":1,"status":0,"priority":10}'  //L6.3 Камин Север
            ,'{"id":128, "type":"R","Pin":28,"lhOn":1,"status":0,"priority":10}'	//L4.4 Спальня зеленая возле балкона
            ,'{"id":129, "type":"R","Pin":29,"lhOn":1,"status":0,"priority":10}'  //L6.4 Гостиная Юг
            ,'{"id":130, "type":"R","Pin":30,"lhOn":1,"status":0,"priority":10}'	//L4.5 Спальня зеленая возле двери
		    ,'{"id":131, "type":"R","Pin":31,"lhOn":1,"status":0,"priority":10}'  //L6.5 Камин Восток
            ,'{"id":132, "type":"R","Pin":32,"lhOn":1,"status":0,"priority":10}'	//L4.6 Синяя спальня Восток
		    ,'{"id":133, "type":"R","Pin":33,"lhOn":1,"status":0,"priority":10}'  //L6.6 СУ2 над ванной
            ,'{"id":134, "type":"R","Pin":34,"lhOn":1,"status":0,"priority":10}'	//L4.7
		    ,'{"id":135, "type":"R","Pin":35,"lhOn":1,"status":0,"priority":10}'  //L6.7 желтая спальня над входом
            ,'{"id":136, "type":"R","Pin":36,"lhOn":1,"status":0,"priority":10}'	//L4.8
		    ,'{"id":137, "type":"R","Pin":37,"lhOn":1,"status":0,"priority":10}'  //L6.8 Синяя спальня Юг
            ,'{"id":138, "type":"R","Pin":38,"lhOn":1,"status":0,"priority":10}'	//L5.2
		    ,'{"id":139, "type":"R","Pin":39,"lhOn":1,"status":0,"priority":10}'   //L5.8 - Взрыв
            ,'{"id":140, "type":"R","Pin":40,"lhOn":1,"status":0,"priority":10}'	//L5.1
		    ,'{"id":141, "type":"R","Pin":41,"lhOn":1,"status":0,"priority":10}'   //L5.7
            ,'{"id":142, "type":"R","Pin":42,"lhOn":1,"status":0,"priority":10}'	//L5.3
		    ,'{"id":143, "type":"R","Pin":43,"lhOn":1,"status":0,"priority":10}'   //L5.6 Терраса 1
            ,'{"id":144, "type":"R","Pin":44,"lhOn":1,"status":0,"priority":10}'	//L5.4 Периметр ССВ
		    ,'{"id":145, "type":"R","Pin":45,"lhOn":1,"status":0,"priority":10}'   //L5.5

    	    ,'{"id":114, "type":"R","Pin":14,"lhOn":0,"status":0,"priority":20}'   //1 Блок Реле 4 клапан 1
	        ,'{"id":115, "type":"R","Pin":15,"lhOn":0,"status":0,"priority":20}'   //1 Блок Реле 5 клапан 2
	        ,'{"id":116, "type":"R","Pin":16,"lhOn":0,"status":0,"priority":20}'   //1 Блок Реле 3 клапан 4
	        ,'{"id":117, "type":"R","Pin":17,"lhOn":0,"status":1,"priority":20}'   //1 Блок Реле 2 насос 1 этажа 
	        ,'{"id":118, "type":"R","Pin":18,"lhOn":0,"status":0,"priority":20}'   //1 Блок Реле 8 
	        ,'{"id":119, "type":"R","Pin":19,"lhOn":0,"status":0,"priority":20}'   //1 Блок Реле 6 клапан 3
	        ,'{"id":120, "type":"R","Pin":20,"lhOn":0,"status":0,"priority":20}'   //1 Блок Реле 7 насос сада
	        ,'{"id":121, "type":"R","Pin":21,"lhOn":0,"status":1,"priority":20}'   //1 Блок Реле 1 насос 2 этаж


        ]
        ,[ //Board 02

           '{"id":64, "type":"P","SerialRX":64,"SerialTX":57,"Factor":1}' // Power meter 3 
           ,'{"id":65, "type":"P","SerialRX":65,"SerialTX":58,"Factor":1}' // Power meter 3 
           ,'{"id":66, "type":"P","SerialRX":66,"SerialTX":59,"Factor":1}' // Power meter 3 
            //'{"id":238, "type":"R","Pin":38,"lhOn":1,"status":0,"priority":10}'  
            //,'{"id":240, "type":"R","Pin":40,"lhOn":1,"status":0,"priority":10}'  
            //,'{"id":242, "type":"R","Pin":42,"lhOn":1,"status":0,"priority":10}'  
            //,'{"id":244, "type":"R","Pin":44,"lhOn":1,"status":0,"priority":10}'  
            //,'{"id":246, "type":"R","Pin":46,"lhOn":1,"status":0,"priority":10}'  
            //,'{"id":248, "type":"R","Pin":48,"lhOn":1,"status":0,"priority":10}'  
    //          '{"id":205, "type":"s","DPin":3,"latchPin":5,"clockPin":4,"PinsN":16}'  
      //      ,'{"id":281, "type":"C","Pin":0,"lhOn":1,"status":0,"priority":10, "parentId":205}'  
        //    ,'{"id":282, "type":"B","Pin":1,"lhOn":0,"status":0,"priority":10, "parentId":205}'  
       

        ]
        ,[ //Board 03
            '{"id":307, "type":"H","Pin":7,"latch":8,"clock":9,"steps":2,"status":0}' // Shift Register out 
		    //,'{"id":370, "type":"R","register":307, "Pin":4, "lhOn":1,"status":0}'   //
		    //,'{"id":371, "type":"R","register":307, "Pin":12, "lhOn":1,"status":0}'   //
                    
     /*
            '{"id":318, "type":"P","Pin":18,"lhOn":0,"status":0}' // Power meter 
        
       
            '{"id":312, "type":"B","Pin":12,"lhOn":0,"status":0}' // 
            ,'{"id":311, "type":"B","Pin":11,"lhOn":0,"status":0}' // 
            ,'{"id":309, "type":"B","Pin":9,"lhOn":0,"status":0}' // 

			 ,'{"id":345, "type":"R","Pin":45,"lhOn":1,"status":0}'   //
        */
        ]
    ];
	
/*
	ACT_OFF = 0, //Off for any device
	ACT_ON = 1, //On for any device
	ACT_SWITCH = 3, //Switch device
	ACT_LONG = 2, //Long (for button)
	ACT_SHORT_LONG = 5, //Button. Is pressed not released yet. And pressed time less than long
	ACT_EXTRA_LONG = 4, //Button. Extra long
	ACT_SENSOR_READY = 10 //Sensor ready

*/	

var Actions = [
        [ //Board 00
// Чулан
         '{"id":1,"originId":17,"originType":"B","event":1,"targetId":39,"targetAction":3,"targetType":"R"}' 
         ,'{"id":2,"originId":17,"originType":"B","event":2,"targetId":32,"targetAction":3,"targetType":"R"}'
         ,'{"id":3,"originId":17,"originType":"B","event":4,"targetId":39,"targetAction":0,"targetType":"R"}' 
         ,'{"id":4,"originId":17,"originType":"B","event":4,"targetId":32,"targetAction":0,"targetType":"R"}'



            
//СУ1            
         ,'{"id":10,"originId":6,"originType":"B","event":1,"targetId":38,"targetAction":3,"targetType":"R"}' 
         ,'{"id":11,"originId":6,"originType":"B","event":1,"targetId":40,"targetAction":3,"targetType":"R"}'
         ,'{"id":12,"originId":6,"originType":"B","event":2,"targetId":38,"targetAction":3,"targetType":"R"}'
         ,'{"id":13,"originId":6,"originType":"B","event":4,"targetId":38,"targetAction":0,"targetType":"R"}'
         ,'{"id":14,"originId":6,"originType":"B","event":4,"targetId":40,"targetAction":0,"targetType":"R"}'

//Коридор
    //Короткие нажатия
        //Возле выхода
         ,'{"id":20,"originId":7,"originType":"B","event":1,"targetId":22,"targetAction":3,"targetType":"R"}' 
         ,'{"id":21,"originId":7,"originType":"B","event":1,"targetId":24,"targetAction":3,"targetType":"R"}'
        //Санузел 
         ,'{"id":22,"originId":2,"originType":"B","event":1,"targetId":22,"targetAction":3,"targetType":"R"}' 
         ,'{"id":23,"originId":2,"originType":"B","event":1,"targetId":24,"targetAction":3,"targetType":"R"}'
        // Второй этаж
         ,'{"id":24,"originId":11,"originType":"B","event":1,"targetId":41,"targetAction":3,"targetType":"R"}'
         ,'{"id":25,"originId":11,"originType":"B","event":1,"targetId":43,"targetAction":3,"targetType":"R"}'

    //Длинное нажатие        
        //Возле выхода
         ,'{"id":30,"originId":7,"originType":"B","event":2,"targetId":41,"targetAction":3,"targetType":"R"}'
         ,'{"id":31,"originId":7,"originType":"B","event":2,"targetId":43,"targetAction":3,"targetType":"R"}'
        //Санузел 
         ,'{"id":32,"originId":2,"originType":"B","event":2,"targetId":41,"targetAction":3,"targetType":"R"}'
         ,'{"id":33,"originId":2,"originType":"B","event":2,"targetId":43,"targetAction":3,"targetType":"R"}'
        // Второй этаж
         ,'{"id":34,"originId":11,"originType":"B","event":2,"targetId":22,"targetAction":3,"targetType":"R"}' 
         ,'{"id":35,"originId":11,"originType":"B","event":2,"targetId":24,"targetAction":3,"targetType":"R"}'

    // Выключить все 
        //Возле выхода
         ,'{"id":37,"originId":7,"originType":"B","event":4,"targetId":24,"targetAction":0,"targetType":"R"}'
         ,'{"id":38,"originId":7,"originType":"B","event":4,"targetId":22,"targetAction":0,"targetType":"R"}'
         ,'{"id":39,"originId":7,"originType":"B","event":4,"targetId":41,"targetAction":0,"targetType":"R"}'
         ,'{"id":40,"originId":7,"originType":"B","event":4,"targetId":43,"targetAction":0,"targetType":"R"}'
        //Санузел 
         ,'{"id":41,"originId":2,"originType":"B","event":4,"targetId":24,"targetAction":0,"targetType":"R"}'
         ,'{"id":42,"originId":2,"originType":"B","event":4,"targetId":22,"targetAction":0,"targetType":"R"}'
         ,'{"id":43,"originId":2,"originType":"B","event":4,"targetId":41,"targetAction":0,"targetType":"R"}'
         ,'{"id":44,"originId":2,"originType":"B","event":4,"targetId":43,"targetAction":0,"targetType":"R"}'
        //Второй этаж
         ,'{"id":45,"originId":11,"originType":"B","event":4,"targetId":24,"targetAction":0,"targetType":"R"}'
         ,'{"id":46,"originId":11,"originType":"B","event":4,"targetId":22,"targetAction":0,"targetType":"R"}'
         ,'{"id":47,"originId":11,"originType":"B","event":4,"targetId":41,"targetAction":0,"targetType":"R"}'
         ,'{"id":48,"originId":11,"originType":"B","event":4,"targetId":43,"targetAction":0,"targetType":"R"}'
         
// Кухня, возле мойки         
         ,'{"id":50,"originId":3,"originType":"B","event":1,"targetId":25,"targetAction":3,"targetType":"R"}'
         ,'{"id":51,"originId":3,"originType":"B","event":2,"targetId":42,"targetAction":3,"targetType":"R"}'

// Кухня, вытяжка          
         ,'{"id":53,"originId":16,"originType":"B","event":1,"targetId":23,"targetAction":3,"targetType":"R"}'
         
//Кухня настенные
         ,'{"id":55,"originId":20,"originType":"B","event":2,"targetId":27,"targetAction":3,"targetType":"R"}'
         ,'{"id":56,"originId":20,"originType":"B","event":1,"targetId":44,"targetAction":3,"targetType":"R"}'
         ,'{"id":57,"originId":20,"originType":"B","event":1,"targetId":26,"targetAction":3,"targetType":"R"}'
         

// Кухня выключить все
         ,'{"id":70,"originId":3,"originType":"B","event":4,"targetId":25,"targetAction":0,"targetType":"R"}'
         ,'{"id":71,"originId":3,"originType":"B","event":4,"targetId":42,"targetAction":0,"targetType":"R"}'
         ,'{"id":72,"originId":3,"originType":"B","event":4,"targetId":27,"targetAction":0,"targetType":"R"}'
         ,'{"id":73,"originId":3,"originType":"B","event":4,"targetId":44,"targetAction":0,"targetType":"R"}'
         ,'{"id":74,"originId":3,"originType":"B","event":4,"targetId":26,"targetAction":0,"targetType":"R"}'

         ,'{"id":75,"originId":20,"originType":"B","event":4,"targetId":25,"targetAction":0,"targetType":"R"}'
         ,'{"id":76,"originId":20,"originType":"B","event":4,"targetId":42,"targetAction":0,"targetType":"R"}'
         ,'{"id":77,"originId":20,"originType":"B","event":4,"targetId":27,"targetAction":0,"targetType":"R"}'
         ,'{"id":78,"originId":20,"originType":"B","event":4,"targetId":44,"targetAction":0,"targetType":"R"}'
         ,'{"id":79,"originId":20,"originType":"B","event":4,"targetId":26,"targetAction":0,"targetType":"R"}'
         ,'{"id":81,"originId":16,"originType":"B","event":4,"targetId":23,"targetAction":0,"targetType":"R"}'

//Кладовка
         ,'{"id":80,"originId":19,"originType":"B","event":1,"targetId":28,"targetAction":3,"targetType":"R"}'

//Красная спальня
         ,'{"id":82,"originId":15,"originType":"B","event":1,"targetId":33,"targetAction":3,"targetType":"R"}'
// Топочная
         ,'{"id":84,"originId":9,"originType":"B","event":1,"targetId":32,"targetAction":3,"targetType":"R"}'

// Сарай
         ,'{"id":86,"originId":14,"originType":"B","event":1,"targetId":29,"targetAction":3,"targetType":"R"}'
         ,'{"id":87,"originId":14,"originType":"B","event":4,"targetId":29,"targetAction":0,"targetType":"R"}'
  


// Крыльцо
         ,'{"id":90,"originId":5,"originType":"B","event":1,"targetId":36,"targetAction":3,"targetType":"R"}'

         ,'{"id":91,"originId":8,"originType":"B","event":1,"targetId":30,"targetAction":3,"targetType":"R"}'
         
         ,'{"id":92,"originId":5,"originType":"B","event":2,"targetId":144,"targetAction":3,"targetType":"R"}'
         ,'{"id":93,"originId":8,"originType":"B","event":2,"targetId":36,"targetAction":3,"targetType":"R"}'

//Switch off everything

         ,'{"id":94,"originId":5,"originType":"B","event":4,"targetId":0001,"targetAction":4,"targetType":"b"}'

//         ,'{"id":94,"originId":5,"originType":"B","event":4,"targetId":34,"targetAction":0,"targetType":"R"}'
//         ,'{"id":95,"originId":5,"originType":"B","event":4,"targetId":144,"targetAction":0,"targetType":"R"}'

         ,'{"id":96,"originId":8,"originType":"B","event":4,"targetId":36,"targetAction":0,"targetType":"R"}'
         ,'{"id":97,"originId":8,"originType":"B","event":4,"targetId":30,"targetAction":0,"targetType":"R"}'


        ]
        ,[ //Board 01
// Гостиная             
    // правый (камин)
         '{"id":101,"originId":112,"originType":"B","event":1,"targetId":131,"targetAction":3,"targetType":"R"}'
         ,'{"id":102,"originId":112,"originType":"B","event":2,"targetId":127,"targetAction":3,"targetType":"R"}'
    // Выключить все
         ,'{"id":103,"originId":112,"originType":"B","event":4,"targetId":125,"targetAction":0,"targetType":"R"}'
         ,'{"id":104,"originId":112,"originType":"B","event":4,"targetId":131,"targetAction":0,"targetType":"R"}'
         ,'{"id":105,"originId":112,"originType":"B","event":4,"targetId":127,"targetAction":0,"targetType":"R"}'
         ,'{"id":106,"originId":112,"originType":"B","event":4,"targetId":123,"targetAction":0,"targetType":"R"}'
         ,'{"id":107,"originId":112,"originType":"B","event":4,"targetId":129,"targetAction":0,"targetType":"R"}'

//Калитка
         ,'{"id":108,"originId":148,"originType":"B","event":1,"targetId":103,"targetAction":1,"targetType":"R"}'
         ,'{"id":109,"originId":148,"originType":"B","event":2,"targetId":103,"targetAction":1,"targetType":"R"}'

         
//Гостиная левый
         ,'{"id":110,"originId":111,"originType":"B","event":1,"targetId":125,"targetAction":3,"targetType":"R"}'
         ,'{"id":111,"originId":111,"originType":"B","event":2,"targetId":123,"targetAction":3,"targetType":"R"}'
         ,'{"id":112,"originId":111,"originType":"B","event":2,"targetId":129,"targetAction":3,"targetType":"R"}'
    // Выключить все
         ,'{"id":113,"originId":111,"originType":"B","event":4,"targetId":125,"targetAction":0,"targetType":"R"}'
         ,'{"id":114,"originId":111,"originType":"B","event":4,"targetId":131,"targetAction":0,"targetType":"R"}'
         ,'{"id":115,"originId":111,"originType":"B","event":4,"targetId":127,"targetAction":0,"targetType":"R"}'
         ,'{"id":116,"originId":111,"originType":"B","event":4,"targetId":123,"targetAction":0,"targetType":"R"}'
         ,'{"id":117,"originId":111,"originType":"B","event":4,"targetId":129,"targetAction":0,"targetType":"R"}'

//СУ2            
         ,'{"id":120,"originId":109,"originType":"B","event":1,"targetId":122,"targetAction":3,"targetType":"R"}' 
         ,'{"id":121,"originId":109,"originType":"B","event":1,"targetId":133,"targetAction":3,"targetType":"R"}'
         ,'{"id":122,"originId":109,"originType":"B","event":2,"targetId":122,"targetAction":3,"targetType":"R"}'
         ,'{"id":123,"originId":109,"originType":"B","event":4,"targetId":122,"targetAction":0,"targetType":"R"}'
         ,'{"id":124,"originId":109,"originType":"B","event":4,"targetId":133,"targetAction":0,"targetType":"R"}'

//Желтая спальня
         ,'{"id":130,"originId":108,"originType":"B","event":1,"targetId":135,"targetAction":3,"targetType":"R"}'

//Зеленая спальня
         ,'{"id":140,"originId":106,"originType":"B","event":1,"targetId":128,"targetAction":3,"targetType":"R"}'
         ,'{"id":141,"originId":106,"originType":"B","event":2,"targetId":130,"targetAction":3,"targetType":"R"}'
         ,'{"id":142,"originId":106,"originType":"B","event":4,"targetId":128,"targetAction":0,"targetType":"R"}'
         ,'{"id":143,"originId":106,"originType":"B","event":4,"targetId":130,"targetAction":0,"targetType":"R"}'

//Синяя спальня
         ,'{"id":150,"originId":107,"originType":"B","event":1,"targetId":132,"targetAction":3,"targetType":"R"}'
         ,'{"id":151,"originId":107,"originType":"B","event":1,"targetId":137,"targetAction":3,"targetType":"R"}'
         ,'{"id":152,"originId":107,"originType":"B","event":2,"targetId":132,"targetAction":3,"targetType":"R"}'

         ,'{"id":153,"originId":107,"originType":"B","event":4,"targetId":132,"targetAction":0,"targetType":"R"}'
         ,'{"id":154,"originId":107,"originType":"B","event":4,"targetId":137,"targetAction":0,"targetType":"R"}'

//Терраса 
         ,'{"id":160,"originId":105,"originType":"B","event":1,"targetId":143,"targetAction":3,"targetType":"R"}'
         ]
         ,[ //Board 02
         '{"id":201,"originId":201,"originType":"B","event":4,"targetId":0211,"targetAction":1,"targetType":"b"}'
        ]
         ,[ //Board 03
        // '{"id":301,"originId":311,"originType":"B","event":1,"targetId":245,"targetAction":3,"targetType":"R"}'
        // ,'{"id":302,"originId":211,"originType":"B","event":2,"targetId":127,"targetAction":0,"targetType":"R"}'
        ]

        
    ]; 
/*
var ElectricityGroup [
                        {
                            "aggregation": "mqtt.0.Others.Heating",
                            ["sensors":"P0064","sensors":"P0066","sensors":"P0065" ]
                        }
] 
*/




