import socket

hostname = socket.gethostname()
h1 = socket.gethostbyname('www.example.org')
h2, ali, add = socket.gethostbyname_ex(hostname)
h3, ali2, add2 = socket.gethostbyaddr('157.181.161.79')

print(hostname)
print(h1)
print(h2, ali, add)
print(h3, ali2, add2)