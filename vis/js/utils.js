function chunks(s, size) {
    const numChunks = Math.ceil(s.length / size);
    const chunks = new Array(numChunks);

    for (let i = 0, o = 0; i < numChunks; ++i, o += size)
        chunks[i] = s.substr(o, size);
    return chunks;
}
