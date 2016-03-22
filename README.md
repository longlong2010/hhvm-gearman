# hhvm-gearman
hhvm gearman extension

## Only client API is supported

# build
```bash
hphpize
cmake .
make
cp gearman.so /etc/hhvm/
```


# configure

add this to your /etc/hhvm/php.ini

```
extension_dir = /etc/hhvm
hhvm.extensions[gearman] = gearman.so

```


