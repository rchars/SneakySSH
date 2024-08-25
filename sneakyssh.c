// gcc -fPIC -shared -o sneakyssh.so sneakyssh.c -ldl
// env SNEAKY_PASSW="SneakyPassw"$'\n' env LD_PRELOAD=./sneakyssh.so ssh sneakyserv
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef ssize_t (*write_t)(int fd, const void *buf, size_t count);
ssize_t write_auth(int fd, const void *buf, size_t count);

static write_t write_functions[2] = {write_auth, NULL};
static unsigned char write_func_index = 0;

ssize_t write(int fd, const void *buf, size_t count) {
    return write_functions[write_func_index](fd, buf, count);
}

ssize_t write_auth(int fd, const void *buf, size_t count) {
    write_functions[1] = (write_t)dlsym(RTLD_NEXT, "write");
    if (write_functions[1] == NULL) {
        fprintf(stderr, "Error: could not find original 'write' function: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    const char *message = getenv("SNEAKY_PASSW");
    if (message != NULL) {
        write_functions[1](fd, message, strlen(message));
    }
    else {
        fprintf(stderr, "You need to set up the SNEAKY_PASSW environment variable.\n");
    }

    write_func_index++;
    return write_functions[1](fd, buf, count);
}
