#include "event.h"

char *event_keyboard(void){

  FILE *eventx;
  char event_str[10];
  char *keyboard_file = NULL;
  char input[20] = "/dev/input/";
  char event_grep[256] = "grep -E 'Handlers|EV=' /proc/bus/input/devices | grep -B1 'EV=120013' | grep -Eo 'event[0-9]{1,2}'";
  eventx = popen(event_grep, "r");
  fgets(event_str,10,eventx);
  event_str[strcspn(event_str, "\n")] = 0;
  strcat(input, event_str);
  keyboard_file = strdup(input);
  pclose(eventx);

  return keyboard_file;
}
