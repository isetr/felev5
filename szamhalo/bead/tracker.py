import socket
import struct
import select
import sys

server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
port = (int)(sys.argv[1])
server_adress = ("127.0.0.1", port)
server.bind(server_adress)

print(port)

datas = {}

unpacker = struct.Struct("s 64s I")

while True:
    print("waiting for request")
    data, client = server.recvfrom(unpacker.size)

    print("data recieved")
    sender, dataname, port = unpacker.unpack(data)

    if (sender == b'S'):
        print("sender is seeder")
        datas[dataname] = port
    if (sender == b'L'):
        print("sender is leecher")
        try:
            port = datas[dataname]

            answer = (b"L", dataname, port)
            packed = unpacker.pack(*answer)

            server.sendto(packed, client)
        except:
            answer = (b"L", dataname, 0)
            packed = unpacker.pack(*answer)

            server.sendto(packed, client)
server.close()