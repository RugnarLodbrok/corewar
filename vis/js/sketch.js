WSS_PORT = 8765;
RUN_SPEED_STEPS = 8;
RUN_SPEED_DT = 128;

class Client {
    constructor() {
        this.state = "down";
        this.elements = {
            buttons_bar: document.getElementById("buttons_bar"),
            status_bar: document.getElementById("status_bar"),
            speed_bar: document.getElementById("speed_bar"),
            selectors: [
                document.getElementById("cor_select1"),
                document.getElementById("cor_select2"),
                document.getElementById("cor_select3"),
                document.getElementById("cor_select4")],
        };
        this.socket = null;
        this.vm = null;
        this.run_speed = 0;
        this.run_step_time = 0;
        this.run_timeout_id = 0;
        this.steps_are_running = 0; //vm did not yet fully responded to step request;
        this.init_buttons();
        this.stop_vm();
        this.elements.status_bar.textContent = "VM is down";
        this.init_selectors();
    }

    init_buttons() {
        let self = this;

        let run_function = function () {
            let now = Date.now();
            // if dt passed and no pending requests then send new step request
            if (now - self.run_step_time >= RUN_SPEED_DT && !self.steps_are_running) {
                self.run_step_time = Date.now();
                self.step(RUN_SPEED_STEPS * self.run_speed);
            }
            if (self.run_speed)
                self.run_timeout_id = setTimeout(run_function, RUN_SPEED_DT);
        };
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
                self.step(1);
            }),
            next_op: button("step op", function (e) {
                let steps = 1000;
                for (let proc of self.vm.procs)
                    if (steps > proc.delay)
                        steps = proc.delay;
                if (steps === 0)
                    steps = 1;
                self.step(steps)
            }),
            run_until_end: button("run 256 steps", function (e) {
                self.step(256);
            }),
            run: button("run", function (e) {
                if (self.run_speed === 0)
                    self.run_speed = 1;
                else
                    self.run_speed *= 2;
                let cps = Math.round(self.run_speed * RUN_SPEED_STEPS* 1000 / RUN_SPEED_DT);
                self.elements.speed_bar.innerText = `speed ${cps} c/s`;
                self.buttons.pause.disabled = false;
                if (self.run_speed === 1)
                    run_function();
            }),
            pause: button("pause", function (e) {
                clearTimeout(self.run_timeout_id);
                self.buttons.pause.disabled = true;
                self.run_speed = 0;
                self.elements.speed_bar.innerText = `paused`;
            })
        };
        this.elements.buttons_bar.appendChild(this.buttons.start);
        this.elements.buttons_bar.appendChild(this.buttons.stop);
        this.elements.buttons_bar.appendChild(this.buttons.next);
        this.elements.buttons_bar.appendChild(this.buttons.next_op);
        this.elements.buttons_bar.appendChild(this.buttons.run_until_end);
        this.elements.buttons_bar.appendChild(this.buttons.run);
        this.elements.buttons_bar.appendChild(this.buttons.pause);
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
                self.stop_vm();
            if (msg.type === "cycle")
                self.steps_are_running--;
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
        this.buttons.run.disabled = false;
        this.buttons.pause.disabled = true;
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
        this.buttons.run.disabled = true;
        this.buttons.pause.disabled = true;
        for (let sel of this.elements.selectors)
            sel.disabled = false;
        this.elements.status_bar.textContent = "VM is stopped";

        if (this.run_timeout_id)
            clearTimeout(this.run_timeout_id);
        this.run_timeout_id = 0;
        this.run_speed = 0;
        this.steps_are_running = 0;
    }

    step(n) {
        let msg;
        if (n === undefined)
            n = 1;
        if (n === 1)
            msg = `{"type": "step"}`;
        else
            msg = `{"type": "step", "steps": ${n}}`;
        this.vm.step(n);
        this.steps_are_running++;
        this.socket.send(msg);
    }
}

window.onload = function (e) {
    new Client();
};
