#! /usr/bin/python
# a simple tcp client
import socket  
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
sock.connect(('localhost', 42124))  
#sock.send('Test\n')  
sock.send(b'\x01gkarbouc\x00' + b'\x42'*64)  
sock.close()
