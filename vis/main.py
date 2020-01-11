import asyncio
import datetime
import random
import websockets
from time import sleep

HOSTNAME = 'localhost'
PORT = 8765


class VM:
    def __init__(self):
        pass


async def on_message(message):
    print(f"got message: {message}")


async def consumer_handler(ws: websockets.WebSocketServerProtocol, path):
    print("*start consuming*")
    try:
        async for message in ws:
            await on_message(message)
    except websockets.ConnectionClosed as e:
        print(f"conn closed, code: {e.code}, reason: {e.reason}")
    else:
        print("*CONSUMING SOUNDS STOP*")


async def producer_handler(ws: websockets.WebSocketServerProtocol, path):
    i = 0
    while True:
        print(f"send i = {i}")
        await ws.send(str(i))
        await asyncio.sleep(1)
        i += 1


async def handler(ws: websockets.WebSocketServerProtocol, path):
    consumer_task = asyncio.ensure_future(consumer_handler(ws, path))
    producer_task = asyncio.ensure_future(producer_handler(ws, path))
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
