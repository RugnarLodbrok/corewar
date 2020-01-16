import os
import asyncio
import asyncio.subprocess as aiosp
import locale
import yaml
import json
import io
from threading import Thread
import websockets

HOSTNAME = 'localhost'
WSS_PORT = 8765
HTTP_PORT = 8888
ASM_DIR = 'asm'

STDIN_F_NAME = 'stdin.txt'


def append_file(data):
    with open(STDIN_F_NAME, 'at') as f:
        f.write(data)


def yaml_to_json(msg):
    try:
        return json.dumps(yaml.safe_load(io.StringIO(msg)))
    except Exception:
        print(f"can't encode:`\n{msg}`")
        raise


class VM:
    def __init__(self):
        self.p = None

    async def stop(self):
        if self.p.returncode is None:
            self.p.terminate()
            print(f"proc killed")
        ret = await self.p.wait()
        print(f"proc finished; ret = {ret}")

    async def __aiter__(self):
        self.p = await aiosp.create_subprocess_exec(
            "cmake-build-debug/corewar_vm", "-v", "asm/write_mem.cor",
            stdin=aiosp.PIPE,
            stdout=aiosp.PIPE)
        print(f"pid: {self.p.pid}")
        encoding = locale.getpreferredencoding(False)
        msg = ""
        while True:
            line = await self.p.stdout.readline()
            if not line and self.p.returncode is not None:
                break
            if isinstance(line, bytes):
                line = line.decode(encoding=encoding)
            if not line.strip():
                if not msg:
                    await asyncio.sleep(.1)
                    continue
                yield yaml_to_json(msg)
                msg = ""
            else:
                msg += line
        yield json.dumps({"type": "end"})


async def on_message(vm, msg):
    print(f"got message: {msg}")
    if msg['type'] == "step":
        l = f"{msg.get('steps', 1)}\n"
    elif msg['type'] == "run_until_end":
        l = "999999\n"
    else:
        print(f"ERROR: unknwon command {msg['type']}")
        return
    append_file(l)
    vm.p.stdin.write(l.encode())
    await vm.p.stdin.drain()


async def consumer_handler(ws: websockets.WebSocketServerProtocol, vm):
    print("*start consuming*")
    try:
        async for message in ws:
            try:
                msg = json.loads(message)
            except:
                print("message:", message)
            else:
                await on_message(vm, msg)

    except websockets.ConnectionClosed as e:
        print(f"conn closed, code: {e.code}, reason: {e.reason}")
    else:
        print("*CONSUMING SOUNDS STOP*")
    finally:
        await vm.stop()


async def producer_handler(ws: websockets.WebSocketServerProtocol, vm):
    i = 0
    async for message in vm:
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
    for task in done:
        try:
            await task
        except websockets.ConnectionClosedOK:
            print("client disconnected")


def wss_server():
    ws_address = f"wss://{HOSTNAME}:{WSS_PORT}/WebSocket"
    loop = asyncio.new_event_loop()
    asyncio.get_child_watcher()._loop = loop
    start_server = websockets.serve(handler, "localhost", WSS_PORT, loop=loop)
    loop.run_until_complete(start_server)
    loop.run_forever()


if __name__ == '__main__':
    Thread(name='wss', target=wss_server, daemon=True).start()

    from flask import Flask, Response

    app = Flask(__name__)


    @app.route('/champions')
    def hello_world():
        response = Response(json.dumps([f for f in os.listdir(ASM_DIR)
                                        if os.path.isfile(os.path.join(ASM_DIR, f))
                                        and f.endswith('.cor')]))
        header = response.headers
        header['Access-Control-Allow-Origin'] = '*'
        return response


    @app.route('/')
    def index():
        with open(os.path.join('vis', 'index.html')) as f:
            return f.read()


    app.run(port=HTTP_PORT)
