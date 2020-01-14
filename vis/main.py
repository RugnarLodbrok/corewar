import asyncio
import datetime
import random
from subprocess import Popen, PIPE
import locale
import yaml
import json
import io

import websockets
from time import sleep

HOSTNAME = 'localhost'
PORT = 8765


def yaml_to_json(msg):
    buff = io.StringIO(msg)
    try:
        return json.dumps(yaml.load(buff, Loader=yaml.BaseLoader))
    except:
        print(f"can't encode:`\n{msg}`")
        raise


class VM:
    def __init__(self):
        self.p = Popen(
            ["cmake-build-debug/corewar_vm", "-v", "asm/hello.cor"],
            stdout=PIPE)

    def __iter__(self):
        encoding = locale.getpreferredencoding(False)
        msg = ""
        for line in self.p.stdout:
            if isinstance(line, bytes):
                line = line.decode(encoding=encoding)
            print(f"stdout line: `{line}`")
            if not line.strip():
                yield yaml_to_json(msg)
                msg = ""
            else:
                msg += line
        yield json.dumps({"type": "end"})


async def on_message(message):
    print(f"got message: {message}")


async def consumer_handler(ws: websockets.WebSocketServerProtocol, vm):
    print("*start consuming*")
    try:
        async for message in ws:
            await on_message(message)
    except websockets.ConnectionClosed as e:
        print(f"conn closed, code: {e.code}, reason: {e.reason}")
    else:
        print("*CONSUMING SOUNDS STOP*")


async def producer_handler(ws: websockets.WebSocketServerProtocol, vm):
    i = 0
    for message in vm:
        print(f"send `{message}`")
        await ws.send(message)
        i += 1


async def handler(ws: websockets.WebSocketServerProtocol, path):
    vm = VM()
    consumer_task = asyncio.ensure_future(consumer_handler(ws, vm))
    producer_task = asyncio.ensure_future(producer_handler(ws, vm))
    done, pending = await asyncio.wait(
        [consumer_task, producer_task],
        return_when=asyncio.FIRST_COMPLETED,
    )
    for task in pending:
        task.cancel()


if __name__ == '__main__':
    ws_address = f"wss://{HOSTNAME}:{PORT}/WebSocket"
    start_server = websockets.serve(handler, "localhost", 8765)

    asyncio.get_event_loop().run_until_complete(start_server)
    asyncio.get_event_loop().run_forever()
