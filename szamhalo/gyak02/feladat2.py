import socket
from urllib.parse import urlparse

for url in ['http://www.example.com', 'https://www.example.com', 'ftp://example.com']:
	parsed_url = urlparse(url)
	port = socket.getservbyname(parsed_url.scheme)
	print(port)
	
print()
	
for port in [80, 443, 21, 70, 25, 143, 993, 110, 995]:
	url = urlparse(socket.getservbyport(port), 'localhost', '/')
	if url.path == 'https':
		print(url, port)
		
print()

for port in range(1,100):
	try:
		url = urlparse(socket.getservbyport(port), 'localhost', '/')
		print(port, url.path)
	except:
		()
		
print()
		
for prot in ['icmp', 'tcp', 'udp']:
	print(prot, socket.getprotobyname(prot))
