$(gateLock).on(function(obj){
    if (obj.state.val == 1) { // Switch on
        setStateDelayed(gateLock, 0,  2000); 
        log("Delayed Lock the gate!");
    } else {
        log("The gate has been locked!");
    }
});

$(intercomOpen).on(function(obj){
   log("Intercom open:" + obj.state.val);
   if (obj.state.val != 0) { //open the gate
        setState(gateLock,1);
   } 
});  