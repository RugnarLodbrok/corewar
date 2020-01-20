function byte_div() {
    let d = document.createElement('td');
    d.className = 'byte';
    d.innerText = '00';
    return d;
}

class Byte {
    constructor(v) {
        this.v = v;
        this.bg = "#000000";
        this.e = byte_div();
    }

    draw(kwargs) {
        if (kwargs.bg) {
            this.bg = kwargs.bg;
            this.e.style.backgroundColor = kwargs.bg;
        }
        this.e.innerText = this.v;
    }
}
