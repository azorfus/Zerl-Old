'''

	Written by Azorfus
	Github: @azorfus

'''

import socket
import threading
import time
import os
import csv

host = "192.168.1.11" # str(input("Enter Host IP: "))
port = 8080
MAX_LISTEN_TIME = 2
exit_flag = threading.Event()

client_sockets = []
s = socket.socket()
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((host, port))
s.listen(20)

print(f"[*] Listening as {host}:{port}")

def send_file(cs, file_name):
	try:
		file = open(file_name, "rb")
		file_size = os.path.getsize(file_name)

		rem = int(file_size % 1024)
		quo = int((file_size - rem)/1024)
		file_name_back = file_name.split("__")[1]
	
		# Packet with file size and name information.
		packet_info = str(quo) + ":@!" + str(rem) + ":@!" + file_name_back + ":@!"
		send_packet = packet_info.ljust(256, "#")
		cs.send(send_packet.encode())
		cs.sendall(file.read())
		
		file.close()

	except Exception as e:
		print(f"[!] Error: {e}")
		return 0

	return 0
	
def recv_file(kcs):
	cs_ip = kcs.getpeername()

	packet_info = kcs.recv(256).decode('utf-8', errors='ignore')
	packet_struct = packet_info.split(":@!$")

	rem = int(packet_struct[1])
	quo = int(packet_struct[0])

	with open(f"{packet_struct[3]}__{packet_struct[2]}", "wb") as file:

		count = 0
		packet = 0
		while count <= quo:
			if count == quo:
				packet = kcs.recv(rem)
			else:
				packet = kcs.recv(1024)
				print("Receiving... ", count*1024)
			file.write(packet)
			count += 1

		file.close()

	now = time.time()
	pong_recv = ""

	while pong_recv != "PONG":		
		if time.time() > now + 5:
			print("Timed out FTS...")
			break

		confirmation = "file!upload!success!@$:"
		confirmation = confirmation.ljust(1024, "#")
		kcs.send(confirmation.encode())
		
		pong_recv = kcs.recv(1024).decode()
		pong_recv = pong_recv.split("!@$:")[0]

	upload_info = f"{packet_struct[3]} uploaded file {packet_struct[2]} size: {quo*4096+rem} bytes.\nType \"!download\" and\n then enter the file name in the format <username>__<file name>\ni.e. \"{packet_struct[3]}__{packet_struct[2]}\" to download the file.\n"
	for client_socket in client_sockets:
		client_socket.send(upload_info.encode())
	
	return 0

def establish_conn(cs, file_name, FPORT, FLAG):

	# Start time of the server
	start_time = time.time()

	fs = socket.socket()
	fs.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	fs.bind((host, fport))
	fs.settimeout(120)
	print("[*] Listening for file tunnel connection...")

	# Listen for file tunnel connections within the time limit
	fs.listen(5)
	print("[*] Receiving connection requests to FTP server...")

	now = time.time()
	pong_recv = ""

	while pong_recv != "PONG":		
		if time.time() > now + 5:
			print("Timed out FTS...")
			return 0

		confirmation = "file!tunnel!open!@$:"
		confirmation = confirmation.ljust(1024, "#")
		cs.send(confirmation.encode())
		
		pong_recv = cs.recv(1024).decode()
		pong_recv = pong_recv.split("!@$:")[0]

	print("[!] Received Pong message from client.")
	
	while (time.time() - start_time) <= MAX_LISTEN_TIME:
		try:

			if FLAG == "DOWN":
				fcs, fcaddr = fs.accept()
				print(f"[*] File transfer tunnel established with {fcaddr}")
				send_file(fcs, file_name)
				print(f"[*] File transfer tunnel successfully closing...")
				break
				
				confirmation = s.recv(1024).decode()
				confirmation = confirmation.split("!@$:")[0]
			
				while confirmation != "file!download!success":
					time.sleep(2)
					print("[*] Waiting for download to finish....")
					confirmation = s.recv(1024).decode()
					confirmation = confirmation.split("!@$:")[0]

				fcs.close()
				fs.close()

			elif FLAG == "UP":	
				fcs, fcaddr = fs.accept()
				print(f"[*] File transfer tunnel established with {fcaddr}")
				recv_file(fcs)
				print(f"[*] File transfer tunnel successfully closing...")
				fcs.close()
				fs.close()
				break

		except Exception as e:
			if e == socket.timeout:
				print("Timing out...")
				pass  # Ignore timeout and continue listening
			
			elif e == ConnectionResetError:
				print("BRUH")
				pass

	return 0

def listen_for_client(cs):
	global log_writer
	while not exit_flag.is_set():
		try:	
			msg_R = cs.recv(1024).decode('utf-8', errors='ignore')
			msg = msg_R.split("!@$:")[0]

			if msg == "client!exit!code":
				print(f"[*] Client: {cs.getpeername()[0]} has closed the connection.")
				client_sockets.remove(cs)
				cs.close()
				break

			elif msg == "file!transfer!code":
				FPORT = int(msg_R.split("!@$:")[1])
				connThread = threading.Thread(target = establish_conn, args = (cs, "", FPORT, "UP" ))
				connThread.daemon = True
				connThread.start()

			elif msg.split("!@$:")[0] == "file!download!request":
				FPORT = int(msg_R.split("!@$:")[1])
				file_name = msg_R.split("!@$:")[2]
				connThread = threading.Thread(target = establish_conn, args = (cs, file_name, FPORT, "DOWN" ))
				connThread.daemon = True
				connThread.start()

			else:
				info = msg_R.split("!@$#:")
				log_writer.writerow([info[0], info[1], info[2].split("!@$:")[0], cs.getpeername()[0], 0])
				msg_R = f"[{info[0]}] {info[1]}: {info[2]}!@$:"
				msg = msg_R.ljust(1024, "#")
				for client_socket in client_sockets:
					client_socket.send(msg.encode())

		except Exception as e:
			print(f"[!] Error: {e}")
			client_sockets.remove(cs)
			cs.close()
			break

def server_term():
	global logfile
	while not exit_flag.is_set():
		cmd = input("$ ")
		if cmd == "ABORT":
			exit_flag.set()
			for i in threading.enumerate():
				i.join()
			for cs in client_sockets:
				abort_code = "server!abort!code!abort!@$:".ljust(1024, "#")
				cs.send(abort_code.encode())
				cs.close()
			s.close()
			break 
		if cmd == "logflush":
			logfile.flush()



with open("logging_file.csv", "w+") as logfile:
	log_writer = csv.writer(logfile, delimiter=",")

	term = threading.Thread(target = server_term)
	term.daemon = True
	term.start()
	
	while True:
		client_socket, client_address = s.accept()
		print(f"[+] {client_address} connected.")
		client_sockets.append(client_socket)
	
		fport = 8081 + client_sockets.index(client_socket)
		sport = f"{fport}!@$:".ljust(256, "#")
		client_socket.send(sport.encode())
	
		client_t = threading.Thread(target=listen_for_client, args=(client_socket,))
		client_t.daemon = True
		client_t.start()

s.close()
