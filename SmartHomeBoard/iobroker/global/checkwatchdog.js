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

function checkWatchDog2(devName) {
    var objName = prefixConfig + "." + devName + "." + watchDog2Part;
    log ("Watchdog2 checker for "+ objName);
    var obj = $(objName);
    if (obj!=null /*&& obj!='undefined'*/) {
        var st = obj.getState();
        if (st != null) {
            //log("Watchdog2 for " + devName + ": " + st.val);
            if (st.val < 3) {
                setState(objName, st.val + 1);
                return true;
            }
            setState(objName,0);   
            return false;
        }
    }
    return true;
}



