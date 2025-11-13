#! /usr/bin/python
import socket 
import time

def send_packet(opcode: int, data: bytes, sock) -> None:
	sock.send(opcode.to_bytes(4, "little"))	
	sock.send(len(data).to_bytes(2, "little"))
	sock.send(data)

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
sock.connect(('localhost', 42124))  
#sock.send('Test\n')  
#send_packet(1, "AABBCC".encode("ascii") + b'\0' + 64 * b'\x40', sock)
send_packet(2, "AABBCC".encode("ascii") + b'\0' + 64 * b'\x42', sock)
send_packet(2, "AABBCC".encode("ascii") + b'\0' + 64 * b'\x42', sock)
time.sleep(0.5)
print(sock.recv(6+4))
send_packet(1, "AABBCC".encode("ascii") + b'\0' + 64 * b'\x40', sock)
sock.close()
