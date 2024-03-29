
function switch_astro(mode) {
    setState(illumination, mode, false);

    //setState(geoPump,Boolean(mode), false);
    
}

schedule({astro: "sunset", shift: -20}, function () {
    log("20 minutes before sunset!");
    switch_astro(1);
});

schedule('{"time":{"exactTime":true,"start":"01:00"},"period":{"days":1}}', function() {
    log("Switch off the Christmass Light");
    switch_astro(0);
});



//schedule("0 6 * * *", function() {
schedule('{"time":{"exactTime":true,"start":"06:00"},"period":{"days":1}}', function() {
    if (getAstroDate("sunrise").getHours() > 6) {
        log("Switch on the Christmass Light at 06:00");
        switch_astro(1);
    } else {
        log("The Christmass Light is already off");
    }
});

schedule({astro: "sunrise", shift:10}, function () {
    log("10 min after Sunrise!");
    switch_astro(0);

});


  