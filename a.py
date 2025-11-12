#! /usr/bin/python
# a simple tcp client
import socket  
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
sock.connect(('10.12.6.6', 42124))  
#sock.send('Test\n')  
sock.send(input("Please input : ").encode('utf-8'))  
sock.close()
