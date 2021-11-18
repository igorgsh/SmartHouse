/* -- do not edit following lines - START --
{
  "debug": false,
  "verbose": false
}
-- do not edit previous lines - END --*/

var hardReset = prefixBrd + "*.HardReset"

$(hardReset).on(function(obj){
    log("Hardreset request:" + obj.id);
    var topic = obj.id;
    var boardIdStr = topic.substr(prefixBrd.length, 2);
   //var boardId = parseInt(boardIdStr);
    resetBoard(boardIdStr);
    
});