COLORS = [
    {color: "#bb0000", bg: "#660000"},
    {color: "#0000bb", bg: "#000066"},
    {color: "#bbbb00", bg: "#666600"},
    {color: "#00bb00", bg: "#006600"},
];

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
    constructor(vm, id, champ_id, name, pc) {
        this.name = name;
        this.id = id;
        this.champ_id = champ_id;
        this.pc = pc;
        this.vm = vm;
        this.delay = 0;
        this.color = COLORS[this.champ_id].color;
        this.bg = COLORS[this.champ_id].bg;
        this.elements = proc_div(id, name, this.color);
        this.registers = new Array(16);
        document.getElementById("sidebar").appendChild(this.elements.e);
        this.move(this.pc)
    }

    update({pc: pc, op: op, delay: delay, registers: registers}) {
        if (pc !== undefined && pc !== this.pc)
            this.move(pc);
        if (op !== undefined && op !== this.op)
            this.elements.op_div.textContent = `op: ${op}`;
        if (delay !== undefined && delay !== this.delay)
            this.update_delay(delay);
        if (registers !== undefined) {
            for (let i = 0; i < this.registers.length; ++i) {
                let v = registers[i].toUpperCase();
                if (v !== this.registers[i]) {
                    this.registers[i] = v;
                    this.elements.registers[i].textContent = "0x" + v;
                }
            }
        }
    }

    update_delay(delay) {
        this.delay = delay;
        this.elements.delay_div.textContent = `delay: ${delay}`;
    }

    move(pc) {
        let byte = this.vm.mem[this.pc];
        byte.e.style.borderColor = "#00000000";
        this.pc = pc;
        byte = this.vm.mem[pc];
        byte.e.style.borderColor = this.color;
    }

    destructor() {
        this.elements.e.remove();
    }
}
