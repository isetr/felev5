import socket
import struct
import sys
import select

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setblocking(0)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,1)

server_address = ('127.0.0.1', (int)(sys.argv[4]))
server.bind(server_address)

client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

client_address = ('127.0.0.1', (int)(sys.argv[1]))

client.connect(client_address)

data = (b'S', sys.argv[2].encode('ascii'), (int)(sys.argv[4]))
packer = struct.Struct('s 64s I')
packed_data = packer.pack(*data)

try:
    client.sendall(packed_data)
    print("sent")
finally:
    client.close()

server.listen(5)

inputs = [server]
outputs = []

owndata = sys.argv[3]

while inputs:
    timeout = 1
        
    readable, writeable, exceptional = select.select(inputs, outputs, inputs, timeout)
        
    if not (readable or writeable or exceptional):
        continue

    for s in readable:
        if s is server:
            c, c_addr = s.accept()
            c.send(owndata.encode('ascii'))
            
            inputs.append(c)
            outputs.append(c)
        else:
            s.send(owndata.encode('ascii'))
            s.close()
            inputs.remove(s)
            outputs.remove(s)
server.close()