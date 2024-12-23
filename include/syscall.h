#pragma once

long sys_getpid();
long sys_read(char *buf, long count);
long sys_write(const char *buf, long count);
int sys_exec(const char *pathname, const char *const *argv);
long sys_fork();
void sys_exit(int status);
int sys_kill(long pid);