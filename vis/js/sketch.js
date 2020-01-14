BYTE_W = 22;
BYTE_H = 20;
ASPECT = .7;

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

    move(vm, pc) {
        let byte = vm.mem[this.pc];
        byte.e.style.borderColor = "#00000000";
        this.pc = pc;
        byte = vm.mem[pc];
        byte.e.style.borderColor = this.color;
    }

    destructor() {
        this.elements.e.remove();
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
        this.e = document.getElementById('memory_view');
        this.e.innerHTML = '';
        this.e.className = "memory";
    }

    show_cycle() {
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
        [this.cols, this.rows] = this.choose_row_cols(data.length);
        console.log(this.rows, this.cols, this.rows * this.cols, data.length);
        let table = document.createElement('table');
        table.className = "byte_table";
        this.e.appendChild(table);
        for (let i = 0; i < this.rows; ++i) {
            let row = document.createElement('tr');
            table.appendChild(row);
            for (let j = 0; j < this.cols; ++j) {
                let n = i * this.cols + j;
                let byte = new Byte(data[n]);
                this.mem.push(byte);
                row.appendChild(byte.e);
            }
        }
    }

    proc_move(id, pc) {
        let proc = this.procs[id];
        if (!proc)
            return console.error(`proc ${id} does not exist`);
        proc.move(this, pc);
    }

    new_proc(id, name, pc) {
        if (id !== this.procs.length)
            console.error(`bad new proc id: ${id}, procs.len: ${this.procs.length}`);
        else
            this.procs[id] = new Proc(id, name, pc);
        this.procs[id].move(this, pc);
    }

    write_mem(pc, data) {
        for (let i = 0; i < data.length; ++i) {
            this.mem[pc + i].v = data[i];
            this.mem[pc + i].draw();
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

window.onload = function (e) {
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
    document.getElementById("buttons_bar").appendChild(div(button('start vm', function (e) {
            vm = new VM();
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
};
