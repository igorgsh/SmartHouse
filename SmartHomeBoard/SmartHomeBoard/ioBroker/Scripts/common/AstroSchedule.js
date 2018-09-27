schedule({astro: "sunrise", shift:10}, function () {
    log("10 min after Sunrise!");
    setState("mqtt.0.Equipment.Relays.R0034", 0, false);
});
  

schedule({astro: "sunset", shift: -20}, function () {
    log("20 minutes before sunset!");
    setState("mqtt.0.Equipment.Relays.R0034", 1, false);
});