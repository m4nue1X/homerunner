
IMPORTANT: to allow non root users to initiate the BTLE scan
you have to do the following
(see http://unix.stackexchange.com/questions/96106/bluetooth-le-scan-as-non-root)

> sudo setcap 'cap_net_raw,cap_net_admin+eip' <path/hrnode>

NOTE: to be able to debug the program you must allow gdb
execution using sudo without password by adding the following
to /etc/sudoers
> <youruser>  ALL=(ALL) NOPASSWD:/usr/bin/gdb

In your IDE specify
> sudo gdb
as debugger command


https://github.com/xorbit/py-decode-beacon/blob/master/IBEACON.md
