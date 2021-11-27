const fs = require('fs');

function getSnapshot(caption){
    sendTo('onvif.0', 'saveFileSnapshot', {"id":"192_168_0_115_8899", "file":"/opt/cameras/snapshot.jpg"}, (data) => {
        log('image принят: ' + data);
    });
}

getSnapshot("sss");