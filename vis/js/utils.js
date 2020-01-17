HOST = "localhost";
HTTP_PORT = 8888;

function chunks(s, size) {
    const numChunks = Math.ceil(s.length / size);
    const chunks = new Array(numChunks);

    for (let i = 0, o = 0; i < numChunks; ++i, o += size)
        chunks[i] = s.substr(o, size);
    return chunks;
}


function ajax_get(path, callback, err_callback) {
    let req = new XMLHttpRequest();
    req.onreadystatechange = function () {
        if (req.readyState === 4) {
            if (req.status === 200)
                callback(JSON.parse(req.responseText));
            else if (err_callback !== undefined)
                err_callback(req.status, req.responseText);
            else
                console.error(req.status, req.responseText);
        }
    };
    if (err_callback !== undefined)
        req.onerror = function (e) {
            err_callback(0, "can't connect to the server");
        };
    req.open("GET", `http://${HOST}:${HTTP_PORT}/${path}`, true);
    req.send();
}

function remove_children(e) {
    while (e.firstChild)
        e.removeChild(e.firstChild);
}
