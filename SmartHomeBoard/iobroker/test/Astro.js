var sunriseEnd = getAstroDate("sunriseEnd");
log("Sunrise ends today at " + sunriseEnd.toLocaleTimeString());

sunriseEnd = getAstroDate("sunrise");
log("Sunrise today at " + sunriseEnd.toLocaleTimeString());


sunriseEnd = getAstroDate("sunset");
log("Sunset today at " + sunriseEnd.toLocaleTimeString());


sunriseEnd = getAstroDate("sunsetStart");
log("Sunset starts today at " + sunriseEnd.toLocaleTimeString());

var X = getAstroDate("sunrise").getHours();
log("X=" + X);

if (X>=6) { 
    log ("POINT1");
} else {
    log ("POINT2");
    
}