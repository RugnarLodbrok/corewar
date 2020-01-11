import asyncio
import datetime
import random
import websockets
from time import sleep


async def handler_hello(websocket, path):
    name = await websocket.recv()
    print(f"< {name}")

    greeting = f"Hello {name}!"

    await websocket.send(greeting)
    print(f"> {greeting}")


async def time_server(websocket, path):
    print(f'time server start; path: {path}')
    i = 0
    while True:
        data = str(i)
        i += 1
        await websocket.send(data)
        sleep(1)


def on_close(*args, **kwargs):
    print("close")
    print(args)
    print(kwargs)


if __name__ == '__main__':
    start_server = websockets.serve(time_server, "localhost", 8765)

    asyncio.get_event_loop().run_until_complete(start_server)
    asyncio.get_event_loop().run_forever()
