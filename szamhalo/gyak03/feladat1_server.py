import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

address = ('localhost', 10000)
print('Socket on %s : %s' % address)
sock.bind(address)

while True:
    print('\nWaiting for message...')
    data, adr = sock.recvfrom(32)

    print('\nMessage from %s : %s' % (adr, data))

    if data:
        sent = sock.sendto(data, address)
        print('\nMessage to %s: %s' % (adr, data))