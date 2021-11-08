
var ElectricityGroup = [
                        {"aggregation": "mqtt.0.Others.Heating", "sensor": ["P0064","P0065","P0066"]},
                        {"aggregation": "mqtt.0.Others.Electricity", "sensor": ["P0064","P0065","P0066"]},
];  
/*
log("g0=" + ElectricityGroup[0].aggregation);
log("g.sensor0=" + ElectricityGroup[0].sensor[1]);

log("g1=" + ElectricityGroup[1].aggregation);
*/
var strVoltage = prefixPowerMeter+"." + ElectricityGroup[0].sensor[0] + ".Voltage" + "|" +
prefixPowerMeter+"." + ElectricityGroup[0].sensor[1] + ".Voltage";
log(strVoltage);

$(strVoltage).on(function(obj){
    
    log("Power ON:" + obj.name);
});




