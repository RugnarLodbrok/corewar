function button(text, callback) {
    let button = document.createElement('input');
    button.type = 'button';
    button.value = text;
    button.onclick = callback;
    return button;
}

let socket;

let main_div = document.getElementById('sketch');
main_div.appendChild(button('start vm', function (e) {
    if (socket)
        socket.close(1000, "re-connecting");
    socket = new WebSocket('ws://localhost:8765/somesocket');

    socket.onopen = function () {
        socket.send('hello');
    };
    socket.onmessage = function (s) {
        // console.log(s);
        console.log(s.data)
    };
}));

main_div.appendChild(button('send hello', function (e) {
    console.log("send");
    socket.send("hello from browser!");
}));

main_div.appendChild(button('stop vm', function (e) {
    console.log('close!');
    socket.close(1000, "stopping");
}));
