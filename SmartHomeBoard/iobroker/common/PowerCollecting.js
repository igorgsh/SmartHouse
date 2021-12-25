var ElectricityGroup = [
                        {"aggregation": "mqtt.0.Others.Heating", "sensor": 
                                                                [prefixPowerMeter + ".P0264",
                                                                 prefixPowerMeter + ".P0265",
                                                                 prefixPowerMeter + ".P0266"]},
                        {"aggregation": "mqtt.0.Others.Electricity", "sensor": 
                                                                [prefixPowerMeter + ".P0267",
                                                                 prefixPowerMeter + ".P0263",
                                                                 prefixPowerMeter + ".P0262"]}
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
    str = item.sensor[j] + ".Current";
    $(str).on(function(obj){
        ProcessEnergy(obj.id);
    });
    str = item.sensor[j] + ".PowerFactor";
    $(str).on(function(obj){
        ProcessEnergy(obj.id);
    });
    str = item.sensor[j] + ".Frequency";
    $(str).on(function(obj){
        ProcessEnergy(obj.id);
    });
  }
});
function ProcessEnergy(nm) {
    var aggr = ElectricityGroup.filter(function(el){
        for (var i=0; i<3 ; i++) {
            if (nm.startsWith(el.sensor[i])) {
                return true; 
            }
        }
        return false;
    });
  
    aggr.forEach(function(item, i, aggr) {
        var sens = item.sensor.filter(function(el){
            return (nm.startsWith(el));
        });

        var suff = nm.substring(sens[0].length + 1);
        var v1, v2, v3;

        v1 = Number(getState(item.sensor[0]+"." + suff).val);
        v2 = Number(getState(item.sensor[1]+"." + suff).val);
        v3 = Number(getState(item.sensor[2]+"." + suff).val);
        var mn = Math.min(v1, v2, v3);
        var mx = Math.max(v1, v2, v3);
        setState(item.aggregation+"."+suff+".Max", Number(mx));
        setState(item.aggregation+"."+suff+".Min", Number(mn));
        setState(item.aggregation+"."+suff+".Delta", Number(mx-mn));
        setState(item.aggregation+"."+suff+".LineA", Number(v1));
        setState(item.aggregation+"."+suff+".LineB", Number(v2));
        setState(item.aggregation+"."+suff+".LineC", Number(v3));

        switch(suff) {
            case "Voltage":
            case "Current":
            case "Frequency":
            case "PowerFactor":
                setState(item.aggregation+"."+suff+".Avg", (v1+v2+v3)/3 );
                break;
            case "Power":
            case "Energy":
                setState(item.aggregation+"."+suff+".Sum", (v1+v2+v3) );
                break;

        }

    });
} 


