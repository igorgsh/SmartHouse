/* -- do not edit following lines - START --
{
  "debug": false,
  "verbose": false
}
-- do not edit previous lines - END --*/

function checkWatchDog(devName) {
    var objName = prefixConfig + "." + devName + "." + watchDogPart;
    //log ("Watchdog checker for "+ objName);
    var obj = $(objName);
    if (obj!=null /*&& obj!='undefined'*/) {
        var st = obj.getState();
        if (st != null) {
            //log("Watchdog for " + devName + ": " + st.ts + ";diff=");
            var now = Date.now();
            var diff = now -st.ts;
            //log("Watchdog for " + devName + ": " + st.ts + ";diff=" + diff);
            return diff;
        }
    }
    return 0;
}
