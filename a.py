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
OPC_PING = 0xFFABBAFF

opcodes_str = [
	"Error",
	"Create acc",
	"Auth acc",
	"Success",
	"Join",
	"Create room"
]

error_str = [
	"None",
	"Name length",
	"Name taken",
	"Invalid data",
	"Already auth",
	"Wrong passwd",
	"Server failure",
	"Invalid name",
	"Invalid room type",
	"Already in room",
	"Not in room",
	"Forbidden"
]

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
	opcode = int.from_bytes(recvall(sock, 4), "little")
	pack_len = int.from_bytes(recvall(sock, 2), "little")
	pack = recvall(sock, pack_len)
	if (opcode >= len(opcodes_str) and opcode != OPC_PING):
		return None
	if opcode == OPC_PING:
		print("PONG", end="")
	else:
		print(f"response\n  {opcodes_str[opcode]}: ", end="")
	if (opcode == OPC_ERROR and pack_len == 4):
		err = int.from_bytes(pack, "little")
		if err >= len(error_str):
			err = 0
		print(f"{error_str[err]}", end="")
	elif (opcode == OPC_SUCCESS and pack_len == 4):
		reason = int.from_bytes(pack, "little")
		if (reason >= len(opcodes_str)):
			reason = "Unknown"
		else:
			reason = opcodes_str[reason]
		print(f"{reason}", end="")
	print()


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
sock.connect(('asqel.ddns.net', 42420))  
while 1:
	cmd = input(">").replace('\n', '')
	if cmd.startswith("auth "):
		name = cmd[len("auth "):].encode("utf-8")
		passwd = input(">").replace('\n', '').encode("utf-8")
		passwd = sha256(passwd).hexdigest().encode("utf-8")
		send_packet(OPC_AUTH_ACC, name + b'\0' + passwd, sock)
	if cmd.startswith("create "):
		name = cmd[len("create "):].encode("utf-8")
		passwd = input(">").replace('\n', '').encode("utf-8")
		passwd = sha256(passwd).hexdigest().encode("utf-8")
		send_packet(OPC_CREATE_ACC, name + b'\0' + passwd, sock)
	if cmd.startswith("ping"):
		send_packet(OPC_PING, b'', sock)

	if cmd.startswith("croom "):
		name = cmd[len("croom "):].encode("utf-8")
		passwd = input(">").replace('\n', '').encode("utf-8")
		passwd = sha256(passwd).hexdigest().encode("utf-8")
		type = int(input(">")).to_bytes(1, "little")
		send_packet(OPC_CREATE_ROOM, name + b'\0' + passwd + type, sock)
		
	print_response(sock)
	



sock.close()
