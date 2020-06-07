import time
from socket import *

serverName = '192.168.1.114'
serverPort = 8001
while(True):
	clientSocket = socket(AF_INET, SOCK_STREAM)
	clientSocket.connect((serverName, serverPort))
	data = 'client'
	clientSocket.send(data.encode())
	data = clientSocket.recv(1024)
	print(data.decode(), end='')
	print('\r')
	time.sleep(5)