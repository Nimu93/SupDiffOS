#ifndef USYSCALL_H
#define USYSCALL_H

#include <stdint.h>

/* Syscall numbers: keep in sync with kernel/includes/syscall.h. */
#define SYS_EXIT    0
#define SYS_WRITE   1
#define SYS_READ    2
#define SYS_GETPID  3

static inline int syscall0(int num) {
    int ret;
    asm volatile(
        "int $0x80"
        : "=a"(ret)
        : "a"(num)
        : "memory"
    );
    return ret;
}

static inline int syscall1(int num, int arg1) {
    int ret;
    asm volatile(
        "int $0x80"
        : "=a"(ret)
        : "a"(num), "b"(arg1)
        : "memory"
    );
    return ret;
}

static inline int syscall2(int num, int arg1, int arg2) {
    int ret;
    asm volatile(
        "int $0x80"
        : "=a"(ret)
        : "a"(num), "b"(arg1), "c"(arg2)
        : "memory"
    );
    return ret;
}

static inline int syscall3(int num, int arg1, int arg2, int arg3) {
    int ret;
    asm volatile(
        "int $0x80"
        : "=a"(ret)
        : "a"(num), "b"(arg1), "c"(arg2), "d"(arg3)
        : "memory"
    );
    return ret;
}

static inline void exit(int status) {
    syscall1(SYS_EXIT, status);
    for (;;) {}
}

static inline int write(int fd, const char *buf, uint32_t count) {
    return syscall3(SYS_WRITE, fd, (int)buf, (int)count);
}

static inline int print(const char *str) {
    int len = 0;
    while (str[len]) len++;
    return write(1, str, len);
}

static inline int getpid(void) {
    return syscall0(SYS_GETPID);
}

#endif
