'''

	Written by Azorfus
	Github: @azorfus

'''

import socket
import threading
import time
from datetime import datetime
import os

SERVER_HOST = "192.168.1.11" # str(input("Enter Host IP: "))
SERVER_PORT = 8080

fsport = 0

s = socket.socket()

# event = threading.Event()
# event.set()

try:
	s.connect((SERVER_HOST, SERVER_PORT))
	fsport = int(s.recv(256).decode().split("!@$:")[0])
	print("File transport port: ", fsport)

except ConnectionRefusedError:
	print("Server is not available. Exiting...")
	exit(1)

print(f"[*] Connected to {SERVER_HOST}:{SERVER_PORT}.")

name = input("Enter your name: ")
date_now = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
to_send = f"{name} has connected."
conn_init = f"{date_now}!@$#:server!@$#:{to_send}!@$:"
s.send(conn_init.ljust(1024, "#").encode())

def listen_for_messages():
	while True:
		#if event.is_set():
		msg_R = s.recv(1024).decode('utf-8', errors='ignore')
		msg = msg_R.split("!@$:")[0]
		if msg == "server!abort!code!abort":
			s.close()
			break
		if msg != "file!download!incoming":
			print("\n" + msg)
		#else:
		#	print("Paused Thread")
		#	event.wait()
		

def file_transfer(file_name):
	print("\n[*] Establishing tunnel to server, this may take a moment...")

	# We wait for server to open its FTP server and send confirmation that its open so that
	# we can connect.
	confirmation = s.recv(1024).decode()
	confirmation = confirmation.split("!@$:")[0]

	# If the confirmation isn't as expected, we wait on a loop listening to the server for
	# the confirmation.
	while confirmation != "file!tunnel!open":
		confirmation = s.send("uncon!@$:".ljust(1024, "#").encode())

		confirmation = s.recv(1024).decode()
		confirmation = confirmation.split("!@$:")[0]

	# If confirmation is received we send a pong message back to the server to confirm 
	# the received confirmation.
	print("[!] Received confirmation from server... Connecting...")
	s.send("PONG!@$:".ljust(1024, "#").encode())

	fs = socket.socket()
	try:
		fs.connect((SERVER_HOST, fsport))
		print("\n[*] Uploading file via tunnel to server... This will take a moment...")
		file = open(file_name, "rb")
		file_size = os.path.getsize(file_name)
		rem = int(file_size % 1024)
		quo = int((file_size - rem)/1024)
		packet_info = str(quo) + ":@!$" + str(rem) + ":@!$" + file_name + ":@!$" + name + ":@!$"
		send_packet = packet_info.ljust(256, "#")
		fs.send(send_packet.encode())

		# packet transmission
		fs.sendall(file.read())

		file.close()

	except Exception as e:
		print(f"[!] Could not establish file transfer tunnel.\nError: {e}")
		fs.close()
		return 0

	try:
		confirmation = fs.recv(1024).decode()
		confirmation = confirmation.split("!@$:")[0]
	
		# If the confirmation isn't as expected, we wait on a loop listening to the server for
		# the confirmation.
		while confirmation != "file!upload!success":
			print("[*] Waiting for upload success confirmation....")
			confirmation = fs.send("uncon!@$:".ljust(1024, "#").encode())
			confirmation = fs.recv(1024).decode()

		print(f"[*] File transfer tunnel successfully closing... File transfer was successful.")
	
	except Exception as e:
		print(f"[!] File transfer failed, Data was corrupted or was not sent through.\nConfirmation not received from server.\nError: {e}")

	fs.close()
	return 0

def file_download(file_name):
	print("\n[*] Establishing tunnel to server, this may take a moment...")


	# We wait for server to open its FTP server and send confirmation that its open so that
	# we can connect.
	confirmation = s.recv(1024).decode()
	confirmation = confirmation.split("!@$:")[0]

	# If the confirmation isn't as expected, we wait on a loop listening to the server for
	# the confirmation.
	while confirmation != "file!tunnel!open":
		confirmation = s.send("uncon!@$:".ljust(1024, "#").encode())

		confirmation = s.recv(1024).decode()
		confirmation = confirmation.split("!@$:")[0]

	# If confirmation is received we send a pong message back to the server to confirm 
	# the received confirmation.
	print("[!] Received confirmation from server... Connecting...")
	s.send("PONG!@$:".ljust(1024, "#").encode())
	
	fs = socket.socket()
	try:
		fs.connect((SERVER_HOST, fsport))
		print(f"[*] Established tunnel to server on port: {fsport}")
		packet_info_R = fs.recv(256)
		packet_info = packet_info_R.decode('utf-8', errors='ignore')
		packet_struct = packet_info.split(":@!")
	
		rem = int(packet_struct[1])
		quo = int(packet_struct[0])
		req_file = packet_struct[2]
	
		with open(req_file, "wb") as f:
	
			data = bytearray()
			count = 0
			packet = 0
			while count <= quo:
				if count == quo:
					packet = fs.recv(rem)
				else:
					packet = fs.recv(1024)
		
				data += packet
				count += 1
		
			f.write(data)
			f.close()
		print("[*] Closing file transfer tunnel...")
		
		done = "file!download!success!@$:"
		done = done.ljust(1024, "#")
		fs.send(done.encode())
		fs.close()

		print("File downloaded...", f"file name: {packet_struct[2]}, size: {quo * 4096 + rem} bytes.\n")

	except Exception as e:
		print(f"[!] Could not establish file transfer tunnel.\nError: {e}")
		fs.close()
		return 0

	return 0

lisn = threading.Thread(target = listen_for_messages)
lisn.daemon = True
lisn.start()

while True:
	date_now = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
	to_send = input()

	try:
		if to_send.lower() == 'quit':
			to_send = "client!exit!code!@$:"
			s.send(to_send.ljust(1024, "#").encode())
			break
	
		elif to_send.strip() == "!upload":
			fileName = input("Enter file name: ")
			to_send = f"file!transfer!code!@$:{fsport}!@$:"
			s.send(to_send.ljust(1024, "#").encode())
			file_transfer(fileName)
	
		elif to_send.strip() == "!download":
			fileName = input("Enter <user>__<file name>: ")
			to_send = f"file!download!request!@$:{fsport}!@$:" + fileName + "!@$:"
			s.send(to_send.ljust(1024, "#").encode())
			file_download(fileName)
	
		else:
			to_send = f"{date_now}!@$#:{name}!@$#:{to_send}!@$:"
			s.send(to_send.ljust(1024, "#").encode())
		
	except Exception as e:
		print(f"[!] Error: {e}")

s.close()
