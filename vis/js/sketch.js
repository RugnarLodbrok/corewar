function button(text, callback) {
    let button = document.createElement('input');
    button.type = 'button';
    button.value = text;
    button.onclick = callback;
    return button;
}

BYTE_W = 16;
BYTE_H = 10;
ASPECT = 1.3;

function chunks(s, size) {
    const numChunks = Math.ceil(s.length / size);
    const chunks = new Array(numChunks);

    for (let i = 0, o = 0; i < numChunks; ++i, o += size)
        chunks[i] = s.substr(o, size);
    return chunks;
}

class Proc {
    constructor(id, pc) {
        this.id = id;
        this.pc = pc;
        this.color = "#bb0000";
    }

    move(sketch, vm, pc) {
        this.pc = pc;
        sketch.push();
        sketch.fill(0, 0);
        sketch.strokeWeight(1);
        sketch.stroke(this.color);
        sketch.rect(...vm.byte_coors(pc), BYTE_W, BYTE_H);
        sketch.pop();
    }
}

class VM {
    constructor() {
        this.mem = null;
        this.rows = 0;
        this.cols = 0;
        this.w = 0;
        this.h = 0;
        this.procs = [];
        this.bg = "#888888";
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

    mem_init(sketch, data) {
        this.mem = data;
        [this.rows, this.cols] = this.choose_row_cols(this.mem.length);
        this.w = this.cols * BYTE_W;
        this.h = this.rows * BYTE_H;
        console.log(this.rows, this.cols);
        // sketch.push();
        // sketch.strokeWeight(0);
        // sketch.fill("#888888");
        // sketch.rect(0, 0, this.w, this.h);
        // sketch.fill(220);
        for (let i = 0; i < this.mem.length; ++i)
            this.draw_byte(sketch, i);
        // sketch.pop();
        console.log("done");
    }

    proc_move(sketch, id, pc) {
        let proc = this.procs[id];
        if (!proc)
            return console.error(`proc ${id} does not exist`);
        proc.move(sketch, this, pc);
    }

    new_proc(sketch, id, pc) {
        if (id !== this.procs.length)
            console.error("bad new proc id");
        else
            this.procs[id] = new Proc(id, pc);
        this.proc_move(sketch, id, pc);
    }

    byte_coors(i) {
        return [
            (BYTE_W * (i % this.cols)),
            (BYTE_H * Math.floor(i / this.cols))];
    }

    draw_byte(sketch, i) {
        let x;
        let y;

        [x, y] = this.byte_coors(i);
        sketch.push();
        sketch.strokeWeight(0);
        sketch.fill(this.bg);
        sketch.rect(x, y, BYTE_W, BYTE_H);
        sketch.fill(220);
        sketch.text(this.mem[i], x, y, BYTE_W, BYTE_H);
        sketch.pop();
    }

    write_mem(sketch, pc, data) {
        console.log("write mem", pc, data);
        for (let i = 0; i < data.length; ++i) {
            this.mem[pc + i] = data[i];
            this.draw_byte(sketch, pc + i);
        }
    }

    update(sketch, msg) {
        if (msg.type === "mem_init")
            this.mem_init(sketch, chunks(msg.data, 2));
        else if (msg.type === "new_proc")
            this.new_proc(sketch, msg.id - 0, msg.pc);
        else if (msg.type === "arr")
            console.log('STDOUT:', msg.char);
        else if (msg.type === "write_mem")
            this.write_mem(sketch, msg.pc - 0, chunks(msg.data, 2));
        else if (msg.type === "proc_move")
            this.proc_move(sketch, msg.id, msg.pc)
    }
}

new p5(function (sketch) {
    let socket;
    let main_div;
    let vm;

    sketch.setup = () => {
        vm = new VM();
        main_div = document.getElementById('sketch');
        main_div.appendChild(button('start vm', function (e) {
            if (socket)
                socket.close(1000, "re-connecting");
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
                vm.update(sketch, msg);
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

        sketch.createCanvas(1200, 800);
        sketch.background(0);
        sketch.stroke(220);
        sketch.fill(220);
        sketch.textSize(BYTE_H);
        sketch.textFont('Courier New');
        sketch.textAlign(sketch.CENTER, sketch.TOP);
        sketch.translate(10, 10);
    };
    sketch.draw = () => {
    };
}, document.getElementById("sketch"));
