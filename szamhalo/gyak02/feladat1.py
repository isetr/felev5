import socket

hostname = socket.gethostname()

print(hostname)

for host in ['homer', 'www', 'www.python.org', 'inf.elte.hu']:
	try:
		hn = socket.gethostbyname(host)
		h, ali, add = socket.gethostbyname_ex(host)
		print(hn, h, ali, add)
	except:
		print('Could not access ' + host)
	
for ip in ['157.181.161.79', '157.181.161.16']:
	try:
		h = socket.gethostbyaddr(ip)
		print(h)
	except:
		print('Could not access ' + ip)