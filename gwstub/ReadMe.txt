Compilation:
gcc cgwstub.c -o gwstub -lcrypto -lssl
NOTE: This code will use the default SSL libraries in the system you use.

Usage:
gwstub <MAC_ID> <COMMAND> <TIMEOUT> <HOSTNAME> <PORT>

Example:[gwstub 00:0e:8f:65:7c:e2 IMVG_UP 5 www.imssmartcontrol.com 2920]

 Commands Supported:
 1. IMVG_UP
 2. IMVG_REGISTER
 3. KEEP_ALIVE
 4. RUN_IMVG
