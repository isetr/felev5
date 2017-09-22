import socket
from urllib.parse import urlparse

for response in socket.getaddrinfo('www.python.org', 'http'):
	family, socktype, proto, canonname, sockaddr = response
	print(family, socktype, proto, canonname, sockaddr)
	
print()
	
for response in socket.getaddrinfo('www.inf.elte.hu', 'http'):
	print(response)