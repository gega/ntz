/*
  BSD 2-Clause License

  Copyright (c) 2026, Gergely Gati

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef NTZ_H
#define NTZ_H

#ifndef NTZ_ABBREV
#define NTZ_ABBREV 0
#endif

#ifndef NTZ_NAME_API
#define NTZ_NAME_API 1
#endif

#include <stdint.h>


struct ntz_iana
{
  uint16_t iana_name_hash;
#if NTZ_ABBREV == 1
  uint8_t abbrev;
#endif
  int8_t offset;
  uint8_t dst_rule;
};

struct ntz_tm
{
  int16_t tm_year;
  int8_t tm_mon;
  int8_t tm_mday;
  int8_t tm_hour;
  int8_t tm_min;
  int8_t tm_sec;
  int8_t tm_wday; /* Day of the week  [0, 6]   (Sunday = 0) */
};

/* {ntz::defines -- generated code do not touch */
#define NTZ_HASH_SHIFT (3)
#define NTZ_HASH "sha1sum"
#define NTZ_TZDATA_VERSION "2026b"
#define NTZ_GMT_HASH 0x02be
/* }ntz::defines -- generated code do not touch */

#define ntz_get_rule(iana) ntz_rules[(iana)->dst_rule]
#define ntz_get_offset(iana) ntz_minute_offsets[(iana)->offset]
#if NTZ_ABBREV == 1
#define ntz_get_abbrev(iana) ntz_abbrev[(iana)->abbrev]
#endif

const struct ntz_iana *ntz_find_tz_key( uint16_t key );
#if NTZ_NAME_API == 1
const struct ntz_iana *ntz_find_tz_name( const char *name, int len );
#endif
int ntz_day_of_week( int year, int month, int day );
int ntz_epoch_to_tm( int64_t epoch, struct ntz_tm *tm, const struct ntz_iana *iana );
int ntz_get_dst_offset_min( int64_t epoch, const struct ntz_iana *iana );
int ntz_dst_offset_min( struct ntz_tm *tm, const struct ntz_iana *iana );
int ntz_epoch_to_localtime( int64_t epoch, struct ntz_tm *tm, const struct ntz_iana *iana );

#endif

#ifdef NTZ_IMPLEMENTATION

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) ((sizeof x) / (sizeof *x))
#endif

#define NTZ_DECODE_INT(c) ( ((c)>='a'&&(c)<='z') ? (-1*((c)-'a'+1)) : ( ((c)>='0'&&(c)<='9') ? ((c)-'0')    : 0 ) )
#define NTZ_DECODE_DAY(c) ( ((c)>='a'&&(c)<='z') ? ((c)-'a'+1)      : ( ((c)>='0'&&(c)<='4') ? ((c)-'0'+27) : 0 ) )

enum ntz_rulelist
{
/* {ntz::enum_ntz_rulelist -- generated code do not touch */
  RULE_0_________,
  RULE_1cDD11JD11,
  RULE_2bDD11ID11,
  RULE_2bJDa1CDa7,
  RULE_2bKD11CD21,
  RULE_2cDD11IDa1,
  RULE_2cDD11JD11,
  RULE_2cDD17ID17,
  RULE_2cJDa1CDa1,
  RULE_2cJDa1CDa6,
  RULE_2cJDa6DDa6,
  RULE_2cJDa7CDa7,
  RULE_2cKD11CD21,
  RULE_4cJDa1CDa1,
  RULE_bcCDa1JDa1,
/* }ntz::enum_ntz_rulelist -- generated code do not touch */
  RULECOUNT
};

const char ntz_rules[][11] = {
/* {ntz::const_char_ntz_rules -- generated code do not touch */
  [RULE_0_________] = "0---------",
  [RULE_1cDD11JD11] = "1cDD11JD11",
  [RULE_2bDD11ID11] = "2bDD11ID11",
  [RULE_2bJDa1CDa7] = "2bJDa1CDa7",
  [RULE_2bKD11CD21] = "2bKD11CD21",
  [RULE_2cDD11IDa1] = "2cDD11IDa1",
  [RULE_2cDD11JD11] = "2cDD11JD11",
  [RULE_2cDD17ID17] = "2cDD17ID17",
  [RULE_2cJDa1CDa1] = "2cJDa1CDa1",
  [RULE_2cJDa1CDa6] = "2cJDa1CDa6",
  [RULE_2cJDa6DDa6] = "2cJDa6DDa6",
  [RULE_2cJDa7CDa7] = "2cJDa7CDa7",
  [RULE_2cKD11CD21] = "2cKD11CD21",
  [RULE_4cJDa1CDa1] = "4cJDa1CDa1",
  [RULE_bcCDa1JDa1] = "bcCDa1JDa1",
/* }ntz::const_char_ntz_rules -- generated code do not touch */
};

/* {ntz::ntz_minute_offset_defines -- generated code do not touch */
#define NTZ_OFFSET_m720 0
#define NTZ_OFFSET_m660 1
#define NTZ_OFFSET_m600 2
#define NTZ_OFFSET_m570 3
#define NTZ_OFFSET_m540 4
#define NTZ_OFFSET_m480 5
#define NTZ_OFFSET_m420 6
#define NTZ_OFFSET_m360 7
#define NTZ_OFFSET_m300 8
#define NTZ_OFFSET_m240 9
#define NTZ_OFFSET_m210 10
#define NTZ_OFFSET_m180 11
#define NTZ_OFFSET_m120 12
#define NTZ_OFFSET_m60 13
#define NTZ_OFFSET_p0 14
#define NTZ_OFFSET_p120 15
#define NTZ_OFFSET_p180 16
#define NTZ_OFFSET_p210 17
#define NTZ_OFFSET_p240 18
#define NTZ_OFFSET_p270 19
#define NTZ_OFFSET_p300 20
#define NTZ_OFFSET_p330 21
#define NTZ_OFFSET_p345 22
#define NTZ_OFFSET_p360 23
#define NTZ_OFFSET_p390 24
#define NTZ_OFFSET_p420 25
#define NTZ_OFFSET_p480 26
#define NTZ_OFFSET_p525 27
#define NTZ_OFFSET_p540 28
#define NTZ_OFFSET_p570 29
#define NTZ_OFFSET_p60 30
#define NTZ_OFFSET_p600 31
#define NTZ_OFFSET_p630 32
#define NTZ_OFFSET_p660 33
#define NTZ_OFFSET_p720 34
#define NTZ_OFFSET_p765 35
#define NTZ_OFFSET_p780 36
#define NTZ_OFFSET_p840 37
/* }ntz::ntz_minute_offset_defines -- generated code do not touch */

const static int16_t ntz_minute_offsets[]=
{
/* {ntz::ntz_minute_offsets -- generated code do not touch */
  [NTZ_OFFSET_m720] = -720,
  [NTZ_OFFSET_m660] = -660,
  [NTZ_OFFSET_m600] = -600,
  [NTZ_OFFSET_m570] = -570,
  [NTZ_OFFSET_m540] = -540,
  [NTZ_OFFSET_m480] = -480,
  [NTZ_OFFSET_m420] = -420,
  [NTZ_OFFSET_m360] = -360,
  [NTZ_OFFSET_m300] = -300,
  [NTZ_OFFSET_m240] = -240,
  [NTZ_OFFSET_m210] = -210,
  [NTZ_OFFSET_m180] = -180,
  [NTZ_OFFSET_m120] = -120,
  [NTZ_OFFSET_m60] = -60,
  [NTZ_OFFSET_p0] = +0,
  [NTZ_OFFSET_p120] = +120,
  [NTZ_OFFSET_p180] = +180,
  [NTZ_OFFSET_p210] = +210,
  [NTZ_OFFSET_p240] = +240,
  [NTZ_OFFSET_p270] = +270,
  [NTZ_OFFSET_p300] = +300,
  [NTZ_OFFSET_p330] = +330,
  [NTZ_OFFSET_p345] = +345,
  [NTZ_OFFSET_p360] = +360,
  [NTZ_OFFSET_p390] = +390,
  [NTZ_OFFSET_p420] = +420,
  [NTZ_OFFSET_p480] = +480,
  [NTZ_OFFSET_p525] = +525,
  [NTZ_OFFSET_p540] = +540,
  [NTZ_OFFSET_p570] = +570,
  [NTZ_OFFSET_p60] = +60,
  [NTZ_OFFSET_p600] = +600,
  [NTZ_OFFSET_p630] = +630,
  [NTZ_OFFSET_p660] = +660,
  [NTZ_OFFSET_p720] = +720,
  [NTZ_OFFSET_p765] = +765,
  [NTZ_OFFSET_p780] = +780,
  [NTZ_OFFSET_p840] = +840,
/* }ntz::ntz_minute_offsets -- generated code do not touch */
};

int ntz_check_rule( const char *r )
{
  if ( NULL == r )
    return 0;
  if ( strlen( r ) != 10 )
    return 0;
  if ( r[0] == '0' )
    return 1;
  int f = NTZ_DECODE_INT( r[0] );
  if ( f < -3 || f > 3 )
    return 0;
  f = NTZ_DECODE_DAY( r[1] ) - 1;
  if ( f < 0 || f > 24 )
    return 0;
  int ret = 1;
  int n = 2;
  const char *codes = "DXA";
  for ( int i = 0; i < 2; i++ )
  {
    f = ( ( int )r[n] ) - ( int )'A';
    if ( f < 0 || f > 12 )
      ret = 0;
    n++;
    if ( strchr( codes, r[n] ) == NULL )
      ret = 0;
    n += 3;
  }
  return ret;
}

int ntz_day_of_week(int year, int month, int day)
{
  if (month < 1 || month > 12 || day < 1 || day > 31)
    return 0;

  static const int t[] =
  {
    0, 3, 2, 5, 0, 3,
    5, 1, 4, 6, 2, 4
  };

  int y = year;

  // Zeller-style shift
  if (month < 3)
    y -= 1;

  int w =
    (y + y/4 - y/100 + y/400 + t[month - 1] + day) % 7;

  return w+1;
}

int ntz_calc_day( int year, int month, char ty, char d1, char d2 )
{                               // month 1..12
  /*
     tipus:
     X-pontos datum
     D-valahanyadik elofordulasa egy napnak (utolso kedd pl)
     A-valahanyadika utan az elso nap (12-e utan az elso szerda)
     tipusfuggo:
     X: 00 .. 31
     D: elso szamjegy: hanyadik elofordulas
     (1,2,3,4 elso, masodik vagy a,b,c,d utolso utolso elotti stb)
     masidk: 1 vasarnap .. 7 szombat
     A: datum: a-z 1..26-ig 0-5 : 27-31-e
     nap: mint D
   */
  int ret = 0;
  int day;
  if ( ty == 'D' )
  {
    int oc = NTZ_DECODE_INT( d1 );
    int tw = d2 - '1' + 1;
    if ( oc > 0 )
    {
      for ( int i = 1; i <= 31 && oc > 0; i++ )
      {
        ret = i;
        if ( ntz_day_of_week( year, month, ret ) == tw )
          oc--;
      }
    } else if ( oc < 0 )
    {
      oc = -oc;
      for ( int i = 31; i > 0 && oc > 0; i-- )
      {
        ret = i;
        if ( ntz_day_of_week( year, month, ret ) == tw )
          oc--;
      }
    }
  } else if ( ty == 'A' )
  {
    day = NTZ_DECODE_DAY( d1 );
    int tw = d2 - '1' + 1;
    for ( int i = 0; i < 8 && day + i <= 31; i++ )
    {
      ret = ntz_day_of_week( year, month, day + i );
      if ( ret == tw )
      {
        ret = day + i;
        break;
      }
    }
  } else if ( ty == 'X' )
  {
    ret = ( d1 - '0' ) * 10 + ( d2 - '0' );
  }
  return ret;
}

static void ntz_civil_from_days( int64_t z, int *year, int *month, int *day )
{
  z += 719468;
  int64_t era = ( z >= 0 ? z : z - 146096 ) / 146097;
  unsigned doe = ( unsigned )( z - era * 146097 );
  unsigned yoe = ( doe - doe / 1460 + doe / 36524 - doe / 146096 ) / 365;
  int y = ( int )yoe + ( int )era * 400;
  unsigned doy = doe - ( 365 * yoe + yoe / 4 - yoe / 100 );
  unsigned mp = ( 5 * doy + 2 ) / 153;
  unsigned d = doy - ( 153 * mp + 2 ) / 5 + 1;
  unsigned m = mp + ( mp < 10 ? 3 : -9 );
  y += ( m <= 2 );
  *year = y;
  *month = m;
  *day = d;
}

int ntz_epoch_to_tm( int64_t epoch, struct ntz_tm *tm, const struct ntz_iana *iana )
{
  if ( !tm || !iana )
    return ( -1 );
  epoch += ( int64_t ) ntz_minute_offsets[iana->offset] * 60;
  int64_t days = epoch / 86400;
  int64_t rem = epoch % 86400;
  if ( rem < 0 )
  {
    rem += 86400;
    days--;
  }
  int year;
  int month;
  int day;
  ntz_civil_from_days( days, &year, &month, &day );
  tm->tm_year = year - 1900;
  tm->tm_mon = month - 1;
  tm->tm_mday = day;
  tm->tm_hour = rem / 3600;
  tm->tm_min = ( rem % 3600 ) / 60;
  tm->tm_sec = rem % 60;
  tm->tm_wday = ntz_day_of_week( year, month, day);
  return ( 0 );
}

int ntz_dst_offset_min( struct ntz_tm *tm, const struct ntz_iana *iana )
{
  if ( !iana || !tm )
    return ( 0 );
  const char *rule = ntz_get_rule( iana );
  if ( !ntz_check_rule( rule ) )
    return ( 0 );
  if ( rule[0] == '0' )
    return 0;
  int offset = NTZ_DECODE_INT( rule[0] )*30;
  int when = NTZ_DECODE_DAY( rule[1] ) - 1;
  int months = ( ( int )rule[2] ) - ( int )'A'; // std
  int monthd = ( ( int )rule[6] ) - ( int )'A'; // dst

  int year = tm->tm_year + 1900;
  int month = tm->tm_mon;
  int day = tm->tm_mday;
  int hour = tm->tm_hour;
  if ( month != monthd && month != months )
  {
    if ( monthd < months )
    {
      if ( month > monthd && month < months ) return offset;
      else return 0;
    }
    else
    {
      if ( month > months && month < monthd ) return 0;
      else return offset;
    }
  }

  int dayn;
  if ( month == monthd )
  {
    // dst transition month
    dayn = ntz_calc_day( year, month + 1, rule[7], rule[8], rule[9] );
    if ( day < dayn )
      return 0;
    if ( day > dayn )
      return offset;
    if ( hour < when )
      return 0;
    else
      return offset;
  }
  else
  {
    // std transition month
    dayn = ntz_calc_day( year, month + 1, rule[3], rule[4], rule[5] );
    if ( day < dayn )
      return offset;
    if ( day > dayn )
      return 0;
    if ( hour < when )
      return offset;
    else
      return 0;
  }
}

int ntz_get_dst_offset_min( int64_t epoch, const struct ntz_iana *iana )
{
  int dst;
  struct ntz_tm dat;
  if ( !iana )
    return ( 0 );

  ntz_epoch_to_tm( epoch, &dat, iana );
  dst = ntz_dst_offset_min( &dat, iana );

  return ( dst );
}

int ntz_epoch_to_localtime( int64_t epoch, struct ntz_tm *tm, const struct ntz_iana *iana )
{
  if ( !iana || !tm )
    return ( -1 );
  int dst = ntz_get_dst_offset_min( epoch, iana );
  epoch += dst * 60;
  ntz_epoch_to_tm( epoch, tm, iana );
  return ( 0 );
}

#if NTZ_ABBREV == 1

/* {ntz::define_abbrev -- generated code do not touch */
#define p00 0,
#define p01 1,
#define m01 2,
#define p02 3,
#define m02 4,
#define p03 5,
#define m03 6,
#define p0330 7,
#define p04 8,
#define m04 9,
#define p0430 10,
#define p05 11,
#define m05 12,
#define p0530 13,
#define p0545 14,
#define p06 15,
#define m06 16,
#define p0630 17,
#define p07 18,
#define m07 19,
#define p08 20,
#define m08 21,
#define p0845 22,
#define p09 23,
#define m09 24,
#define m0930 25,
#define p10 26,
#define m10 27,
#define p11 28,
#define m11 29,
#define p12 30,
#define m12 31,
#define p13 32,
#define p1345 33,
#define p14 34,
#define ACST 35,
#define ADT 36,
#define AEDT 37,
#define AEST 38,
#define AKDT 39,
#define AST 40,
#define AWST 41,
#define BST 42,
#define CAT 43,
#define CDT 44,
#define CET 45,
#define ChST 46,
#define CST 47,
#define EAT 48,
#define EDT 49,
#define EET 50,
#define EST 51,
#define GMT 52,
#define HDT 53,
#define HKT 54,
#define HST 55,
#define IDT 56,
#define IST 57,
#define JST 58,
#define KST 59,
#define MDT 60,
#define MSK 61,
#define MST 62,
#define NST 63,
#define NZDT 64,
#define PDT 65,
#define PKT 66,
#define PST 67,
#define SAST 68,
#define SST 69,
#define UTC 70,
#define WAT 71,
#define WET 72,
#define WIB 73,
#define WIT 74,
#define WITA 75,
/* }ntz::define_abbrev -- generated code do not touch */

const char *ntz_abbrev[] = {
/* {ntz::const_char_ntz_abbrev -- generated code do not touch */
  "+00",
  "+01",
  "-01",
  "+02",
  "-02",
  "+03",
  "-03",
  "+0330",
  "+04",
  "-04",
  "+0430",
  "+05",
  "-05",
  "+0530",
  "+0545",
  "+06",
  "-06",
  "+0630",
  "+07",
  "-07",
  "+08",
  "-08",
  "+0845",
  "+09",
  "-09",
  "-0930",
  "+10",
  "-10",
  "+11",
  "-11",
  "+12",
  "-12",
  "+13",
  "+1345",
  "+14",
  "ACST",
  "ADT",
  "AEDT",
  "AEST",
  "AKDT",
  "AST",
  "AWST",
  "BST",
  "CAT",
  "CDT",
  "CET",
  "ChST",
  "CST",
  "EAT",
  "EDT",
  "EET",
  "EST",
  "GMT",
  "HDT",
  "HKT",
  "HST",
  "IDT",
  "IST",
  "JST",
  "KST",
  "MDT",
  "MSK",
  "MST",
  "NST",
  "NZDT",
  "PDT",
  "PKT",
  "PST",
  "SAST",
  "SST",
  "UTC",
  "WAT",
  "WET",
  "WIB",
  "WIT",
  "WITA",
/* }ntz::const_char_ntz_abbrev -- generated code do not touch */
};
#else

/* {ntz::define_empty_abbrev -- generated code do not touch */
#define p00 
#define p01 
#define m01 
#define p02 
#define m02 
#define p03 
#define m03 
#define p0330 
#define p04 
#define m04 
#define p0430 
#define p05 
#define m05 
#define p0530 
#define p0545 
#define p06 
#define m06 
#define p0630 
#define p07 
#define m07 
#define p08 
#define m08 
#define p0845 
#define p09 
#define m09 
#define m0930 
#define p10 
#define m10 
#define p11 
#define m11 
#define p12 
#define m12 
#define p13 
#define p1345 
#define p14 
#define ACST 
#define ADT 
#define AEDT 
#define AEST 
#define AKDT 
#define AST 
#define AWST 
#define BST 
#define CAT 
#define CDT 
#define CET 
#define ChST 
#define CST 
#define EAT 
#define EDT 
#define EET 
#define EST 
#define GMT 
#define HDT 
#define HKT 
#define HST 
#define IDT 
#define IST 
#define JST 
#define KST 
#define MDT 
#define MSK 
#define MST 
#define NST 
#define NZDT 
#define PDT 
#define PKT 
#define PST 
#define SAST 
#define SST 
#define UTC 
#define WAT 
#define WET 
#define WIB 
#define WIT 
#define WITA 
/* }ntz::define_empty_abbrev -- generated code do not touch */

#endif

const struct ntz_iana ntz_db[] = {
/* {ntz::const_struct_ntz_iana_ntz_db -- generated code do not touch */
  {0x0045, UTC NTZ_OFFSET_p0, RULE_0_________},	// Zulu
  {0x0072, p08 NTZ_OFFSET_p480, RULE_0_________},	// Antarctica/Casey
  {0x0124, MST NTZ_OFFSET_m420, RULE_0_________},	// America/Dawson_Creek
  {0x0161, p01 NTZ_OFFSET_p60, RULE_0_________},	// Africa/El_Aaiun
  {0x0171, BST NTZ_OFFSET_p0, RULE_2cJDa1CDa1},	// GB-Eire
  {0x01af, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Vienna
  {0x01f9, m05 NTZ_OFFSET_m300, RULE_0_________},	// Brazil/Acre
  {0x025c, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Argentina/Cordoba
  {0x0261, CST NTZ_OFFSET_m360, RULE_0_________},	// America/El_Salvador
  {0x02ad, MDT NTZ_OFFSET_m420, RULE_2bKD11CD21},	// America/Edmonton
  {0x02be, GMT NTZ_OFFSET_p0, RULE_0_________},	// Greenwich
  {0x02bf, CDT NTZ_OFFSET_m360, RULE_2bKD11CD21},	// America/Indiana/Tell_City
  {0x02e6, EST NTZ_OFFSET_m300, RULE_0_________},	// America/Panama
  {0x02f4, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Kentucky/Louisville
  {0x03b7, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Poland
  {0x03e8, PDT NTZ_OFFSET_m480, RULE_2bKD11CD21},	// America/Ensenada
  {0x05b1, p10 NTZ_OFFSET_p600, RULE_0_________},	// Pacific/Truk
  {0x06bc, IST NTZ_OFFSET_p330, RULE_0_________},	// Asia/Calcutta
  {0x0784, AKDT NTZ_OFFSET_m540, RULE_2bKD11CD21},	// America/Sitka
  {0x089c, WITA NTZ_OFFSET_p480, RULE_0_________},	// Asia/Ujung_Pandang
  {0x08ce, ACST NTZ_OFFSET_p570, RULE_2cDD11JD11},	// Australia/Yancowinna
  {0x08de, CST NTZ_OFFSET_m360, RULE_0_________},	// Canada/Saskatchewan
  {0x08e3, p05 NTZ_OFFSET_p300, RULE_0_________},	// Asia/Atyrau
  {0x093e, p07 NTZ_OFFSET_p420, RULE_0_________},	// Antarctica/Davis
  {0x0a1e, p02 NTZ_OFFSET_p0, RULE_4cJDa1CDa1},	// Antarctica/Troll
  {0x0a23, CST NTZ_OFFSET_m360, RULE_0_________},	// America/Chihuahua
  {0x0a31, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Anguilla
  {0x0a91, m02 NTZ_OFFSET_m120, RULE_2bJDa1CDa7},	// America/Scoresbysund
  {0x0aba, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Monaco
  {0x0aec, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Sao_Paulo
  {0x0b00, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Argentina/Ushuaia
  {0x0b7b, p08 NTZ_OFFSET_p480, RULE_0_________},	// Singapore
  {0x0ba0, m04 NTZ_OFFSET_m240, RULE_2bDD11ID11},	// Chile/Continental
  {0x0bf4, m05 NTZ_OFFSET_m300, RULE_0_________},	// Etc/GMT+5
  {0x0c88, m04 NTZ_OFFSET_m240, RULE_2bDD11ID11},	// America/Santiago
  {0x0c9a, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Santo_Domingo
  {0x0cf6, AEST NTZ_OFFSET_p600, RULE_2cDD11JD11},	// Australia/Victoria
  {0x0d06, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Zurich
  {0x0d0b, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Andorra
  {0x0d17, SAST NTZ_OFFSET_p120, RULE_0_________},	// Africa/Mbabane
  {0x0dc7, m04 NTZ_OFFSET_m240, RULE_0_________},	// America/La_Paz
  {0x0f0b, WET NTZ_OFFSET_p0, RULE_2cJDa1CDa1},	// Europe/Lisbon
  {0x0f1e, GMT NTZ_OFFSET_p0, RULE_0_________},	// Africa/Abidjan
  {0x0f4c, BST NTZ_OFFSET_p0, RULE_2cJDa1CDa1},	// Europe/Belfast
  {0x0f79, AEST NTZ_OFFSET_p600, RULE_0_________},	// Australia/Lindeman
  {0x0f94, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Bahia
  {0x0fa8, IST NTZ_OFFSET_p60, RULE_bcCDa1JDa1},	// Eire
  {0x0ffc, NZDT NTZ_OFFSET_p720, RULE_2cDD11IDa1},	// Pacific/Auckland
  {0x10a8, KST NTZ_OFFSET_p540, RULE_0_________},	// Asia/Seoul
  {0x10df, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// Asia/Beirut
  {0x1184, EST NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Indiana/Winamac
  {0x1187, p0630 NTZ_OFFSET_p390, RULE_0_________},	// Asia/Yangon
  {0x11b9, UTC NTZ_OFFSET_p0, RULE_0_________},	// UCT
  {0x11d4, p05 NTZ_OFFSET_p300, RULE_0_________},	// Asia/Aqtobe
  {0x1201, MDT NTZ_OFFSET_m420, RULE_2bKD11CD21},	// Canada/Mountain
  {0x1352, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Toronto
  {0x140a, m11 NTZ_OFFSET_m660, RULE_0_________},	// Pacific/Niue
  {0x1464, AEST NTZ_OFFSET_p600, RULE_2cDD11JD11},	// Australia/NSW
  {0x1470, p11 NTZ_OFFSET_p660, RULE_0_________},	// Pacific/Efate
  {0x14df, NST NTZ_OFFSET_m210, RULE_2bKD11CD21},	// America/St_Johns
  {0x15ae, EAT NTZ_OFFSET_p180, RULE_0_________},	// Indian/Comoro
  {0x1619, EET NTZ_OFFSET_p120, RULE_2cJDa7CDa7},	// Asia/Gaza
  {0x16aa, p04 NTZ_OFFSET_p240, RULE_0_________},	// Etc/GMT-4
  {0x17ce, GMT NTZ_OFFSET_p0, RULE_0_________},	// Africa/Bamako
  {0x17ea, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Cayenne
  {0x18cc, p12 NTZ_OFFSET_p720, RULE_0_________},	// Pacific/Tarawa
  {0x1948, m03 NTZ_OFFSET_m180, RULE_0_________},	// Etc/GMT+3
  {0x197b, p06 NTZ_OFFSET_p360, RULE_0_________},	// Asia/Kashgar
  {0x1b92, WITA NTZ_OFFSET_p480, RULE_0_________},	// Asia/Makassar
  {0x1b9c, p09 NTZ_OFFSET_p540, RULE_0_________},	// Asia/Khandyga
  {0x1bce, m02 NTZ_OFFSET_m120, RULE_0_________},	// Atlantic/South_Georgia
  {0x1bd7, CAT NTZ_OFFSET_p120, RULE_0_________},	// Africa/Harare
  {0x1bec, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Gibraltar
  {0x1bef, m12 NTZ_OFFSET_m720, RULE_0_________},	// Etc/GMT+12
  {0x1c59, m08 NTZ_OFFSET_m480, RULE_0_________},	// Pacific/Pitcairn
  {0x1c8f, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Indiana/Petersburg
  {0x1ca5, GMT NTZ_OFFSET_p0, RULE_0_________},	// Etc/Greenwich
  {0x1cbf, p11 NTZ_OFFSET_p660, RULE_0_________},	// Pacific/Noumea
  {0x1e25, p11 NTZ_OFFSET_p660, RULE_0_________},	// Etc/GMT-11
  {0x202d, p12 NTZ_OFFSET_p720, RULE_0_________},	// Pacific/Kwajalein
  {0x205c, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Coyhaique
  {0x20f8, WIT NTZ_OFFSET_p540, RULE_0_________},	// Asia/Jayapura
  {0x211c, m05 NTZ_OFFSET_m300, RULE_0_________},	// America/Bogota
  {0x219c, p03 NTZ_OFFSET_p180, RULE_0_________},	// Asia/Aden
  {0x21e0, p12 NTZ_OFFSET_p720, RULE_0_________},	// Etc/GMT-12
  {0x21f4, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Kralendijk
  {0x224e, p0330 NTZ_OFFSET_p210, RULE_0_________},	// Iran
  {0x22f7, CDT NTZ_OFFSET_m360, RULE_2bKD11CD21},	// America/North_Dakota/New_Salem
  {0x234c, GMT NTZ_OFFSET_p0, RULE_0_________},	// GMT
  {0x240f, CET NTZ_OFFSET_p60, RULE_0_________},	// Africa/Algiers
  {0x241c, p14 NTZ_OFFSET_p840, RULE_0_________},	// Etc/GMT-14
  {0x249c, MST NTZ_OFFSET_m420, RULE_0_________},	// America/Vancouver
  {0x282b, p06 NTZ_OFFSET_p360, RULE_0_________},	// Asia/Thimbu
  {0x2837, CST NTZ_OFFSET_p480, RULE_0_________},	// PRC
  {0x28fb, p11 NTZ_OFFSET_p660, RULE_0_________},	// Asia/Srednekolymsk
  {0x29c3, p04 NTZ_OFFSET_p240, RULE_0_________},	// Europe/Saratov
  {0x29f6, JST NTZ_OFFSET_p540, RULE_0_________},	// Japan
  {0x2abf, m10 NTZ_OFFSET_m600, RULE_0_________},	// Pacific/Tahiti
  {0x2b13, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// US/East-Indiana
  {0x2b64, MST NTZ_OFFSET_m420, RULE_0_________},	// America/Hermosillo
  {0x2bfa, m06 NTZ_OFFSET_m360, RULE_0_________},	// Pacific/Galapagos
  {0x2cd0, ACST NTZ_OFFSET_p570, RULE_0_________},	// Australia/North
  {0x2d20, IDT NTZ_OFFSET_p120, RULE_2cJDa1CDa6},	// Asia/Jerusalem
  {0x2d87, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Louisville
  {0x2fe1, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Antigua
  {0x301f, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Cordoba
  {0x303b, EST NTZ_OFFSET_m300, RULE_0_________},	// America/Cayman
  {0x3049, EAT NTZ_OFFSET_p180, RULE_0_________},	// Africa/Kampala
  {0x3068, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Mendoza
  {0x30a1, PKT NTZ_OFFSET_p300, RULE_0_________},	// Asia/Karachi
  {0x30c8, GMT NTZ_OFFSET_p0, RULE_0_________},	// Africa/Lome
  {0x3135, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Detroit
  {0x32a5, GMT NTZ_OFFSET_p0, RULE_0_________},	// Africa/Accra
  {0x32c6, p05 NTZ_OFFSET_p300, RULE_0_________},	// Antarctica/Vostok
  {0x3361, m03 NTZ_OFFSET_m180, RULE_0_________},	// Antarctica/Rothera
  {0x3372, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Indiana/Vincennes
  {0x33a3, HST NTZ_OFFSET_m600, RULE_0_________},	// HST
  {0x33b6, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// US/Eastern
  {0x3402, EAT NTZ_OFFSET_p180, RULE_0_________},	// Africa/Asmera
  {0x34ba, p0845 NTZ_OFFSET_p525, RULE_0_________},	// Australia/Eucla
  {0x34eb, CST NTZ_OFFSET_m360, RULE_0_________},	// America/Monterrey
  {0x35f8, ACST NTZ_OFFSET_p570, RULE_2cDD11JD11},	// Australia/Adelaide
  {0x3606, p03 NTZ_OFFSET_p180, RULE_0_________},	// Asia/Kuwait
  {0x3663, EAT NTZ_OFFSET_p180, RULE_0_________},	// Africa/Djibouti
  {0x36a3, p0545 NTZ_OFFSET_p345, RULE_0_________},	// Asia/Kathmandu
  {0x36e8, WET NTZ_OFFSET_p0, RULE_2cJDa1CDa1},	// WET
  {0x379e, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Marigot
  {0x37ab, AST NTZ_OFFSET_m240, RULE_0_________},	// America/St_Kitts
  {0x3812, p10 NTZ_OFFSET_p600, RULE_0_________},	// Antarctica/DumontDUrville
  {0x392c, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// Asia/Famagusta
  {0x39eb, HDT NTZ_OFFSET_m600, RULE_2bKD11CD21},	// America/Atka
  {0x3a3f, p08 NTZ_OFFSET_p480, RULE_0_________},	// Asia/Kuching
  {0x3a86, NZDT NTZ_OFFSET_p720, RULE_2cDD11IDa1},	// Antarctica/McMurdo
  {0x3aba, SST NTZ_OFFSET_m660, RULE_0_________},	// Pacific/Samoa
  {0x3af6, p07 NTZ_OFFSET_p420, RULE_0_________},	// Asia/Barnaul
  {0x3b64, p12 NTZ_OFFSET_p720, RULE_0_________},	// Pacific/Majuro
  {0x3b82, GMT NTZ_OFFSET_p0, RULE_0_________},	// Africa/Monrovia
  {0x3c27, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Argentina/Jujuy
  {0x3ce5, WAT NTZ_OFFSET_p60, RULE_0_________},	// Africa/Porto-Novo
  {0x3d51, PDT NTZ_OFFSET_m480, RULE_2bKD11CD21},	// America/Tijuana
  {0x3d83, EAT NTZ_OFFSET_p180, RULE_0_________},	// Africa/Nairobi
  {0x3dec, p11 NTZ_OFFSET_p660, RULE_0_________},	// Pacific/Pohnpei
  {0x3df3, GMT NTZ_OFFSET_p0, RULE_0_________},	// America/Danmarkshavn
  {0x3e0a, p14 NTZ_OFFSET_p840, RULE_0_________},	// Pacific/Kiritimati
  {0x3e83, ADT NTZ_OFFSET_m240, RULE_2bKD11CD21},	// America/Glace_Bay
  {0x3ff3, AWST NTZ_OFFSET_p480, RULE_0_________},	// Australia/West
  {0x4011, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Thunder_Bay
  {0x4063, CAT NTZ_OFFSET_p120, RULE_0_________},	// Africa/Kigali
  {0x408b, p07 NTZ_OFFSET_p420, RULE_0_________},	// Etc/GMT-7
  {0x4180, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Blanc-Sablon
  {0x4265, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// Canada/Eastern
  {0x4278, p05 NTZ_OFFSET_p300, RULE_0_________},	// Asia/Qyzylorda
  {0x429e, m07 NTZ_OFFSET_m420, RULE_0_________},	// Etc/GMT+7
  {0x4315, MSK NTZ_OFFSET_p180, RULE_0_________},	// Europe/Simferopol
  {0x4351, CST NTZ_OFFSET_m360, RULE_0_________},	// America/Swift_Current
  {0x43c7, m02 NTZ_OFFSET_m120, RULE_0_________},	// Etc/GMT+2
  {0x43d6, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Budapest
  {0x43da, BST NTZ_OFFSET_p0, RULE_2cJDa1CDa1},	// Europe/Isle_of_Man
  {0x43f9, p07 NTZ_OFFSET_p420, RULE_0_________},	// Asia/Saigon
  {0x448e, GMT NTZ_OFFSET_p0, RULE_0_________},	// GMT0
  {0x44a0, AKDT NTZ_OFFSET_m540, RULE_2bKD11CD21},	// America/Yakutat
  {0x4520, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// MET
  {0x4570, CAT NTZ_OFFSET_p120, RULE_0_________},	// Africa/Juba
  {0x4586, m04 NTZ_OFFSET_m240, RULE_0_________},	// Brazil/West
  {0x4623, p10 NTZ_OFFSET_p600, RULE_0_________},	// Pacific/Chuuk
  {0x46db, p13 NTZ_OFFSET_p780, RULE_0_________},	// Pacific/Fakaofo
  {0x471e, m0930 NTZ_OFFSET_m570, RULE_0_________},	// Pacific/Marquesas
  {0x4860, NZDT NTZ_OFFSET_p720, RULE_2cDD11IDa1},	// Antarctica/South_Pole
  {0x48e7, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Warsaw
  {0x4958, EET NTZ_OFFSET_p120, RULE_0_________},	// Europe/Kaliningrad
  {0x499b, p06 NTZ_OFFSET_p360, RULE_0_________},	// Asia/Urumqi
  {0x49d4, EET NTZ_OFFSET_p120, RULE_0_________},	// Libya
  {0x4aa4, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Malta
  {0x4b13, CST NTZ_OFFSET_m300, RULE_2bKD11CD21},	// Cuba
  {0x4b34, AEDT NTZ_OFFSET_p600, RULE_2cDD11JD11},	// Australia/Tasmania
  {0x4b5f, p01 NTZ_OFFSET_p60, RULE_0_________},	// Etc/GMT-1
  {0x4bc4, p08 NTZ_OFFSET_p480, RULE_0_________},	// Asia/Singapore
  {0x4be5, p10 NTZ_OFFSET_p600, RULE_0_________},	// Pacific/Yap
  {0x4ca9, WIB NTZ_OFFSET_p420, RULE_0_________},	// Asia/Pontianak
  {0x4cb7, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// Europe/Bucharest
  {0x4d12, EAT NTZ_OFFSET_p180, RULE_0_________},	// Africa/Mogadishu
  {0x4d1b, m11 NTZ_OFFSET_m660, RULE_0_________},	// Etc/GMT+11
  {0x4d1f, m04 NTZ_OFFSET_m240, RULE_0_________},	// America/Porto_Velho
  {0x4da2, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Tirane
  {0x4e43, p07 NTZ_OFFSET_p420, RULE_0_________},	// Asia/Hovd
  {0x4e53, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Barbados
  {0x4e80, p04 NTZ_OFFSET_p240, RULE_0_________},	// Asia/Muscat
  {0x4f7c, m04 NTZ_OFFSET_m240, RULE_0_________},	// Etc/GMT+4
  {0x4fcd, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// Europe/Kiev
  {0x4fd5, p0630 NTZ_OFFSET_p390, RULE_0_________},	// Indian/Cocos
  {0x517d, p11 NTZ_OFFSET_p660, RULE_0_________},	// Pacific/Bougainville
  {0x5190, WET NTZ_OFFSET_p0, RULE_2cJDa1CDa1},	// Portugal
  {0x5234, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Argentina/Tucuman
  {0x5278, ACST NTZ_OFFSET_p570, RULE_2cDD11JD11},	// Australia/Broken_Hill
  {0x53d3, MST NTZ_OFFSET_m420, RULE_0_________},	// America/Fort_Nelson
  {0x53e6, p03 NTZ_OFFSET_p180, RULE_0_________},	// Turkey
  {0x5473, CST NTZ_OFFSET_m360, RULE_0_________},	// America/Regina
  {0x54d0, GMT NTZ_OFFSET_p0, RULE_0_________},	// Etc/GMT-0
  {0x5609, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// US/Michigan
  {0x56ab, m02 NTZ_OFFSET_m180, RULE_2bKD11CD21},	// America/Miquelon
  {0x5746, UTC NTZ_OFFSET_p0, RULE_0_________},	// Etc/UTC
  {0x576a, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Rome
  {0x57d1, CDT NTZ_OFFSET_m360, RULE_2bKD11CD21},	// America/Matamoros
  {0x57ea, p11 NTZ_OFFSET_p660, RULE_0_________},	// Pacific/Guadalcanal
  {0x57ec, m10 NTZ_OFFSET_m600, RULE_0_________},	// Etc/GMT+10
  {0x588f, p07 NTZ_OFFSET_p420, RULE_0_________},	// Asia/Bangkok
  {0x5895, PDT NTZ_OFFSET_m480, RULE_2bKD11CD21},	// US/Pacific
  {0x58c3, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Argentina/Buenos_Aires
  {0x5921, CDT NTZ_OFFSET_m360, RULE_2bKD11CD21},	// America/Winnipeg
  {0x5a3c, CST NTZ_OFFSET_p480, RULE_0_________},	// Asia/Shanghai
  {0x5a44, MST NTZ_OFFSET_m420, RULE_0_________},	// MST
  {0x5c36, AEST NTZ_OFFSET_p600, RULE_2cDD11JD11},	// Australia/ACT
  {0x5c8e, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// Europe/Tallinn
  {0x5d19, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// Europe/Helsinki
  {0x5d72, p06 NTZ_OFFSET_p360, RULE_0_________},	// Indian/Chagos
  {0x5dad, AEST NTZ_OFFSET_p600, RULE_0_________},	// Australia/Queensland
  {0x5e2d, GMT NTZ_OFFSET_p0, RULE_0_________},	// Etc/GMT+0
  {0x5e4a, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/New_York
  {0x5e68, MDT NTZ_OFFSET_m420, RULE_2bKD11CD21},	// Navajo
  {0x5e7f, CDT NTZ_OFFSET_m360, RULE_2bKD11CD21},	// US/Indiana-Starke
  {0x5eef, p07 NTZ_OFFSET_p420, RULE_0_________},	// Asia/Phnom_Penh
  {0x5fdf, m04 NTZ_OFFSET_m240, RULE_0_________},	// America/Cuiaba
  {0x601c, p03 NTZ_OFFSET_p180, RULE_0_________},	// Asia/Bahrain
  {0x6028, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Araguaina
  {0x6031, UTC NTZ_OFFSET_p0, RULE_0_________},	// Universal
  {0x6075, AEST NTZ_OFFSET_p600, RULE_0_________},	// Australia/Brisbane
  {0x60df, GMT NTZ_OFFSET_p0, RULE_0_________},	// Africa/Dakar
  {0x61de, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// Europe/Riga
  {0x61f2, AEDT NTZ_OFFSET_p600, RULE_2cDD11JD11},	// Australia/Currie
  {0x6242, GMT NTZ_OFFSET_p0, RULE_0_________},	// Africa/Banjul
  {0x626a, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Dominica
  {0x637f, GMT NTZ_OFFSET_p0, RULE_0_________},	// Etc/GMT
  {0x63cb, CST NTZ_OFFSET_m360, RULE_0_________},	// America/Merida
  {0x6466, CET NTZ_OFFSET_p60, RULE_0_________},	// Africa/Tunis
  {0x646e, WET NTZ_OFFSET_p0, RULE_2cJDa1CDa1},	// Atlantic/Canary
  {0x648b, AEST NTZ_OFFSET_p600, RULE_2cDD11JD11},	// Australia/Melbourne
  {0x648d, ADT NTZ_OFFSET_m240, RULE_2bKD11CD21},	// Canada/Atlantic
  {0x6523, p12 NTZ_OFFSET_p720, RULE_0_________},	// Pacific/Wake
  {0x6543, CDT NTZ_OFFSET_m360, RULE_2bKD11CD21},	// Canada/Central
  {0x65c2, ChST NTZ_OFFSET_p600, RULE_0_________},	// Pacific/Guam
  {0x65fa, m01 NTZ_OFFSET_m60, RULE_0_________},	// Atlantic/Cape_Verde
  {0x66ac, AST NTZ_OFFSET_m240, RULE_0_________},	// America/St_Barthelemy
  {0x66fa, EAT NTZ_OFFSET_p180, RULE_0_________},	// Africa/Dar_es_Salaam
  {0x67c5, m04 NTZ_OFFSET_m240, RULE_0_________},	// America/Campo_Grande
  {0x67cc, p06 NTZ_OFFSET_p360, RULE_0_________},	// Asia/Bishkek
  {0x69c9, CAT NTZ_OFFSET_p120, RULE_0_________},	// Africa/Lusaka
  {0x6a0c, m05 NTZ_OFFSET_m300, RULE_0_________},	// America/Guayaquil
  {0x6bbc, CDT NTZ_OFFSET_m360, RULE_2bKD11CD21},	// America/Rainy_River
  {0x6c0d, p11 NTZ_OFFSET_p660, RULE_0_________},	// Pacific/Kosrae
  {0x6c36, m04 NTZ_OFFSET_m240, RULE_0_________},	// America/Manaus
  {0x6c41, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// CET
  {0x6c69, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Fortaleza
  {0x6cae, p03 NTZ_OFFSET_p180, RULE_0_________},	// Asia/Damascus
  {0x6d60, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Recife
  {0x6e02, MST NTZ_OFFSET_m420, RULE_0_________},	// America/Mazatlan
  {0x6f47, GMT NTZ_OFFSET_p0, RULE_0_________},	// Atlantic/St_Helena
  {0x6fbe, CST NTZ_OFFSET_p480, RULE_0_________},	// Asia/Harbin
  {0x70aa, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// Europe/Tiraspol
  {0x70b5, m06 NTZ_OFFSET_m360, RULE_2cDD17ID17},	// Pacific/Easter
  {0x70c2, MST NTZ_OFFSET_m420, RULE_0_________},	// America/Whitehorse
  {0x71a0, p05 NTZ_OFFSET_p300, RULE_0_________},	// Indian/Kerguelen
  {0x71b4, MST NTZ_OFFSET_m420, RULE_0_________},	// America/Phoenix
  {0x7256, CST NTZ_OFFSET_p480, RULE_0_________},	// Asia/Macao
  {0x7259, m09 NTZ_OFFSET_m540, RULE_0_________},	// Etc/GMT+9
  {0x725a, m05 NTZ_OFFSET_m300, RULE_0_________},	// America/Eirunepe
  {0x7275, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Argentina/Rio_Gallegos
  {0x72c4, p09 NTZ_OFFSET_p540, RULE_0_________},	// Asia/Chita
  {0x72ee, AKDT NTZ_OFFSET_m540, RULE_2bKD11CD21},	// US/Alaska
  {0x72f4, p06 NTZ_OFFSET_p360, RULE_0_________},	// Asia/Thimphu
  {0x73fa, CST NTZ_OFFSET_m360, RULE_0_________},	// America/Tegucigalpa
  {0x74fc, GMT NTZ_OFFSET_p0, RULE_0_________},	// Africa/Freetown
  {0x7591, ADT NTZ_OFFSET_m240, RULE_2cKD11CD21},	// Atlantic/Bermuda
  {0x75fa, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Kentucky/Monticello
  {0x762c, PST NTZ_OFFSET_p480, RULE_0_________},	// Asia/Manila
  {0x76a7, CAT NTZ_OFFSET_p120, RULE_0_________},	// Africa/Lubumbashi
  {0x76fa, JST NTZ_OFFSET_p540, RULE_0_________},	// Asia/Tokyo
  {0x775c, HST NTZ_OFFSET_m600, RULE_0_________},	// US/Hawaii
  {0x77ad, p05 NTZ_OFFSET_p300, RULE_0_________},	// Asia/Oral
  {0x7834, CST NTZ_OFFSET_m360, RULE_0_________},	// America/Bahia_Banderas
  {0x78f0, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Indianapolis
  {0x7909, m08 NTZ_OFFSET_m480, RULE_0_________},	// Etc/GMT+8
  {0x7a32, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Zagreb
  {0x7a4a, CST NTZ_OFFSET_p480, RULE_0_________},	// ROC
  {0x7a6b, ADT NTZ_OFFSET_m240, RULE_2bKD11CD21},	// America/Moncton
  {0x7a85, CST NTZ_OFFSET_m360, RULE_0_________},	// America/Guatemala
  {0x7ac4, MDT NTZ_OFFSET_m420, RULE_2bKD11CD21},	// America/Ciudad_Juarez
  {0x7aca, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Busingen
  {0x7acc, MST NTZ_OFFSET_m420, RULE_0_________},	// America/Creston
  {0x7af3, CST NTZ_OFFSET_p480, RULE_0_________},	// Asia/Macau
  {0x7b7d, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Lower_Princes
  {0x7b86, MDT NTZ_OFFSET_m420, RULE_2bKD11CD21},	// America/Inuvik
  {0x7be9, m05 NTZ_OFFSET_m300, RULE_0_________},	// America/Porto_Acre
  {0x7c42, CDT NTZ_OFFSET_m360, RULE_2bKD11CD21},	// America/Ojinaga
  {0x7c4c, p03 NTZ_OFFSET_p180, RULE_0_________},	// Etc/GMT-3
  {0x7c5e, CAT NTZ_OFFSET_p120, RULE_0_________},	// Africa/Blantyre
  {0x7e8a, MST NTZ_OFFSET_m420, RULE_0_________},	// Canada/Yukon
  {0x7e94, CAT NTZ_OFFSET_p120, RULE_0_________},	// Africa/Bujumbura
  {0x7e95, SST NTZ_OFFSET_m660, RULE_0_________},	// Pacific/Midway
  {0x7eac, p05 NTZ_OFFSET_p300, RULE_0_________},	// Asia/Ashgabat
  {0x7f0a, EAT NTZ_OFFSET_p180, RULE_0_________},	// Indian/Antananarivo
  {0x7f2d, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Guadeloupe
  {0x7f4c, CDT NTZ_OFFSET_m360, RULE_2bKD11CD21},	// America/Rankin_Inlet
  {0x7f92, p08 NTZ_OFFSET_p480, RULE_0_________},	// Etc/GMT-8
  {0x7fd5, p12 NTZ_OFFSET_p720, RULE_0_________},	// Pacific/Fiji
  {0x7ff7, p12 NTZ_OFFSET_p720, RULE_0_________},	// Pacific/Nauru
  {0x801f, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Africa/Ceuta
  {0x8023, p11 NTZ_OFFSET_p660, RULE_0_________},	// Asia/Magadan
  {0x805f, WAT NTZ_OFFSET_p60, RULE_0_________},	// Africa/Luanda
  {0x80e9, WAT NTZ_OFFSET_p60, RULE_0_________},	// Africa/Libreville
  {0x80ec, PDT NTZ_OFFSET_m480, RULE_2bKD11CD21},	// PST8PDT
  {0x8136, KST NTZ_OFFSET_p540, RULE_0_________},	// ROK
  {0x8146, p11 NTZ_OFFSET_p630, RULE_1cDD11JD11},	// Australia/LHI
  {0x8193, AEST NTZ_OFFSET_p600, RULE_2cDD11JD11},	// Australia/Canberra
  {0x8232, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Vatican
  {0x8240, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// EET
  {0x8271, p05 NTZ_OFFSET_p300, RULE_0_________},	// Asia/Ashkhabad
  {0x8286, MST NTZ_OFFSET_m420, RULE_0_________},	// Mexico/BajaSur
  {0x83c0, EST NTZ_OFFSET_m300, RULE_0_________},	// America/Cancun
  {0x83d1, m01 NTZ_OFFSET_m60, RULE_0_________},	// Etc/GMT+1
  {0x84df, p05 NTZ_OFFSET_p300, RULE_0_________},	// Indian/Maldives
  {0x84e4, MDT NTZ_OFFSET_m420, RULE_2bKD11CD21},	// MST7MDT
  {0x8530, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Maceio
  {0x8605, p03 NTZ_OFFSET_p180, RULE_0_________},	// Antarctica/Syowa
  {0x860e, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Argentina/La_Rioja
  {0x8752, m03 NTZ_OFFSET_m180, RULE_0_________},	// Atlantic/Stanley
  {0x876f, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Podgorica
  {0x8851, EAT NTZ_OFFSET_p180, RULE_0_________},	// Africa/Addis_Ababa
  {0x88c3, BST NTZ_OFFSET_p0, RULE_2cJDa1CDa1},	// Europe/Jersey
  {0x88df, p04 NTZ_OFFSET_p240, RULE_0_________},	// Asia/Dubai
  {0x8ad0, m04 NTZ_OFFSET_m240, RULE_0_________},	// America/Guyana
  {0x8b42, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Indiana/Indianapolis
  {0x8b85, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Belgrade
  {0x8b8b, UTC NTZ_OFFSET_p0, RULE_0_________},	// Etc/UCT
  {0x8baf, m04 NTZ_OFFSET_m240, RULE_0_________},	// America/Caracas
  {0x8c3a, SAST NTZ_OFFSET_p120, RULE_0_________},	// Africa/Johannesburg
  {0x8c43, CST NTZ_OFFSET_p480, RULE_0_________},	// Asia/Chungking
  {0x8c6f, ChST NTZ_OFFSET_p600, RULE_0_________},	// Pacific/Saipan
  {0x8c81, MDT NTZ_OFFSET_m420, RULE_2bKD11CD21},	// America/Boise
  {0x8c83, MDT NTZ_OFFSET_m420, RULE_2bKD11CD21},	// America/Cambridge_Bay
  {0x8d75, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Martinique
  {0x8e1c, AKDT NTZ_OFFSET_m540, RULE_2bKD11CD21},	// America/Juneau
  {0x8e79, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Arctic/Longyearbyen
  {0x8fad, p05 NTZ_OFFSET_p300, RULE_0_________},	// Asia/Qostanay
  {0x909d, AST NTZ_OFFSET_m240, RULE_0_________},	// America/St_Vincent
  {0x90b8, p05 NTZ_OFFSET_p300, RULE_0_________},	// Asia/Aqtau
  {0x90e5, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Montevideo
  {0x9117, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// Europe/Athens
  {0x91f9, p02 NTZ_OFFSET_p120, RULE_0_________},	// Etc/GMT-2
  {0x9274, PDT NTZ_OFFSET_m480, RULE_2bKD11CD21},	// America/Santa_Isabel
  {0x928b, AWST NTZ_OFFSET_p480, RULE_0_________},	// Australia/Perth
  {0x92ee, GMT NTZ_OFFSET_p0, RULE_0_________},	// Iceland
  {0x9436, p08 NTZ_OFFSET_p480, RULE_0_________},	// Asia/Brunei
  {0x951e, p0430 NTZ_OFFSET_p270, RULE_0_________},	// Asia/Kabul
  {0x9592, PDT NTZ_OFFSET_m480, RULE_2bKD11CD21},	// America/Los_Angeles
  {0x959a, p07 NTZ_OFFSET_p420, RULE_0_________},	// Asia/Novokuznetsk
  {0x95a2, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Vaduz
  {0x95b2, AKDT NTZ_OFFSET_m540, RULE_2bKD11CD21},	// America/Nome
  {0x96b6, HDT NTZ_OFFSET_m600, RULE_2bKD11CD21},	// America/Adak
  {0x96d8, p01 NTZ_OFFSET_p60, RULE_0_________},	// Africa/Casablanca
  {0x9710, CST NTZ_OFFSET_p480, RULE_0_________},	// Asia/Taipei
  {0x976d, AEDT NTZ_OFFSET_p600, RULE_2cDD11JD11},	// Australia/Hobart
  {0x9778, GMT NTZ_OFFSET_p0, RULE_0_________},	// GMT-0
  {0x97df, IDT NTZ_OFFSET_p120, RULE_2cJDa1CDa6},	// Israel
  {0x9886, p0530 NTZ_OFFSET_p330, RULE_0_________},	// Asia/Colombo
  {0x9957, m02 NTZ_OFFSET_m120, RULE_0_________},	// Brazil/DeNoronha
  {0x9984, p04 NTZ_OFFSET_p240, RULE_0_________},	// Asia/Yerevan
  {0x9a0c, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Argentina/Mendoza
  {0x9abf, EET NTZ_OFFSET_p120, RULE_2cJDa6DDa6},	// Egypt
  {0x9b06, p10 NTZ_OFFSET_p600, RULE_0_________},	// Etc/GMT-10
  {0x9b17, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Catamarca
  {0x9b77, GMT NTZ_OFFSET_p0, RULE_0_________},	// Africa/Bissau
  {0x9bb7, p05 NTZ_OFFSET_p300, RULE_0_________},	// Asia/Samarkand
  {0x9c60, CAT NTZ_OFFSET_p120, RULE_0_________},	// Africa/Khartoum
  {0x9c81, m03 NTZ_OFFSET_m180, RULE_0_________},	// Antarctica/Palmer
  {0x9c8e, p08 NTZ_OFFSET_p480, RULE_0_________},	// Asia/Kuala_Lumpur
  {0x9d14, BST NTZ_OFFSET_p0, RULE_2cJDa1CDa1},	// Europe/London
  {0x9d4d, CST NTZ_OFFSET_m360, RULE_0_________},	// America/Costa_Rica
  {0x9d84, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Grenada
  {0x9e14, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Atlantic/Jan_Mayen
  {0x9e3e, MDT NTZ_OFFSET_m420, RULE_2bKD11CD21},	// America/Yellowknife
  {0x9eb3, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Nassau
  {0x9f42, WET NTZ_OFFSET_p0, RULE_2cJDa1CDa1},	// Atlantic/Madeira
  {0x9fd0, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Amsterdam
  {0xa000, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Curacao
  {0xa021, p09 NTZ_OFFSET_p540, RULE_0_________},	// Pacific/Palau
  {0xa0f7, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Puerto_Rico
  {0xa1cd, p03 NTZ_OFFSET_p180, RULE_0_________},	// Asia/Riyadh
  {0xa1e4, HST NTZ_OFFSET_m600, RULE_0_________},	// Pacific/Honolulu
  {0xa33d, CST NTZ_OFFSET_m360, RULE_2bKD11CD21},	// America/Resolute
  {0xa362, p08 NTZ_OFFSET_p480, RULE_0_________},	// Asia/Ulaanbaatar
  {0xa44f, BST NTZ_OFFSET_p0, RULE_2cJDa1CDa1},	// GB
  {0xa5b8, ACST NTZ_OFFSET_p570, RULE_2cDD11JD11},	// Australia/South
  {0xa5c3, p09 NTZ_OFFSET_p540, RULE_0_________},	// Asia/Dili
  {0xa655, CDT NTZ_OFFSET_m360, RULE_2bKD11CD21},	// America/North_Dakota/Beulah
  {0xa711, m02 NTZ_OFFSET_m120, RULE_0_________},	// America/Noronha
  {0xa850, WET NTZ_OFFSET_p0, RULE_2cJDa1CDa1},	// Atlantic/Faroe
  {0xa8c7, p07 NTZ_OFFSET_p420, RULE_0_________},	// Asia/Krasnoyarsk
  {0xa94c, WAT NTZ_OFFSET_p60, RULE_0_________},	// Africa/Bangui
  {0xa958, HST NTZ_OFFSET_m600, RULE_0_________},	// Pacific/Johnston
  {0xa998, p0545 NTZ_OFFSET_p345, RULE_0_________},	// Asia/Katmandu
  {0xa9bf, IDT NTZ_OFFSET_p120, RULE_2cJDa1CDa6},	// Asia/Tel_Aviv
  {0xa9df, p12 NTZ_OFFSET_p660, RULE_2cDD11JD11},	// Pacific/Norfolk
  {0xaa2a, EET NTZ_OFFSET_p120, RULE_2cJDa7CDa7},	// Asia/Hebron
  {0xaa3e, MSK NTZ_OFFSET_p180, RULE_0_________},	// W-SU
  {0xaae5, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Brussels
  {0xab2d, CDT NTZ_OFFSET_m360, RULE_2bKD11CD21},	// America/North_Dakota/Center
  {0xabbc, GMT NTZ_OFFSET_p0, RULE_0_________},	// GMT+0
  {0xabbe, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Aruba
  {0xacca, p03 NTZ_OFFSET_p180, RULE_0_________},	// Asia/Istanbul
  {0xad90, p06 NTZ_OFFSET_p360, RULE_0_________},	// Asia/Dacca
  {0xae32, CST NTZ_OFFSET_m360, RULE_0_________},	// America/Managua
  {0xae9e, UTC NTZ_OFFSET_p0, RULE_0_________},	// Etc/Zulu
  {0xaeed, m05 NTZ_OFFSET_m300, RULE_0_________},	// America/Lima
  {0xaf34, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Paramaribo
  {0xafa3, GMT NTZ_OFFSET_p0, RULE_0_________},	// Etc/GMT0
  {0xaff4, WAT NTZ_OFFSET_p60, RULE_0_________},	// Africa/Kinshasa
  {0xb0ba, m06 NTZ_OFFSET_m360, RULE_0_________},	// Etc/GMT+6
  {0xb0ee, MST NTZ_OFFSET_m420, RULE_0_________},	// Canada/Pacific
  {0xb15a, SST NTZ_OFFSET_m660, RULE_0_________},	// US/Samoa
  {0xb286, p11 NTZ_OFFSET_p630, RULE_1cDD11JD11},	// Australia/Lord_Howe
  {0xb294, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Punta_Arenas
  {0xb324, EST NTZ_OFFSET_m300, RULE_0_________},	// America/Jamaica
  {0xb32e, ACST NTZ_OFFSET_p570, RULE_0_________},	// Australia/Darwin
  {0xb354, MSK NTZ_OFFSET_p180, RULE_0_________},	// Europe/Kirov
  {0xb365, MSK NTZ_OFFSET_p180, RULE_0_________},	// Europe/Volgograd
  {0xb4d2, p05 NTZ_OFFSET_p300, RULE_0_________},	// Antarctica/Mawson
  {0xb510, p12 NTZ_OFFSET_p720, RULE_0_________},	// Kwajalein
  {0xb53e, WAT NTZ_OFFSET_p60, RULE_0_________},	// Africa/Niamey
  {0xb54a, AST NTZ_OFFSET_m240, RULE_2bKD11CD21},	// America/Goose_Bay
  {0xb5ba, p10 NTZ_OFFSET_p600, RULE_0_________},	// Asia/Vladivostok
  {0xb646, HKT NTZ_OFFSET_p480, RULE_0_________},	// Hongkong
  {0xb64a, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Madrid
  {0xb6d0, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/San_Marino
  {0xb6f8, EET NTZ_OFFSET_p120, RULE_2cJDa6DDa6},	// Africa/Cairo
  {0xb72b, CDT NTZ_OFFSET_m360, RULE_2bKD11CD21},	// CST6CDT
  {0xb72d, p04 NTZ_OFFSET_p240, RULE_0_________},	// Asia/Tbilisi
  {0xb77e, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Argentina/San_Luis
  {0xb795, m04 NTZ_OFFSET_m240, RULE_0_________},	// America/Boa_Vista
  {0xb7e0, CDT NTZ_OFFSET_m360, RULE_2bKD11CD21},	// America/Indiana/Knox
  {0xb87f, KST NTZ_OFFSET_p540, RULE_0_________},	// Asia/Pyongyang
  {0xb89b, m10 NTZ_OFFSET_m600, RULE_0_________},	// Pacific/Rarotonga
  {0xb9aa, p05 NTZ_OFFSET_p300, RULE_0_________},	// Asia/Yekaterinburg
  {0xba92, MSK NTZ_OFFSET_p180, RULE_0_________},	// Europe/Moscow
  {0xbb70, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Skopje
  {0xbb7b, p10 NTZ_OFFSET_p600, RULE_0_________},	// Asia/Ust-Nera
  {0xbbc1, EST NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Grand_Turk
  {0xbc26, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Paris
  {0xbc3f, AST NTZ_OFFSET_m240, RULE_0_________},	// America/St_Thomas
  {0xbd52, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// Europe/Mariehamn
  {0xbd91, AEST NTZ_OFFSET_p600, RULE_2cDD11JD11},	// Antarctica/Macquarie
  {0xbeae, p04 NTZ_OFFSET_p240, RULE_0_________},	// Europe/Samara
  {0xbf08, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Montreal
  {0xbf0d, CST NTZ_OFFSET_m360, RULE_0_________},	// America/Mexico_City
  {0xbf1c, p06 NTZ_OFFSET_p360, RULE_0_________},	// Etc/GMT-6
  {0xbf3d, WAT NTZ_OFFSET_p60, RULE_0_________},	// Africa/Lagos
  {0xbfbe, p07 NTZ_OFFSET_p420, RULE_0_________},	// Asia/Tomsk
  {0xbff5, p13 NTZ_OFFSET_p780, RULE_0_________},	// Pacific/Apia
  {0xbff6, CST NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Havana
  {0xc00a, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Argentina/Salta
  {0xc044, PDT NTZ_OFFSET_m480, RULE_2bKD11CD21},	// Mexico/BajaNorte
  {0xc112, p1345 NTZ_OFFSET_p765, RULE_2cDD11IDa1},	// NZ-CHAT
  {0xc11c, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Indiana/Vevay
  {0xc11f, p07 NTZ_OFFSET_p420, RULE_0_________},	// Asia/Ho_Chi_Minh
  {0xc13e, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Pangnirtung
  {0xc1f0, p04 NTZ_OFFSET_p240, RULE_0_________},	// Asia/Baku
  {0xc1f2, CDT NTZ_OFFSET_m360, RULE_2bKD11CD21},	// US/Central
  {0xc299, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// Europe/Sofia
  {0xc29f, p07 NTZ_OFFSET_p420, RULE_0_________},	// Indian/Christmas
  {0xc32e, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Bratislava
  {0xc350, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Copenhagen
  {0xc48d, CDT NTZ_OFFSET_m360, RULE_2bKD11CD21},	// America/Menominee
  {0xc4de, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Jujuy
  {0xc4f4, CST NTZ_OFFSET_p480, RULE_0_________},	// Asia/Chongqing
  {0xc673, p13 NTZ_OFFSET_p780, RULE_0_________},	// Pacific/Kanton
  {0xc8a1, ADT NTZ_OFFSET_m240, RULE_2bKD11CD21},	// America/Halifax
  {0xc8c6, p05 NTZ_OFFSET_p300, RULE_0_________},	// Etc/GMT-5
  {0xc8ee, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// Europe/Uzhgorod
  {0xc9bf, p12 NTZ_OFFSET_p720, RULE_0_________},	// Asia/Kamchatka
  {0xc9c3, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Argentina/San_Juan
  {0xc9da, p04 NTZ_OFFSET_p240, RULE_0_________},	// Europe/Astrakhan
  {0xcb46, p04 NTZ_OFFSET_p240, RULE_0_________},	// Indian/Mauritius
  {0xcc6b, AKDT NTZ_OFFSET_m540, RULE_2bKD11CD21},	// America/Metlakatla
  {0xcd36, p05 NTZ_OFFSET_p300, RULE_0_________},	// Asia/Dushanbe
  {0xcd78, ADT NTZ_OFFSET_m240, RULE_2bKD11CD21},	// America/Thule
  {0xce27, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// Europe/Chisinau
  {0xce6d, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Luxembourg
  {0xce87, IST NTZ_OFFSET_p330, RULE_0_________},	// Asia/Kolkata
  {0xce8f, CAT NTZ_OFFSET_p120, RULE_0_________},	// Africa/Windhoek
  {0xcf44, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Ljubljana
  {0xcf75, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Stockholm
  {0xd028, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Port_of_Spain
  {0xd043, MST NTZ_OFFSET_m420, RULE_0_________},	// US/Arizona
  {0xd097, p10 NTZ_OFFSET_p600, RULE_0_________},	// Pacific/Port_Moresby
  {0xd13b, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Nipigon
  {0xd192, EST NTZ_OFFSET_m300, RULE_0_________},	// America/Atikokan
  {0xd21b, p11 NTZ_OFFSET_p660, RULE_0_________},	// Asia/Sakhalin
  {0xd22b, AKDT NTZ_OFFSET_m540, RULE_2bKD11CD21},	// America/Anchorage
  {0xd3ff, p03 NTZ_OFFSET_p180, RULE_0_________},	// Europe/Minsk
  {0xd4d0, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Argentina/Catamarca
  {0xd4d8, UTC NTZ_OFFSET_p0, RULE_0_________},	// UTC
  {0xd53e, IST NTZ_OFFSET_p60, RULE_bcCDa1JDa1},	// Europe/Dublin
  {0xd569, p12 NTZ_OFFSET_p720, RULE_0_________},	// Pacific/Wallis
  {0xd57f, EET NTZ_OFFSET_p120, RULE_0_________},	// Africa/Tripoli
  {0xd63f, p13 NTZ_OFFSET_p780, RULE_0_________},	// Etc/GMT-13
  {0xd6a8, p08 NTZ_OFFSET_p480, RULE_0_________},	// Asia/Ulan_Bator
  {0xd6b7, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Montserrat
  {0xd6ba, p12 NTZ_OFFSET_p720, RULE_0_________},	// Asia/Anadyr
  {0xd6e9, EST NTZ_OFFSET_m300, RULE_0_________},	// EST
  {0xd74f, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Asuncion
  {0xd7da, m05 NTZ_OFFSET_m300, RULE_0_________},	// America/Rio_Branco
  {0xd7fd, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Belem
  {0xd8bd, CDT NTZ_OFFSET_m360, RULE_2bKD11CD21},	// America/Chicago
  {0xd918, MDT NTZ_OFFSET_m420, RULE_2bKD11CD21},	// America/Shiprock
  {0xd94b, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// Europe/Vilnius
  {0xd95f, NZDT NTZ_OFFSET_p720, RULE_2cDD11IDa1},	// NZ
  {0xd993, MST NTZ_OFFSET_m420, RULE_0_________},	// America/Dawson
  {0xd998, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Port-au-Prince
  {0xd9a1, WAT NTZ_OFFSET_p60, RULE_0_________},	// Africa/Malabo
  {0xd9f9, m01 NTZ_OFFSET_m120, RULE_2bJDa1CDa7},	// America/Nuuk
  {0xda19, p04 NTZ_OFFSET_p240, RULE_0_________},	// Europe/Ulyanovsk
  {0xda7b, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Buenos_Aires
  {0xdaa7, WAT NTZ_OFFSET_p60, RULE_0_________},	// Africa/Brazzaville
  {0xdb2e, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// Asia/Nicosia
  {0xdb30, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Iqaluit
  {0xdbd1, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Fort_Wayne
  {0xdc14, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Oslo
  {0xdc33, p12 NTZ_OFFSET_p720, RULE_0_________},	// Pacific/Funafuti
  {0xdced, p09 NTZ_OFFSET_p540, RULE_0_________},	// Etc/GMT-9
  {0xdd19, p13 NTZ_OFFSET_p780, RULE_0_________},	// Pacific/Tongatapu
  {0xdd6a, EST NTZ_OFFSET_m300, RULE_0_________},	// Jamaica
  {0xddef, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// America/Indiana/Marengo
  {0xdf42, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Prague
  {0xdf73, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Sarajevo
  {0xe00c, p11 NTZ_OFFSET_p660, RULE_0_________},	// Pacific/Ponape
  {0xe039, WET NTZ_OFFSET_p0, RULE_2cJDa1CDa1},	// Atlantic/Faeroe
  {0xe0b3, p1345 NTZ_OFFSET_p765, RULE_2cDD11IDa1},	// Pacific/Chatham
  {0xe17d, p13 NTZ_OFFSET_p780, RULE_0_________},	// Pacific/Enderbury
  {0xe243, BST NTZ_OFFSET_p0, RULE_2cJDa1CDa1},	// Europe/Guernsey
  {0xe28b, CDT NTZ_OFFSET_m360, RULE_2bKD11CD21},	// America/Knox_IN
  {0xe352, UTC NTZ_OFFSET_p0, RULE_0_________},	// Etc/Universal
  {0xe3b8, GMT NTZ_OFFSET_p0, RULE_0_________},	// Africa/Nouakchott
  {0xe3d3, CST NTZ_OFFSET_m360, RULE_0_________},	// America/Belize
  {0xe488, p03 NTZ_OFFSET_p180, RULE_0_________},	// Asia/Qatar
  {0xe4fe, p08 NTZ_OFFSET_p480, RULE_0_________},	// Asia/Choibalsan
  {0xe5b0, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Argentina/ComodRivadavia
  {0xe6dc, GMT NTZ_OFFSET_p0, RULE_0_________},	// Atlantic/Reykjavik
  {0xe6f5, p00 NTZ_OFFSET_m60, RULE_2cJDa1CDa1},	// Atlantic/Azores
  {0xe782, MDT NTZ_OFFSET_m420, RULE_2bKD11CD21},	// America/Denver
  {0xe82b, p07 NTZ_OFFSET_p420, RULE_0_________},	// Asia/Vientiane
  {0xea0b, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Santarem
  {0xea13, WIB NTZ_OFFSET_p420, RULE_0_________},	// Asia/Jakarta
  {0xea45, m06 NTZ_OFFSET_m360, RULE_2cDD17ID17},	// Chile/EasterIsland
  {0xebf4, HKT NTZ_OFFSET_p480, RULE_0_________},	// Asia/Hong_Kong
  {0xedeb, EAT NTZ_OFFSET_p180, RULE_0_________},	// Indian/Mayotte
  {0xee15, EDT NTZ_OFFSET_m300, RULE_2bKD11CD21},	// EST5EDT
  {0xeec9, WAT NTZ_OFFSET_p60, RULE_0_________},	// Africa/Ndjamena
  {0xeefb, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// Europe/Nicosia
  {0xef8e, SAST NTZ_OFFSET_p120, RULE_0_________},	// Africa/Maseru
  {0xefc6, p03 NTZ_OFFSET_p180, RULE_0_________},	// Asia/Baghdad
  {0xefcf, p07 NTZ_OFFSET_p420, RULE_0_________},	// Asia/Novosibirsk
  {0xf076, p04 NTZ_OFFSET_p240, RULE_0_________},	// Indian/Reunion
  {0xf0a1, CAT NTZ_OFFSET_p120, RULE_0_________},	// Africa/Maputo
  {0xf0a2, p0630 NTZ_OFFSET_p390, RULE_0_________},	// Asia/Rangoon
  {0xf0d3, p05 NTZ_OFFSET_p300, RULE_0_________},	// Asia/Tashkent
  {0xf0d8, CAT NTZ_OFFSET_p120, RULE_0_________},	// Africa/Gaborone
  {0xf16c, p08 NTZ_OFFSET_p480, RULE_0_________},	// Asia/Irkutsk
  {0xf1bf, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Virgin
  {0xf1f0, p05 NTZ_OFFSET_p300, RULE_0_________},	// Asia/Almaty
  {0xf296, SST NTZ_OFFSET_m660, RULE_0_________},	// Pacific/Pago_Pago
  {0xf311, AST NTZ_OFFSET_m240, RULE_0_________},	// America/St_Lucia
  {0xf360, m03 NTZ_OFFSET_m180, RULE_0_________},	// America/Rosario
  {0xf39f, p03 NTZ_OFFSET_p180, RULE_0_________},	// Asia/Amman
  {0xf3fc, GMT NTZ_OFFSET_p0, RULE_0_________},	// Africa/Ouagadougou
  {0xf419, CET NTZ_OFFSET_p60, RULE_2cJDa1CDa1},	// Europe/Berlin
  {0xf432, EST NTZ_OFFSET_m300, RULE_0_________},	// America/Coral_Harbour
  {0xf43e, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// Europe/Kyiv
  {0xf492, m01 NTZ_OFFSET_m120, RULE_2bJDa1CDa7},	// America/Godthab
  {0xf557, p04 NTZ_OFFSET_p240, RULE_0_________},	// Indian/Mahe
  {0xf6c4, p0330 NTZ_OFFSET_p210, RULE_0_________},	// Asia/Tehran
  {0xf7ee, EAT NTZ_OFFSET_p180, RULE_0_________},	// Africa/Asmara
  {0xf882, p06 NTZ_OFFSET_p360, RULE_0_________},	// Asia/Omsk
  {0xf8ae, p06 NTZ_OFFSET_p360, RULE_0_________},	// Asia/Dhaka
  {0xfa0a, GMT NTZ_OFFSET_p0, RULE_0_________},	// Africa/Conakry
  {0xfb41, m09 NTZ_OFFSET_m540, RULE_0_________},	// Pacific/Gambier
  {0xfb96, WAT NTZ_OFFSET_p60, RULE_0_________},	// Africa/Douala
  {0xfbb5, m03 NTZ_OFFSET_m180, RULE_0_________},	// Brazil/East
  {0xfbe9, AEST NTZ_OFFSET_p600, RULE_2cDD11JD11},	// Australia/Sydney
  {0xfc8b, GMT NTZ_OFFSET_p0, RULE_0_________},	// Africa/Timbuktu
  {0xfcfe, p09 NTZ_OFFSET_p540, RULE_0_________},	// Asia/Yakutsk
  {0xfd7c, NST NTZ_OFFSET_m210, RULE_2bKD11CD21},	// Canada/Newfoundland
  {0xfd97, p03 NTZ_OFFSET_p180, RULE_0_________},	// Europe/Istanbul
  {0xfe55, CST NTZ_OFFSET_m360, RULE_0_________},	// Mexico/General
  {0xfe74, MDT NTZ_OFFSET_m420, RULE_2bKD11CD21},	// US/Mountain
  {0xff26, EET NTZ_OFFSET_p120, RULE_2cJDa1CDa1},	// Europe/Zaporozhye
  {0xff2d, AST NTZ_OFFSET_m240, RULE_0_________},	// America/Tortola
  {0xff79, HDT NTZ_OFFSET_m600, RULE_2bKD11CD21},	// US/Aleutian
  {0xff8a, GMT NTZ_OFFSET_p0, RULE_0_________},	// Africa/Sao_Tome
/* }ntz::const_struct_ntz_iana_ntz_db -- generated code do not touch */
};

const struct ntz_iana *ntz_find_tz_key( uint16_t key )
{
  size_t lo = 0;
  size_t hi = ARRAY_SIZE( ntz_db );

  while ( lo < hi )
  {
    size_t mid = lo + ( hi - lo ) / 2;
    uint16_t hash = ntz_db[mid].iana_name_hash;
    if ( hash < key )
      lo = mid + 1;
    else
      hi = mid;
  }
  if ( lo < ARRAY_SIZE( ntz_db ) && ntz_db[lo].iana_name_hash == key )
    return ( &ntz_db[lo] );

  return ( NULL );
}

#if NTZ_NAME_API == 1

/*
        source: https://github.com/halloweeks/sha1

	MIT License

	Copyright (c) 2024 Hallo Weeks

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#define SHA1_BLOCK_SIZE 20

typedef struct
{
  unsigned char data[64];
  unsigned int datalen;
  unsigned long long bitlen;
  unsigned int state[5];
  unsigned int k[4];
} SHA1_CTX;

#define ROTLEFT(a, b) ((a << b) | (a >> (32 - b)))


static inline void sha1_transform( SHA1_CTX *ctx, const unsigned char data[] )
{
  unsigned int a,
   b,
   c,
   d,
   e,
   i,
   j,
   t,
   m[80];

  for ( i = 0, j = 0; i < 16; ++i, j += 4 )
    m[i] = ( data[j] << 24 ) + ( data[j + 1] << 16 ) + ( data[j + 2] << 8 ) + ( data[j + 3] );
  for ( ; i < 80; ++i )
  {
    m[i] = ( m[i - 3] ^ m[i - 8] ^ m[i - 14] ^ m[i - 16] );
    m[i] = ( m[i] << 1 ) | ( m[i] >> 31 );
  }

  a = ctx->state[0];
  b = ctx->state[1];
  c = ctx->state[2];
  d = ctx->state[3];
  e = ctx->state[4];

  for ( i = 0; i < 20; ++i )
  {
    t = ROTLEFT( a, 5 ) + ( ( b & c ) ^ ( ~b & d ) ) + e + ctx->k[0] + m[i];
    e = d;
    d = c;
    c = ROTLEFT( b, 30 );
    b = a;
    a = t;
  }
  for ( ; i < 40; ++i )
  {
    t = ROTLEFT( a, 5 ) + ( b ^ c ^ d ) + e + ctx->k[1] + m[i];
    e = d;
    d = c;
    c = ROTLEFT( b, 30 );
    b = a;
    a = t;
  }
  for ( ; i < 60; ++i )
  {
    t = ROTLEFT( a, 5 ) + ( ( b & c ) ^ ( b & d ) ^ ( c & d ) ) + e + ctx->k[2] + m[i];
    e = d;
    d = c;
    c = ROTLEFT( b, 30 );
    b = a;
    a = t;
  }
  for ( ; i < 80; ++i )
  {
    t = ROTLEFT( a, 5 ) + ( b ^ c ^ d ) + e + ctx->k[3] + m[i];
    e = d;
    d = c;
    c = ROTLEFT( b, 30 );
    b = a;
    a = t;
  }

  ctx->state[0] += a;
  ctx->state[1] += b;
  ctx->state[2] += c;
  ctx->state[3] += d;
  ctx->state[4] += e;
}

#define SHA1_Init(ctx) do \
{ \
	(ctx)->datalen = 0; \
	(ctx)->bitlen = 0; \
	(ctx)->state[0] = 0x67452301; \
	(ctx)->state[1] = 0xEFCDAB89; \
	(ctx)->state[2] = 0x98BADCFE; \
	(ctx)->state[3] = 0x10325476; \
	(ctx)->state[4] = 0xc3d2e1f0; \
	(ctx)->k[0] = 0x5a827999; \
	(ctx)->k[1] = 0x6ed9eba1; \
	(ctx)->k[2] = 0x8f1bbcdc; \
	(ctx)->k[3] = 0xca62c1d6; \
} while(0)

#define SHA1_Update(ctx, dat, len) do \
{ \
	size_t i; \
	for (i = 0; i < (len); ++i) { \
		(ctx)->data[(ctx)->datalen] = (dat)[i]; \
		(ctx)->datalen++; \
		if ((ctx)->datalen == 64) { \
			sha1_transform((ctx), (ctx)->data); \
			(ctx)->bitlen += 512; \
			(ctx)->datalen = 0; \
		} \
	} \
} while(0)

#define SHA1_Final(ctx, hash) do \
{ \
	unsigned int i; \
	i = (ctx)->datalen; \
	/* Pad whatever data is left in the buffer. */ \
	if ((ctx)->datalen < 56) { \
		(ctx)->data[i++] = 0x80; \
		while (i < 56) \
			(ctx)->data[i++] = 0x00; \
	} \
	else { \
		(ctx)->data[i++] = 0x80; \
		while (i < 64) \
			(ctx)->data[i++] = 0x00; \
		sha1_transform((ctx), (ctx)->data); \
		memset((ctx)->data, 0, 56); \
	} \
	/* Append to the padding the total message's length in bits and transform. */ \
	(ctx)->bitlen += (ctx)->datalen * 8; \
	(ctx)->data[63] = (ctx)->bitlen; \
	(ctx)->data[62] = (ctx)->bitlen >> 8; \
	(ctx)->data[61] = (ctx)->bitlen >> 16; \
	(ctx)->data[60] = (ctx)->bitlen >> 24; \
	(ctx)->data[59] = (ctx)->bitlen >> 32; \
	(ctx)->data[58] = (ctx)->bitlen >> 40; \
	(ctx)->data[57] = (ctx)->bitlen >> 48; \
	(ctx)->data[56] = (ctx)->bitlen >> 56; \
	sha1_transform((ctx), (ctx)->data); \
	/* Since this implementation uses little endian byte ordering and MD uses big endian, */ \
	/* reverse all the bytes when copying the final state to the output hash. */ \
	for (i = 0; i < 4; ++i) { \
		(hash)[i]      = ((ctx)->state[0] >> (24 - i * 8)) & 0x000000ff; \
		(hash)[i + 4]  = ((ctx)->state[1] >> (24 - i * 8)) & 0x000000ff; \
		(hash)[i + 8]  = ((ctx)->state[2] >> (24 - i * 8)) & 0x000000ff; \
		(hash)[i + 12] = ((ctx)->state[3] >> (24 - i * 8)) & 0x000000ff; \
		(hash)[i + 16] = ((ctx)->state[4] >> (24 - i * 8)) & 0x000000ff; \
	} \
} while(0)

#define SHA1(buf, len, hash) do \
{ \
	SHA1_CTX ctx; \
	SHA1_Init(&ctx); \
	SHA1_Update(&ctx, buf, len); \
	SHA1_Final(&ctx, hash); \
} while(0)

const struct ntz_iana *ntz_find_tz_name( const char *name, int len )
{
  unsigned char hash[SHA1_BLOCK_SIZE] = { 0 };
  SHA1( name, len, hash );
#if ( NTZ_HASH_SHIFT % 2)
  // odd nibble shift
  int h = ( NTZ_HASH_SHIFT - 1 ) / 2;
  uint16_t key = ( hash[h] & 0x0f ) << 12 | ( hash[h + 1] ) << 4 | ( hash[h + 2] & 0xf0 ) >> 4;
#else
  // even nibble shift
  uint16_t key = hash[NTZ_HASH_SHIFT / 2] << 8 | hash[NTZ_HASH_SHIFT / 2 + 1];
#endif
  return ( ntz_find_tz_key( key ) );
}

#endif

#endif
