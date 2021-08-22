#ifndef STATUS_H
#define STATUS_H

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

void check_status();
void read_path();
void read_time();
void read_size();
void read_boot();
void print_path_logs();
void print_smtp_logs();

extern const char *pid_file;

extern FILE *get_pid;
extern FILE *readpath;
extern FILE *readtime;
extern FILE *filesize;
extern FILE *readsize;
extern FILE *readboot;

extern char buffer_pid[32];
extern char *buffer_smtp;

extern int flag_running;

extern const char *path_conf;
extern const char *time_conf;
extern const char *size_conf;
extern const char *smtp_file;
extern const char *boot_conf;

extern char outfile[200];

extern char buffer_path[100];
extern char buffer_time[100];
extern char buffer_size[24];
extern char buffer_boot[24];

#endif
