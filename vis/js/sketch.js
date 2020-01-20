WSS_PORT = 8765;

class Client {
    constructor() {
        let self = this;
        this.state = "down";
        this.elements = {
            buttons_bar: document.getElementById("buttons_bar"),
            status_bar: document.getElementById("status_bar"),
            selectors: [
                document.getElementById("cor_select1"),
                document.getElementById("cor_select2"),
                document.getElementById("cor_select3"),
                document.getElementById("cor_select4")],
        };
        this.socket = null;
        this.vm = null;
        this.buttons = {
            start: button('start vm', function (e) {
                let cors = [];
                for (let sel of self.elements.selectors)
                    if (sel.value)
                        cors.push(sel.value);
                self.start_vm(cors)
            }),
            stop: button('stop vm', function (e) {
                self.stop_vm("stop button");
            }),
            next: button("step", function (e) {
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
            run_until_end: button("run 256 steps", function (e) {
                let steps = 256;
                self.vm.step(steps);
                self.socket.send(`{"type": "step", "steps": ${steps}}`);
            })
        };
        this.elements.buttons_bar.appendChild(this.buttons.start);
        this.elements.buttons_bar.appendChild(this.buttons.stop);
        this.elements.buttons_bar.appendChild(this.buttons.next);
        this.elements.buttons_bar.appendChild(this.buttons.next_op);
        this.elements.buttons_bar.appendChild(this.buttons.run_until_end);
        this.stop_vm();
        this.elements.status_bar.textContent = "VM is down";
        this.init_selectors();
    }

    init_selectors() {
        let self = this;
        let on_change = function (e) {
            window.localStorage.setItem(e.target.id, e.target.value)
        };
        for (let selector of self.elements.selectors) {
            selector.onchange = on_change;
        }
        ajax_get('champions', function (r) {
                for (let selector of self.elements.selectors) {
                    remove_children(selector);
                    let saved_value = window.localStorage.getItem(selector.id);
                    selector.appendChild(document.createElement('option'));
                    for (let cor of r) {
                        let opt = document.createElement('option');
                        opt.text = cor;
                        selector.appendChild(opt);
                    }
                    selector.value = saved_value;
                }
            },
            function (status, msg) {
                self.elements.status_bar.textContent = `${status}: ${msg}`
            });
    }

    start_vm(cors) {
        let self = this;
        this.state = "up";
        if (this.vm) {
            this.vm.destructor();
            this.vm = null;
        }
        this.vm = new VM();
        this.socket = new WebSocket(`ws://localhost:${WSS_PORT}/${cors.join(';')}`); //wss://?
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
        for (let sel of this.elements.selectors)
            sel.disabled = true;
        this.elements.status_bar.textContent = "VM is running";
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
        for (let sel of this.elements.selectors)
            sel.disabled = false;
        this.elements.status_bar.textContent = "VM is stopped";
    }
}

window.onload = function (e) {
    new Client();
};
