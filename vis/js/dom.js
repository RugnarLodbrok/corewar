function button(text, callback) {
    let button = document.createElement('input');
    button.type = 'button';
    button.value = text;
    button.onclick = callback;
    return button;
}

function div()
{
    let div = document.createElement('div');
    for (let c of arguments) {
        if (typeof(c) === "string")
            div.innerText += c;
        else
            div.appendChild(c);
    }
    return div;
}
