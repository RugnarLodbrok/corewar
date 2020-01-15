window.onload = function (e) {
    let socket;
    let vm;

    let stop = function (msg) {
        socket.close(1000, msg);
        vm.destructor()
    };

    let stop_button = button('stop vm', function (e) {
        stop("stop button");
    });
    stop_button.disabled = true;
    document.getElementById("buttons_bar").appendChild(div(button('start vm', function (e) {
            stop_button.disabled = false;
            if (socket)
                stop("re-connecting");
            vm = new VM();
            socket = new WebSocket('ws://localhost:8765/somesocket');

            socket.onopen = function () {
                socket.send('hello');
            };
            socket.onmessage = function (s) {
                let data = s.data;
                let msg = JSON.parse(data);
                if (!data)
                    return console.error("empty message");
                if (!msg)
                    return console.error(`can't parse data: "${data}"`);
                vm.update(msg);
                if (vm.stopped)
                    stop_button.disabled = true;
            };
        }),
        button('send hello', function (e) {
            console.log("send");
            socket.send("hello from browser!");
        }),
        stop_button,
        button("step cycle", function (e) {
            vm.step(1);
            socket.send(`{"type": "step"}`);
        }),
        button("step op", function (e) {
            let steps = 1000;
            for (let proc of vm.procs)
                if (steps > proc.delay)
                    steps = proc.delay;
            if (steps === 0)
                steps = 1;
            vm.step(steps);
            console.log(`step op: ${steps} steps`);
            socket.send(`{"type": "step", "steps": ${steps}}`);
        }),
        button("run until end", function (e) {
            socket.send(`{"type": "run_until_end"}`)
        })
    ));
};
