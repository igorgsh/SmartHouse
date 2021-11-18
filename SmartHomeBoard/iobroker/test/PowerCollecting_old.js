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

var powerVoltage='';
var powerPower='';
var powerEnergy='';

ElectricityGroup.forEach(function(item, i, ElectricityGroup) {
  if (powerVoltage != '') {
      powerVoltage += '|';
      powerPower += '|';
      powerEnergy += '|';
  }
  var voltage = '';
  var power = '';
  var energy = '';
  for (var j=0; j<3 ; j++) {
    if (voltage!='') {
        voltage += '|';
        power += '|';
        energy += '|';
    }
    voltage += item.sensor[j] + ".Voltage";
    power += item.sensor[j] + ".Power";
    energy += item.sensor[j] + ".Energy";
  }
  powerVoltage += voltage;
  powerPower += power;
  powerEnergy += energy;
});
powerVoltage=powerVoltage.replace('.',"\\.");
powerPower=powerPower.replace('.',"\\.");
RpowerEnergy=new RegExp(powerEnergy.replace(/\./g,"\\."));
log('tst=' + "aaa.bbb.ccc".replace('.', '\\.'));
//log("V=" + powerVoltage);
log("E=" + powerEnergy);
//log("P=" + powerPower);
/*
//var powerHeatersPower = prefixPowerMeter + '.P006*.Power';
//var powerHeatersEnergy = prefixPowerMeter + '.P006*.Energy';
//log (powerHeaters);
function SumEnergy(nm) {
    var suff;
    var outName;
    switch (typ) {
        case 0: 
        suff='.Energy';
        outName = totalHeatingConsumption;
        break; 
        case 1: 
            suff='.Power';
            outName = totalHeatingPower;
            break;
        default:
            return; 
    }
    var pm1 = prefixPowerMeter + '.P0064' + suff;
    var pm2 = prefixPowerMeter + '.P0065' + suff;
    var pm3 = prefixPowerMeter + '.P0066' + suff;
    var p = Number(getState(pm1).val) + Number(getState(pm2).val) + Number(getState(pm3).val); 
 //   log("p=" +p);
 //   log("output="+outName);
    setState(outName, p);
}
*/ 
function ProcessEnergy(nm) {
    log("nm=" + nm);
    var aggr = ElectricityGroup.filter(function(el){
        for (var i=0; i<3 ; i++) {
            log ("Point2");
            if (nm.startsWith(el.sensor[i])) {
                log("Point3");
                return true; 
            }
        }
        log("Point4");
        return false;
    });
  
    aggr.forEach(function(item, i, aggr) {
        log("Point1");
        var sens = item.sensor.filter(function(el){
            return (nm.startsWith(el));
        });

        var suff = nm.substring(sens[0].length + 1);
        log ("suffix=" + suff);
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
/*
$(powerPower).on(function(obj){
    ProcessEnergy(obj.id);
});
*/

$(RpowerEnergy).on(function(obj){
    ProcessEnergy(obj.id);
});

/*
$(powerVoltage).on(function(obj){
    ProcessEnergy(obj.id);
});
*/

//SumEnergy(1);
//SumEnergy(0);
//SumEnergy(3);
//ProcessEnergy('mqtt.0.Equipment.PowerMeter.P0064.Power');
//ProcessEnergy('mqtt.0.Equipment.PowerMeter.P0064.Energy');
//ProcessEnergy('mqtt.0.Equipment.PowerMeter.P0064.Voltage');









