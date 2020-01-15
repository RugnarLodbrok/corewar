import asyncio
import asyncio.subprocess as aiosp
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
    try:
        return json.dumps(yaml.safe_load(io.StringIO(msg)))
    except Exception:
        print(f"can't encode:`\n{msg}`")
        raise


class VM:
    def __init__(self):
        self._p_coro = aiosp.create_subprocess_exec(
            "cmake-build-debug/corewar_vm", "-v", "asm/write_mem.cor",
            stdin=aiosp.PIPE,
            stdout=aiosp.PIPE)
        self.p = None

    async def stop(self):
        if not self.p:
            self.p = await self._p_coro
        if self.p.returncode is None:
            self.p.terminate()
        ret = await self.p.wait()
        print(f"proc killed; ret = {ret}")

    async def __aiter__(self):
        if not self.p:
            self.p = await self._p_coro
        encoding = locale.getpreferredencoding(False)
        msg = ""
        while True:
            line = await self.p.stdout.readline()
            if not line and self.p.returncode is not None:
                break
            if isinstance(line, bytes):
                line = line.decode(encoding=encoding)
            if not line.strip():
                yield yaml_to_json(msg)
                msg = ""
            else:
                msg += line
        yield json.dumps({"type": "end"})


async def on_message(vm, msg):
    print(f"got message: {msg}")
    if msg['type'] == "step":
        vm.p.stdin.write(b"1\n")
    elif msg['type'] == "run_until_end":
        vm.p.stdin.write(b"999999\n")
    else:
        print(f"ERROR: unknwon command {msg['type']}")


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


if __name__ == '__main__':
    ws_address = f"wss://{HOSTNAME}:{PORT}/WebSocket"
    start_server = websockets.serve(handler, "localhost", 8765)

    asyncio.get_event_loop().run_until_complete(start_server)
    asyncio.get_event_loop().run_forever()
