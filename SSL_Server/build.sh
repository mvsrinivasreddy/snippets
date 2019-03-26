gcc -I./openssl-1.1.0e/include/ ssl_simple_server.c -o ssl_server openssl-1.1.0e/libssl.a openssl-1.1.0e/libcrypto.a -lpthread -ldl
