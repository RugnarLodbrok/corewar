function byte_div() {
    let d = document.createElement('td');
    d.className = 'byte';
    d.innerText = '00';
    return d;
}

class Byte {
    constructor(v) {
        this.v = v;
        this.bg = "#888888";
        this.e = byte_div();
    }

    draw() {
        this.e.innerText = this.v;
    }
}
