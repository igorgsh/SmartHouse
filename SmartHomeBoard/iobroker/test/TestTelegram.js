
sendTo('telegram', {user: 'igorgsh', text: 'Test message6'}, function (res) {
    console.log('Sent to ' + res + ' users');
});

//sendTo('telegram', 'Hello Igor')