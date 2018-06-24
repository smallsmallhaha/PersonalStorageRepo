#include <stdio.h>
#include <string.h>
#include <time.h>

int year = 2018;
int month = 6;
int day = 21;
int hour = 11;
int minute = 55;
int second = 50;

int main() {
  struct tm stm;
  memset(&stm, 0, sizeof(stm));
  stm.tm_year = year - 1900;
  stm.tm_mon = month - 1;
  stm.tm_mday = day;
  stm.tm_hour = hour;
  stm.tm_min = minute;
  stm.tm_sec = second;

  // struct tm to unix timestamp
  long timestamp = mktime(&stm);
  printf("unix timestamp: %ld\n", timestamp);

  // unix timestamp to struct tm
  struct tm *p = localtime(&timestamp);
}
