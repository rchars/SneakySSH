# SneakySSH
A very simple ssh LD_PRELOAD library that allows connecting to an ssh server through SneakyServ (https://github.com/rchars/SneakyServ).
## Compilation
gcc -fPIC -shared -o sneakyssh.so sneakyssh.c -ldl
## Usage
env SNEAKY_PASSW="SneakyPassw"$'\n' env LD_PRELOAD=./sneakyssh.so ssh sneakyserv
