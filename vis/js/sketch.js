class Client {
    constructor() {
        let self = this;
        this.state = "down";
        this.status_bar = document.getElementById("status_bar");
        this.cor_selector = document.getElementById("cor_select");
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
        this.status_bar.textContent = "VM is down";
        ajax_get('champions', function (r) {
                remove_children(self.cor_selector);
                for (let cor of r) {
                    let opt = document.createElement('option');
                    opt.text = cor;
                    self.cor_selector.appendChild(opt);
                }
            },
            function (status, msg) {
                self.status_bar.textContent = `${status}: ${msg}`
            });
    }

    start_vm() {
        let self = this;
        this.state = "up";
        if (this.vm) {
            this.vm.destructor();
            this.vm = null;
        }
        this.vm = new VM();
        this.socket = new WebSocket(`ws://localhost:8765/${this.cor_selector.value}`);
        this.socket.onopen = function () {
        };
        this.socket.onmessage = function (s) {
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
        this.socket.onclose = function (e) {
            console.log(`close ${e.code}: ${e.reason}`);
            self.stop_vm("connection was closed by server");
        };
        this.buttons.start.disabled = true;
        this.buttons.stop.disabled = false;
        this.buttons.next.disabled = false;
        this.buttons.next_op.disabled = false;
        this.buttons.run_until_end.disabled = false;
        this.cor_selector.disabled = true;
        this.status_bar.textContent = "VM is running";
    }

    stop_vm(msg) {
        if (this.socket)
            this.socket.close(1000, msg || "stop");
        this.state = "down";
        this.buttons.start.disabled = false;
        this.buttons.stop.disabled = true;
        this.buttons.next.disabled = true;
        this.buttons.next_op.disabled = true;
        this.buttons.run_until_end.disabled = true;
        this.cor_selector.disabled = false;
        this.status_bar.textContent = "VM is stopped";
    }
}

window.onload = function (e) {
    let client;

    client = new Client();
};
