#!/usr/bin/python
import socket
import time
import ssl

MSGLEN = 4096

class mysocket:
    '''demonstration class only
      - coded for clarity, not efficiency
    '''

    def __init__(self, sock=None):
        if sock is None:
            self.sock = socket.socket(
                socket.AF_INET, socket.SOCK_STREAM)
            #self.sock.settimeout(10)
        else:
            self.sock = sock

    def connect(self, host, port):
        self.sock.connect((host, port))

    def sslconnect(self, host, port):
        #self.sslsock=ssl.wrap_socket(self.sock, ssl_version=ssl.PROTOCOL_TLSv1_1, ciphers="ECDHE-RSA-AES256-GCM-SHA384")
        #context = ssl.create_default_context(ssl.Purpose.SERVER_AUTH)
        context = ssl.SSLContext(ssl.PROTOCOL_TLSv1_2)
        #context.options |= ssl.OP_NO_TLSv1 | ssl.OP_NO_TLSv1_1 # optional
        context.check_hostname = False
        context.verify_mode = ssl.CERT_NONE
        self.sslsock = context.wrap_socket(self.sock, server_hostname=host)
        self.sslsock.connect((host, port))

    def mysend(self, msg):
        totalsent = 0
        while totalsent < MSGLEN:
            sent = self.sock.send(msg[totalsent:])
            #sent = self.sock.send("IMVG_UP")
            if sent == 0:
                raise RuntimeError("socket connection broken")
            totalsent = totalsent + sent
    
    def sslsend(self, msg):
        totalsent = 0
        while totalsent < MSGLEN:
            #sent = self.sslsock.send(msg[totalsent:])
            sent = self.sock.send("IMVG_UP")
            if sent == 0:
                raise RuntimeError("socket connection broken")
            totalsent = totalsent + sent

    def myreceive(self):
        chunks = []
        bytes_recd = 0
        while bytes_recd < MSGLEN:
            chunk = self.sock.recv(min(MSGLEN - bytes_recd, 2048))
            if chunk == '':
                raise RuntimeError("socket connection broken")
            chunks.append(chunk)
            bytes_recd = bytes_recd + len(chunk)
        return ''.join(chunks)
    
    def sslreceive(self):
        chunks = []
        bytes_recd = 0
        while bytes_recd < MSGLEN:
            chunk = self.sslsock.recv(min(MSGLEN - bytes_recd, 2048))
            if chunk == '':
                raise RuntimeError("socket connection broken")
            chunks.append(chunk)
            bytes_recd = bytes_recd + len(chunk)
        return ''.join(chunks)

# Main Function
if __name__ == '__main__':
	m=mysocket()
	m.sslconnect("www.imssmartcontrol.com", 2900)
	print("Connected--- Great")
	while True:
		m.sslsend('IMVG_UP'.encode())
		#m.sslreceive()
		time.sleep(2)

