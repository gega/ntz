#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define NTZ_ABBREV 1
#define NTZ_NAME_API 1
#define NTZ_IMPLEMENTATION
#include "ntz.h"


int main(int argc, char **argv)
{
  static const char days[][3]={"??","SU","MO","TU","WE","TH","FR","SA" };
  if(argc<3)
  {
    printf("Usage: %s iana-timezone epoch\n",argv[0]);
    return(1);
  }
  int len=strlen(argv[1]);

  const struct ntz_iana *tz;
  tz=ntz_find_tz_name(argv[1],len);
  //printf("size=%ld\n",sizeof(ntz_db)+sizeof(ntz_rules));

  if(tz!=NULL)
  {
    struct ntz_tm tm;
    ntz_epoch_to_localtime( atoll(argv[2]), &tm, tz );
    printf("localtime=%4d.%02d.%02d %02d:%02d:%02d %s\n",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec,days[tm.tm_wday]);
    printf("offset=%d\ndst_rule='%s'\ndst_offset=%dmin\ntz=%s\n",ntz_minute_offsets[tz->offset],ntz_rules[tz->dst_rule], ntz_get_dst_offset_min(atoll(argv[2]),tz),
                                                                            ntz_abbrev[tz->abbrev]);
  }
  else printf("not found\n");

  return(0);
}
