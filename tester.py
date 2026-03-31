import argparse
import threading
import time
import socket


def socketWorker(client_id, reqPerClient, delay, ip, port):
    reqRecieved = 0
    connectionFailed = 0
    buffersize = 1024
    timeout = 0
    # TODO play around wiht those values
    clientFd = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
    clientFd.setblocking(True)
    clientFd.settimeout(4)
    try:
        clientFd.connect((ip, port))
    except Exception:
        connectionFailed += 1
        with lock:
            stats.append({"clientID": client_id,
                          "reqRecieved": reqRecieved,
                          "connectionFailed": connectionFailed}
                         )

    for i in range(reqPerClient):
        message = f"Ping$ {client_id} {i}"
        try:

            clientFd.send(message.encode())
            data = clientFd.recv(buffersize)
            if data:
                reqRecieved += 1
        except Exception:
            timeout += 1
            continue

        if delay > 0:
            time.sleep(delay)
    clientFd.close()
    with lock:
        stats.append({"clientID": client_id,
                      "reqRecieved": reqRecieved,
                      "connectionFailed": connectionFailed,
                      "timeout": timeout}
                     )


parser = argparse.ArgumentParser()
parser.add_argument('-i', '--ip', help='Server ip', type=str, required=True)
parser.add_argument(
    '-p', '--port', help='Port at which server is running at ', type=int, required=True)
parser.add_argument(
    '-c', '--clients', help='Number of concurrent clients', type=int, default=1)
parser.add_argument('-r', '--requests',
                    help='Number of requests per clients', type=int, default=1)
parser.add_argument(
    '-d', '--delay', help='delay between requests made from a single client', type=int, default=0)


args = parser.parse_args()
ip = args.ip
port = args.port
clients = args.clients
reqPerClient = args.requests
delay = args.delay

stats = []
threads = []

lock = threading.Lock()

for client_id in range(clients):

    t = threading.Thread(target=socketWorker, args=(
        client_id, reqPerClient, delay, ip, port))
    t.start()
    threads.append(t)


for thread in threads:
    thread.join()

print(args)
print(stats)
