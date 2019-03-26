#!/usr/bin/python

import ssl
import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('www.imssmartcontrol.com', 2900))
sslSocket = socket.ssl(s)
print repr(sslSocket.server())
print repr(sslSocket.issuer())
sslSocket.write('Hello secure socket\n')
s.close()
