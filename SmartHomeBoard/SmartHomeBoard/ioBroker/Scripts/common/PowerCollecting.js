var ElectricityGroup = [
                        {"aggregation": "mqtt.0.Others.Heating", "sensor": 
                                                                [prefixPowerMeter + ".P0064",
                                                                 prefixPowerMeter + ".P0065",
                                                                 prefixPowerMeter + ".P0066"]},
                        {"aggregation": "mqtt.0.Others.Electricity", "sensor": 
                                                                [prefixPowerMeter + ".P0064",
                                                                 prefixPowerMeter + ".P0065",
                                                                 prefixPowerMeter + ".P0066"]}
];  


ElectricityGroup.forEach(function(item, i, ElectricityGroup) {
  var str = '';
  for (var j=0; j<3 ; j++) {
    str = item.sensor[j] + ".Voltage";
    $(str).on(function(obj){
        ProcessEnergy(obj.id);
    });
    str = item.sensor[j] + ".Power";
    $(str).on(function(obj){
        ProcessEnergy(obj.id);
    });
    str = item.sensor[j] + ".Energy";
    $(str).on(function(obj){
        ProcessEnergy(obj.id);
    });
  }
});
function ProcessEnergy(nm) {
//    log("nm=" + nm);
    var aggr = ElectricityGroup.filter(function(el){
        for (var i=0; i<3 ; i++) {
//            log ("Point2");
            if (nm.startsWith(el.sensor[i])) {
//                log("Point3");
                return true; 
            }
        }
//        log("Point4");
        return false;
    });
  
    aggr.forEach(function(item, i, aggr) {
//        log("Point1");
        var sens = item.sensor.filter(function(el){
            return (nm.startsWith(el));
        });

        var suff = nm.substring(sens[0].length + 1);
  //      log ("suffix=" + suff);
        var v1, v2, v3;
        switch (suff) {
            case 'Voltage':
                var v1, v2, v3;
                v1 = Number(getState(item.sensor[0]+".Voltage").val);
                v2 = Number(getState(item.sensor[1]+".Voltage").val);
                v3 = Number(getState(item.sensor[2]+".Voltage").val);

                var mn = Math.min(v1, v2, v3);
                var mx = Math.max(v1, v2, v3);
                setState(item.aggregation+".Max", Number(mx));
                setState(item.aggregation+".Min", Number(mn));
                setState(item.aggregation+".Delta", Number(mx-mn));
                setState(item.aggregation+".LineA", Number(v1));
                setState(item.aggregation+".LineB", Number(v2));
                setState(item.aggregation+".LineC", Number(v3));
                break;
            case 'Power':
            case 'Energy':
                var p = Number(getState(item.sensor[0] + '.' + suff).val) + 
                    Number(getState(item.sensor[1] + '.' + suff).val) + 
                    Number(getState(item.sensor[2] + '.' + suff).val); 
                setState(item.aggregation + '.' + suff, p);
                break;

        } 

        
    });

} 




