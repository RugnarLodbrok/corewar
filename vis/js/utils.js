HOST = "localhost";
PORT = 8888;

function chunks(s, size) {
    const numChunks = Math.ceil(s.length / size);
    const chunks = new Array(numChunks);

    for (let i = 0, o = 0; i < numChunks; ++i, o += size)
        chunks[i] = s.substr(o, size);
    return chunks;
}


function ajax_get(path, callback) {
    let req = new XMLHttpRequest();
    req.onreadystatechange = function() {
        if (req.readyState === 4) {
            if (req.status === 200)
                callback(JSON.parse(req.responseText));
            else
                console.error(req.status, req.responseText);
        }
    };
    req.open("GET", `http://${HOST}:${PORT}/${path}`, true);
    req.send();
}
