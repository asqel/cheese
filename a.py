#! /usr/bin/python
import socket 
import time
from hashlib import sha256

OPC_ERROR = 0
OPC_CREATE_ACC = 1
OPC_AUTH_ACC = 2
OPC_SUCCESS = 3
OPC_JOIN = 4
OPC_CREATE_ROOM = 5

def send_packet(opcode: int, data: bytes, sock) -> None:
	sock.send(opcode.to_bytes(4, "little"))	
	sock.send(len(data).to_bytes(2, "little"))
	sock.send(data)

def recvall(sock, n):
	data = b""
	while len(data) < n:
		chunk = sock.recv(n - len(data))
		if not chunk:
			return None 
		data += chunk
	return data

def print_response(sock) -> None:
	opcode = recvall(sock, 4)
	pack_len = recvall(sock, 2)
	pack = recvall(sock, pack_len)
	

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
sock.connect(('localhost', 42124))  
while 1:
	cmd = input(">").replace('\n', '')
	if cmd.startswith("auth "):
		name = cmd[len("auth "):].encode("utf-8")
		passwd = input(">").replace('\n', '').encode("utf-8")
		passwd = sha256(passwd).hexdigest().encode("utf-8")
		send_packet(OPC_AUTH_ACC, name + b'\0' + passwd)
	



sock.close()
