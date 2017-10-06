import socket
import select
import Queue
import sys

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setblocking(0)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,1)

server_address = ('localhost',10000)
server.bind(server_address)

server.listen(5)

inputs = [server]

outputs = []

message_queues = Queue.Queue()
username = {}

while inputs:
	timeout = 1
	readable, writeable, exceptional = select.select(inputs, outputs, inputs, timeout)
	
	if not (readable or writeable or exceptional):
		continue
	
	for s in readable:
		if s is server:
			client, client_address = s.accept()
			client.setblocking(1)
			name = client.recv(20)
			print "new connection from ", client_address,"with username", name
			username[client] = name.strip()
			message_queues.put("["+username[client]+"] is LOGIN")
			
			inputs.append(client)
			outputs.append(client)
		elif not sys.stdin.isatty():
		# elif s == sys.stdin
			print "Close the system"
			inputs.remove(server)
			for c in inputs:
				inputs.remove(c)
				c.close()
			server.close()
			inputs = []
		else:
			data = s.recv(1024)
			data = data.strip()
			if data:
				print "received data: ",data,"from",s.getpeername()
				message_queues.put("["+username[s]+"]: "+data)
			else:
				print "client close"
				if s in outputs:
					outputs.remove(s)
				inputs.remove(s)
				s.close()
				message_queues.put("["+username[s]+"] is LOGOUT")
				if s in writeable:
					writeable.remove(s)

	while not message_queues.empty():
		try:
			next_msg = message_queues.get_nowait()
			print next_msg
		except Queue.empty:
			break
		else:
			for s in writeable:
				print "sending:",next_msg,"to",s.getpeername()
				s.send(next_msg)
#	for s in exceptional
		
		
		
		
		
		
		
		
		
		
		
		
		