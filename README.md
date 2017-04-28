# hhvm-gearman
hhvm gearman extension

## Only client API is supported

# Installation

1. `$ ./build.sh`
2. `$ sudo cp gearman.so /usr/lib64/hhvm/extensions
3.  $ echo 'hhvm.extensions[gearman] = gearman.so' | tee -a /etc/hhvm/server.ini
4. Restart HHVM.

