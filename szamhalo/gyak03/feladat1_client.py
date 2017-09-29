import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

address = ('localhost', 10000)
msg = 'Message to send \o/'

try:
    sent = sock.sendto(msg.encode(), address)
    print('\nMessage to %s: %s' % (address, msg))

    print('\nWaiting for response...')

    data, adr = sock.recvfrom(32)
    print('\nMessage from %s: %s' % (adr, data))
finally:
    print('Closing socket')
    sock.close()