import socket
import struct
import sys
import typing

print("create socket")
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

server_address = ('127.0.0.1', (int)(sys.argv[1]))

client.connect(server_address)

print("pack data")
data = (b'L', sys.argv[2].encode('ascii'), 0)
packer = struct.Struct('s 64s I')
packed_data = packer.pack(*data)

print("try block")
try:
    print("trying to send")
    client.sendall(packed_data)
    print("sent")
    ans, address = client.recvfrom(packer.size)
    sender, dataname, port = packer.unpack(ans)
    if (port == 0):
        print("no seeder with this data")
    else:
        c = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        c.connect(('127.0.0.1', port))
        answer, d = c.recvfrom(4096)
        print("answer: ", answer.strip())
finally:
    client.close()