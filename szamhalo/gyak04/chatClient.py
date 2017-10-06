import socket
import sys
import time
import select
import msvcrt

username = sys.argv[1]

def prompt(nl):
	if nl:
		print ''
	sys.stdout.write('<'+username+'> ')
	sys.stdout.flush()

def readInput(timeout = 1):
	start_time = time.time()
	input = ''
	while True:
		if msvcrt.kbhit():
			chr = msvcrt.getche()
			if ord(chr) == 13:	#enter
				break
			elif ord(chr) >= 32: #space_char
				input += chr
		if len(input) == 0 and ((time.time() + start_time) > timeout):
			break
	
	if len(input) > 0:
		return input
	else:
		return ''
	
server_address = ('localhost',10000)

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

client.connect(server_address)
client.send(username)
client.settimeout(1.0)
prompt(False)

while True:
	socket_list = [client]
	
	readable, writable, error = select.select(socket_list, [],[],1)
	
	for s in readable:
		data = client.recv(4096)
		if not data:
			print "disconnect"
			client.close()
			sys.exit()
		else:
			if not data.startswith("["+username):
				print data
				sys.stdout.flush()
				prompt(False)
				
	try:
		msg = readInput()
		if msg != '':
			msg = msg.strip()
			client.send(msg)
			prompt(True)
	except socket.error, msg:
		print msg

client.close()

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	