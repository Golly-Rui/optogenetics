import socket
import threading
import time

s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)      #创建一个socket
s.bind(('192.168.2.133',9999))                          #绑定监听地址和端口
s.listen(5)                                             #开始监听端口
print('waiting for connection...')

def tcplink(sock,addr):
    print('Accept new connection from %s:%s...'%addr)
    sock.send(b'Welcome!')
    while True:
        data=sock.recv(1024)
        time.sleep(1)
        #if not data or data.decode('utf-8')=='exit':
        #    break
        #print('get data!')
        #sock.send(('Hello,%s!' %data.decode('utf-8')).encode('utf-8'))
        print(('Hello,%s!' %data.decode('utf-8')).encode('utf-8'))
    #sock.close()
    #print('Connection from %s:%s closed.'%addr)

while True:
    sock,addr=s.accept()                                #接受一个新连接
    t=threading.Thread(target=tcplink,args=(sock,addr)) #创建新线程来处理TCP连接
    t.start()
