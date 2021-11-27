
sendTo('telegram', {user: '@igorgsh', text: 'Test message3'}, function (res) {
    console.log('Sent to ' + res + ' users');
});

sendTo('telegram', 'https://www.google.com/images/branding/googlelogo/1x/googlelogo_color_272x92dp.png')