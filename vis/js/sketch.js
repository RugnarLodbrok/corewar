BYTE_W = 16;
BYTE_H = 10;
ASPECT = 1.3;

function proc_div(id, name, color) {
    let delay_div = div(`delay: ${0}`);
    let op_div = div(`op: ${null}`);
    let registers_div = div();
    registers_div.className = "registers";
    let d = div(div(`name: ${name}`), div(`id: ${id}`), op_div, delay_div, registers_div);
    d.className = "proc";
    let registers = [];
    for (let i = 0; i < 16; ++i) {
        let reg_label = div(`r${i + 1}`);
        reg_label.className = "register_label";
        let reg_value = div(`0x00000000`);
        reg_value.className = "register_val";
        let reg = div(reg_label, reg_value);
        reg.className = "register";
        registers_div.appendChild(reg);
        registers.push(reg_value)
    }
    return {
        e: d,
        delay_div: delay_div,
        op_div: op_div,
        registers: registers
    }
}

class Proc {
    constructor(id, name, pc) {
        this.name = name;
        this.id = id;
        this.pc = pc;
        this.color = "#bb0000";
        this.elements = proc_div(id, name, this.color);
        document.getElementById("sidebar").appendChild(this.elements.e)
    }

    draw(vm) {
        let sketch = vm.sketch;
        sketch.push();
        sketch.fill(0, 0);
        sketch.strokeWeight(1);
        sketch.stroke(this.color);
        sketch.rect(...vm.byte_coors(this.pc), BYTE_W, BYTE_H);
        sketch.pop();
    }

    destructor() {
        this.elements.e.remove();
    }
}

class Byte {
    constructor(v) {
        this.v = v;
        this.bg = "#888888";
    }

    draw(vm, pc) {
        let sketch = vm.sketch;
        let x, y;

        [x, y] = vm.byte_coors(pc);
        sketch.push();
        sketch.strokeWeight(0);
        sketch.fill(this.bg);
        sketch.rect(x, y, BYTE_W, BYTE_H);
        sketch.fill(220);
        sketch.text(this.v, x, y, BYTE_W, BYTE_H);
        sketch.pop();
    }
}

class VM {
    constructor(sketch) {
        this.sketch = sketch;
        this.stopped = false;
        this.cycle = 0;
        this.mem = null;
        this.rows = 0;
        this.cols = 0;
        this.w = 0;
        this.h = 0;
        this.procs = [];
    }

    destructor(sketch) {
        for (let proc of this.procs)
            proc.destructor();
    }

    choose_row_cols(len) {
        let best = 1;
        for (let i = 1; i < len / 2; ++i) {
            if (BYTE_W * len / (i + 1) < BYTE_H * (i + 1) * ASPECT) {
                break;
            }
            if (len % i)
                continue;
            best = i;
        }
        return [len / best, best]
    }

    mem_init(data) {
        this.mem = [];
        for (let datum of data)
            this.mem.push(new Byte(datum));
        [this.rows, this.cols] = this.choose_row_cols(this.mem.length);
        this.w = this.cols * BYTE_W;
        this.h = this.rows * BYTE_H;
        this.sketch.createCanvas(this.w, this.h);
        console.log(this.rows, this.cols);
        for (let i = 0; i < this.mem.length; ++i)
            this.mem[i].draw(this, i);
        console.log("done");
    }

    show_cycle() {
        let s = this.sketch;
        let rect = [this.w + 10, 100, 100, 50];

        s.push();
        s.strokeWeight(0);
        s.textSize(20);
        s.fill(0);
        s.rect(...rect);
        s.fill(220);
        s.text(`cycle: [${this.cycle}]`, ...rect);
        s.pop();
    }

    proc_move(id, pc) {
        let proc = this.procs[id];
        if (!proc)
            return console.error(`proc ${id} does not exist`);
        proc.pc = pc;
        proc.draw(this);
    }

    new_proc(id, name, pc) {
        if (id !== this.procs.length)
            console.error(`bad new proc id: ${id}, procs.len: ${this.procs.length}`);
        else
            this.procs[id] = new Proc(id, name, pc);
        this.procs[id].draw(this);
    }

    byte_coors(i) {
        return [
            (BYTE_W * (i % this.cols)),
            (BYTE_H * Math.floor(i / this.cols))];
    }

    draw_byte(i) {
        let x;
        let y;
        let sketch = this.sketch;

        [x, y] = this.byte_coors(i);
        this.mem[i].draw(this.sketch, x, y)
    }

    write_mem(pc, data) {
        for (let i = 0; i < data.length; ++i) {
            this.mem[pc + i].v = data[i];
            this.mem[pc + i].draw(this, pc + i);
        }
    }

    update(msg) {
        if (msg.type === "mem_init")
            this.mem_init(chunks(msg.data, 2));
        else if (msg.type === "new_proc")
            this.new_proc(msg.id - 0, msg.name, msg.pc);
        else if (msg.type === "arr")
            console.log('STDOUT:', msg.char);
        else if (msg.type === "write_mem")
            this.write_mem(msg.pc - 0, chunks(msg.data, 2));
        else if (msg.type === "proc_move")
            this.proc_move(msg.id, msg.pc);
        else if (msg.type === "end") {
            this.stopped = true;
            console.log('command: end')
        } else
            console.error(`unknown command ${msg.type}`);
    }
}

new p5(function (sketch) {
    let socket;
    let vm;

    let stop = function (msg) {
        console.log('close!');
        socket.close(1000, msg);
    };

    let stop_button = button('stop vm', function (e) {
        stop("stop button");
        vm.destructor()
    });
    stop_button.disabled = true;
    sketch.setup = () => {
        document.getElementById("buttons_bar").appendChild(div(button('start vm', function (e) {
                vm = new VM(sketch);
                stop_button.disabled = false;
                if (socket)
                    stop("re-connecting");
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
            button("step", function (e) {
                console.log("send step");
                vm.cycle += 1;
                vm.show_cycle();
                socket.send(`{"type": "step"}`);
            }),
            button("run until end", function (e) {
                socket.send(`{"type": "run_until_end"}`)
            })
        ));
        vm_div = div();
        vm_div.id = 'vm_div';

        // sketch.createCanvas(1200, 800);
        sketch.background(0);
        sketch.stroke(220);
        sketch.fill(220);
        sketch.textSize(BYTE_H);
        sketch.textFont('Courier New');
        // sketch.textAlign(sketch.CENTER, sketch.TOP);
        sketch.translate(10, 10);
    };
    sketch.draw = () => {
    };
}, document.getElementById("memory_view"));
