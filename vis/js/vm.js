BYTE_W = 22;
BYTE_H = 20;
ASPECT = .7;

class VM {
    constructor() {
        this.stopped = false;
        this.cycle = 0;
        this.mem = null;
        this.rows = 0;
        this.cols = 0;
        this.w = 0;
        this.h = 0;
        this.procs = [];
        this.e = document.getElementById('memory_view');
        this.e.className = "memory";
        this.cycles_bar = document.getElementById('cycles_bar');
    }

    step(n) {
        if (n === undefined)
            n = 1;
        this.cycle += n;
        for (let proc of this.procs)
            proc.update_delay(proc.delay - n)
    }

    destructor() {
        for (let proc of this.procs)
            proc.destructor();
        remove_children(this.e);
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
        // console.log("best", best);
        best = 64;
        return [len / best, best]
    }

    mem_init(size) {
        this.mem = [];
        [this.cols, this.rows] = this.choose_row_cols(size);
        console.log(this.rows, this.cols, this.rows * this.cols, size);
        let table = document.createElement('table');
        table.className = "byte_table";
        this.e.appendChild(table);
        for (let i = 0; i < this.rows; ++i) {
            let row = document.createElement('tr');
            table.appendChild(row);
            for (let j = 0; j < this.cols; ++j) {
                let n = i * this.cols + j;
                let byte = new Byte("00");
                this.mem.push(byte);
                row.appendChild(byte.e);
            }
        }
    }

    proc_update(id, kwargs) {
        let proc;
        if (id < this.procs.length)
            proc = this.procs[id];
        else if (id === this.procs.length) {
            console.log("new proc!");
            proc = new Proc(this, id, kwargs.champ_id, kwargs.name, kwargs.pc);
            this.procs.push(proc);
        } else
            return console.error(`proc ${id} does not exist`);
        proc.update(kwargs);
    }

    new_proc(id, name, pc) {
        if (id !== this.procs.length)
            console.error(`bad new proc id: ${id}, procs.len: ${this.procs.length}`);
        else
            this.procs[id] = new Proc(this, id, name, pc);
        this.procs[id].move(pc);
    }

    write_mem(pc, data, proc_id) {
        let bg = null;
        let byte;

        if (proc_id !== undefined) {
            let proc = this.procs[proc_id];
            bg = proc.colors.bg_bright;
            for (let i = 0; i < proc.last_mem_write_len; ++i) {
                byte = this.mem[(proc.last_mem_write + i) % this.mem.length];
                if (byte.bg === proc.colors.bg_bright)
                    byte.draw({bg: proc.colors.bg})
            }
            proc.last_mem_write = pc;
            proc.last_mem_write_len = data.length;
        }
        for (let i = 0; i < data.length; ++i) {
            byte = this.mem[(pc + i) % this.mem.length];
            byte.v = data[i];
            byte.draw({bg: bg});
        }
    }

    update(msg) {
        if (msg.type === "cycle") {
            if (this.cycle !== msg.value) {
                console.error(`cycles count divergence: ${this.cycle} != ${msg.value}`);
                this.cycle = msg.value;
            }
        } else if (msg.type === "proc_update")
            this.proc_update(msg.id, msg);
        else if (msg.type === "write_mem")
            this.write_mem(msg.pc, chunks(msg.data, 2), msg.proc_id);
        else if (msg.type === "arr")
            console.log('STDOUT:', msg.char);
        else if (msg.type === "mem_init")
            this.mem_init(msg.size);
        else if (msg.type === "end") {
            this.stopped = true;
            console.log('command: end')
        } else
            console.error(`unknown command ${msg.type}`);
        this.cycles_bar.innerText = this.cycle + "";
        return msg.type;
    }
}
