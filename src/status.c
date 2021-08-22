#include "status.h"

char buffer_path[100];
char buffer_time[100];
char buffer_size[24];
char buffer_boot[24];

int max = 10;
char proc[24] = {"/proc/"};
FILE *read_path_logs;

void check_status() {

if( (access(pid_file, F_OK)) == 0 ) {
  get_pid = fopen(pid_file, "r");
    if(get_pid == NULL){
        printf("Error");
      exit(1);
    } else {
      fgets(buffer_pid, sizeof(buffer_pid), get_pid);
      strncat(proc, buffer_pid, 10);
        DIR *dir = opendir(proc);
        if (dir) {
            flag_running = 1;
            closedir(dir);
            buffer_smtp = malloc(max);
            if( (access(smtp_file, F_OK)) == 0 ){
              strncpy(buffer_smtp, "enabled", 10);
            } else {
              strncpy(buffer_smtp, "disabled", 10);
            }
          } else if (ENOENT == errno) {
              flag_running = 0;
              closedir(dir);
          }
      }
  } else {
    flag_running = 0;
  }
}

void read_path(){
  readpath = fopen(path_conf, "r");
  fgets(buffer_path, sizeof(buffer_path), readpath);
}

void read_time(){
  readtime = fopen(time_conf, "r");
  fgets(buffer_time, sizeof(buffer_time), readtime);
}

void read_boot() {
  readboot = fopen(boot_conf, "r");
  fgets(buffer_boot, sizeof(buffer_boot), readboot);
}

void read_size(){

  struct stat size;
  stat(buffer_path, &size);

// Writing File
  filesize = fopen(size_conf, "w");
  fprintf(filesize, "%d", size.st_size);
  fclose(filesize);

// Reading File
  readsize = fopen(size_conf, "r");
  fgets(buffer_size, sizeof(buffer_size), readsize);
}

void print_path_logs(){
  read_path();
  char *buffer_path_file = buffer_path;
  char lines[6][256];
  size_t i = 0;
  read_path_logs = fopen(buffer_path_file, "r");

  if (read_path_logs != NULL) {
      while (fgets(lines[i % 6], sizeof(lines[i % 6]), read_path_logs) != NULL) {
          i++;
      }
      fclose(read_path_logs);
      for (size_t j = i < 6 ? 0 : i - 6; j < i; j++) {
          fputs(lines[j % 6], stdout);
      }
  }
}
