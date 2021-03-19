/**
 * @file xmod_log.h
 * @brief xmod_log header file
 * @copyright Copyright (c) 2021
 * 
 */
 
#ifndef XMOD_LOG
#define XMOD_LOG

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>

int check_envvar_set(const char* str);

int close_file(int file);

int create_log_file(char* str);

int open_file(char* str);

int prog_create(double instant, int pid, char** argv , int argc, char* path);

int send_proc_exit(double instant, int exit_status, int pid, char* path);

int signal_recv(double instant,int signal, int pid, char* path);

int file_modf(double instant, char* oldPerms, char* newPerms, char* path, int pid, char* open_path);

#endif //XMOD_LOG
