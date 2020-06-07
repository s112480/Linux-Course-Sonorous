import socket
import time

HOST = '192.168.1.114'  # 内网IP地址
PORT = 8001  # 端口号
# socket.AF_INET用于服务器与服务器之间的网络通信
# socket.SOCK_STREAM代表基于TCP的流式socket通信
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind((HOST, PORT))
sock.listen(1)  # 最多连接1个设备
while True:
    connection, address = sock.accept()
    try:
        buf = ""
        msg = connection.recv(1024).decode()
        connection.settimeout(10)
        if msg == "client":  # 接收成功
            print("connect success")
            print(msg)
            buf += time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
            f = open('/mnt/lp/data.txt')
            buf += "\ntemperature is: "
            buf += f.readline()
            buf += "\nPM2.5 is: "
            buf += f.readline()
            buf += "\nHumidity is: "
            buf += f.readline()
            f.close()
            connection.send(buf.encode())
        else:  # 接收失败
            connection.send(b'Fail!')
    except socket.timeout:
        print('time out!')
    connection.close()
