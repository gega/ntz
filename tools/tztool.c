#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
  if(argc<3)
  {
    printf("Usage: %s timezone epoch\n",argv[0]);
    return(1);
  }
  time_t t = atoll(argv[2]);
  setenv("TZ", argv[1], 1);
  tzset();

  struct tm tm;
  localtime_r(&t, &tm);

  printf("localtime=%04d.%02d.%02d %02d:%02d:%02d %.2s\n",
    tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
    tm.tm_hour, tm.tm_min, tm.tm_sec,
    "SUMOTUWETHFRSA"+tm.tm_wday*2);

  int standard_offset = tm.tm_gmtoff/60 - (tm.tm_isdst ? 60 : 0);

  struct tm tm_jan1;
  struct tm tm_jul1;
  time_t jan1=1767254400;
  time_t jul1=1782889200;
  localtime_r(&jan1, &tm_jan1);
  localtime_r(&jul1, &tm_jul1);

#ifdef __USE_MISC
    printf("offset=%d\ndst_rule=n/a\ndst_offset=%dhr\ntz=%s\ntz=%s\n",
        standard_offset,
        tm.tm_isdst ? 1 : 0,
        tm_jan1.tm_zone,
        tm_jul1.tm_zone);
#else
#error missing MISC
#endif
}
