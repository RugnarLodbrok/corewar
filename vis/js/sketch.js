class Client {
    constructor() {
        let self = this;
        this.state = "down";
        this.socket = null;
        this.vm = null;
        this.buttons = {
            start: button('start vm', function (e) {
                self.start_vm()
            }),
            stop: button('stop vm', function (e) {
                self.stop_vm("stop button");
            }),
            next: button("next", function (e) {
                self.vm.step(1);
                self.socket.send(`{"type": "step"}`);
            }),
            next_op: button("step op", function (e) {
                let steps = 1000;
                for (let proc of self.vm.procs)
                    if (steps > proc.delay)
                        steps = proc.delay;
                if (steps === 0)
                    steps = 1;
                self.vm.step(steps);
                console.log(`step op: ${steps} steps`);
                self.socket.send(`{"type": "step", "steps": ${steps}}`);
            }),
            run_until_end: button("run until end", function (e) {
                self.socket.send(`{"type": "run_until_end"}`)
            })
        };
        document.getElementById("buttons_bar").appendChild(this.buttons.start);
        document.getElementById("buttons_bar").appendChild(this.buttons.stop);
        document.getElementById("buttons_bar").appendChild(this.buttons.next);
        document.getElementById("buttons_bar").appendChild(this.buttons.next_op);
        document.getElementById("buttons_bar").appendChild(this.buttons.run_until_end);
        this.stop_vm();
    }

    start_vm() {
        let self = this;
        this.state = "up";
        this.vm = new VM();
        this.socket = new WebSocket('ws://localhost:8765/somesocket');
        this.socket.onopen = function () {
        };
        this.socket.onmessage = function (s) {
            console.log("msg");
            let data = s.data;
            let msg = JSON.parse(data);
            if (!data)
                return console.error("empty message");
            if (!msg)
                return console.error(`can't parse data: "${data}"`);
            self.vm.update(msg);
            if (self.vm.stopped)
                self.stop_vm()
        };
        this.buttons.start.disabled = true;
        this.buttons.stop.disabled = false;
        this.buttons.next.disabled = false;
        this.buttons.next_op.disabled = false;
        this.buttons.run_until_end.disabled = false;
    }

    stop_vm(msg) {
        if (this.socket)
            this.socket.close(1000, msg || "stop");
        if (this.vm)
            this.vm.destructor();
        this.state = "down";
        this.buttons.start.disabled = false;
        this.buttons.stop.disabled = true;
        this.buttons.next.disabled = true;
        this.buttons.next_op.disabled = true;
        this.buttons.run_until_end.disabled = true;
    }
}

window.onload = function (e) {
    let client;

    ajax_get('champions', function (r) {
        let selector = document.getElementById("cor_select");
        while (selector.firstChild)
            selector.removeChild(selector.firstChild);
        for (let cor of r) {
            let opt = document.createElement('option');
            opt.text = cor;
            selector.appendChild(opt);
        }
    });

    client = new Client();
};
