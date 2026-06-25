#ifndef NTZ_H
#define NTZ_H

#ifndef NTZ_ABBREV
#define NTZ_ABBREV 0
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
};

#define ntz_get_rule(iana) ntz_rules[(iana)->dst_rule]

const struct ntz_iana *ntz_find_tz_key( uint16_t key );
#if NTZ_NAME_API == 1
const struct ntz_iana *ntz_find_tz_name( const char *name, int len );
#endif
int ntz_day_of_week( int year, int month, int day );
int ntz_epoch_to_tm( int64_t epoch, struct ntz_tm *tm, const struct ntz_iana *iana );
int ntz_get_dst_offset_hr( int64_t epoch, const struct ntz_iana *iana );
int ntz_dst_offset_hr( struct ntz_tm *tm, const struct ntz_iana *iana );
int ntz_epoch_to_localtime( int64_t epoch, struct ntz_tm *tm, const struct ntz_iana *iana );

#endif

#ifdef NTZ_IMPLEMENTATION

#define NTZ_DECODE_INT(c) ( ((c)>='a'&&(c)<='z') ? (-1*((c)-'a'+1)) : ( ((c)>='0'&&(c)<='9') ? ((c)-'0')    : 0 ) )
#define NTZ_DECODE_DAY(c) ( ((c)>='a'&&(c)<='z') ? ((c)-'a'+1)      : ( ((c)>='0'&&(c)<='4') ? ((c)-'0'+27) : 0 ) )

enum ntz_rulelist
{
  RULE_0_________,
  RULE_1bDAb1IAb1,
  RULE_1bJDa1CAx7,
  RULE_1bKD11CD21,
  RULE_1cCDa1CDa1,
  RULE_1cCDa1JDa1,
  RULE_1c_D06DDa6,
  RULE_1cDD11IDa1,
  RULE_1cDD11JD11,
  RULE_1cDD17ID17,
  RULE_1cJAx7CAx7,
  RULE_1cJDa1CAw6,
  RULE_1cJDa1CDa1,
  RULE_1cKD11CD21,
  RULECOUNT
};

const char ntz_rules[][11] = {
  [RULE_0_________] = "0---------",
  [RULE_1bDAb1IAb1] = "1bDAb1IAb1",
  [RULE_1bJDa1CAx7] = "1bJDa1CAx7",
  [RULE_1bKD11CD21] = "1bKD11CD21",
  [RULE_1cCDa1CDa1] = "1cCDa1CDa1",
  [RULE_1cCDa1JDa1] = "1cCDa1JDa1",
  [RULE_1c_D06DDa6] = "1c@D06DDa6",
  [RULE_1cDD11IDa1] = "1cDD11IDa1",
  [RULE_1cDD11JD11] = "1cDD11JD11",
  [RULE_1cDD17ID17] = "1cDD17ID17",
  [RULE_1cJAx7CAx7] = "1cJAx7CAx7",
  [RULE_1cJDa1CAw6] = "1cJDa1CAw6",
  [RULE_1cJDa1CDa1] = "1cJDa1CDa1",
  [RULE_1cKD11CD21] = "1cKD11CD21",
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

int ntz_day_of_week( int year, int month, int day )
{
  if ( day > 30 )
  {
    if ( month == 4 || month == 6 || month == 9 || month == 11 )
      return 0;
  }
  if ( month == 2 && day > 28 )
  {
    if ( year % 4 != 0 )
      return 0;
    if ( year % 100 == 0 && year % 400 != 0 )
      return 0;
  }
  int MDS = ( year - 1 ) * 365 + ( ( year - 1 ) / 4 ) + ( month - 1 ) * 30 + ( day - 11 );
  int COR = ( ( year - 1 ) / 100 ) - 16;
  if ( COR > 0 )
    MDS -= COR;
  int CO = ( ( year - 1 ) / 400 ) - 4;
  if ( CO > 0 )
    MDS += CO;
  int X = 0;
  for ( int i = 1; i <= ( month - 1 ); i++ )
  {
    X++;
    if ( X == 1 )
      MDS++;
    else if ( X == 3 )
      MDS++;
    else if ( X == 5 )
      MDS++;
    else if ( X == 7 )
      MDS++;
    else if ( X == 8 )
      MDS++;
    else if ( X == 10 )
      MDS++;
    else if ( X == 12 )
      MDS++;
    else if ( X == 2 )
    {
      if ( year % 4 == 0 )
      {
        if ( year > 1752 )
        {
          if ( year % 100 == 0 )
          {
            MDS -= 2;
            if ( year % 400 == 0 )
              MDS++;
          } else
            MDS--;
        } else
          MDS--;
      } else
        MDS -= 2;
    }
  }
  return ( MDS % 7 == 0 ? 7 : MDS % 7 );
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
      for ( int i = 1; i < 31 && oc > 0; i++ )
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

int ntz_epoch_to_tm( int64_t epoch, struct ntz_tm *tm, const struct ntz_iana *iana )
{
  if ( NULL == tm || NULL == iana )
    return ( -1 );

  int64_t tzoffset_min = iana->offset * 10L;
  epoch += tzoffset_min * 60;
  int64_t days = epoch / 86400;
  int64_t rem = epoch % 86400;

  int hour = rem / 3600;
  int minute = ( rem % 3600 ) / 60;
  int second = rem % 60;

  int64_t y = 1970;

  // Number of 400-year cycles
  int64_t n400 = days / 146097; // 365*400 + 97 leap days
  y += n400 * 400;
  days -= n400 * 146097;

  // Number of 100-year cycles
  int64_t n100 = days / 36524;  // 365*100 + 24 leap days
  if ( n100 == 4 )
    n100 = 3;                   // last day of 400-year cycle
  y += n100 * 100;
  days -= n100 * 36524;

  // Number of 4-year cycles
  int64_t n4 = days / 1461;     // 365*4 + 1 leap day
  y += n4 * 4;
  days -= n4 * 1461;

  // Remaining years
  int64_t n1 = days / 365;
  if ( n1 == 4 )
    n1 = 3;                     // last day of 4-year cycle
  y += n1;
  days -= n1 * 365;

  // Now `days` is 0-based day of year
  int day_of_year = ( int )days;

  uint8_t month_lengths[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  int leap = ( y % 4 == 0 && ( y % 100 != 0 || y % 400 == 0 ) );
  if ( leap )
    month_lengths[1]++;
  // find month
  int month = 0;
  while ( day_of_year >= month_lengths[month] )
  {
    day_of_year -= month_lengths[month];
    month++;
  }
  int day = day_of_year + 1;

  tm->tm_year = y - 1900;
  tm->tm_mon = month;           // 0..11
  tm->tm_mday = day;            // 1..31
  tm->tm_hour = hour;
  tm->tm_min = minute;
  tm->tm_sec = second;

  return ( 0 );
}

int ntz_dst_offset_hr( struct ntz_tm *tm, const struct ntz_iana *iana )
{
  if ( !iana || !tm )
    return ( 0 );
  const char *rule = ntz_get_rule( iana );
  if ( !ntz_check_rule( rule ) )
    return ( 0 );
  if ( rule[0] == '0' )
    return 0;
  int offset = NTZ_DECODE_INT( rule[0] );
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
      if ( month > monthd && month < months )
        return offset;
      else
        return 0;
    } else
    {
      if ( month > months && month < monthd )
        return 0;
      else
        return offset;
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
  } else
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

int ntz_get_dst_offset_hr( int64_t epoch, const struct ntz_iana *iana )
{
  int dst;
  struct ntz_tm dat;
  if ( !iana )
    return ( 0 );

  ntz_epoch_to_tm( epoch, &dat, iana );
  dst=ntz_dst_offset_hr( &dat, iana );

  return(dst);
}

int ntz_epoch_to_localtime( int64_t epoch, struct ntz_tm *tm, const struct ntz_iana *iana )
{
  if(!iana || !tm) return(-1);
  int dst=ntz_get_dst_offset_hr(epoch, iana);
  epoch+=dst*60*60;
  ntz_epoch_to_tm( epoch, tm, iana);
  return(0);
}

#if NTZ_ABBREV == 1

#define ACST 0,
#define ACT 1,
#define ACWST 2,
#define AEST 3,
#define AFT 4,
#define AKST 5,
#define AMT 6,
#define ART 7,
#define AST 8,
#define AWST 9,
#define AZOT 10,
#define AZT 11,
#define BOT 12,
#define BRT 13,
#define BST 14,
#define BTT 15,
#define CAT 16,
#define CET 17,
#define CHAST 18,
#define ChST 19,
#define CKT 20,
#define CLT 21,
#define COT 22,
#define CST 23,
#define CVT 24,
#define DAVT 25,
#define EAST 26,
#define EAT 27,
#define ECT 28,
#define EET 29,
#define EST 30,
#define FJT 31,
#define FKST 32,
#define FNT 33,
#define GALT 34,
#define GAMT 35,
#define GET 36,
#define GFT 37,
#define GILT 38,
#define GMT 39,
#define GMTp1 40,
#define GMTm1 41,
#define GMTp10 42,
#define GMTm10 43,
#define GMTp11 44,
#define GMTm11 45,
#define GMTp12 46,
#define GMTm12 47,
#define GMTp13 48,
#define GMTp14 49,
#define GMTp2 50,
#define GMTm2 51,
#define GMTp3 52,
#define GMTm3 53,
#define GMTp4 54,
#define GMTm4 55,
#define GMTp5 56,
#define GMTm5 57,
#define GMTp6 58,
#define GMTm6 59,
#define GMTp7 60,
#define GMTm7 61,
#define GMTp8 62,
#define GMTm8 63,
#define GMTp9 64,
#define GMTm9 65,
#define GST 66,
#define GYT 67,
#define HAST 68,
#define HKT 69,
#define HOVT 70,
#define ICT 71,
#define IOT 72,
#define IRKT 73,
#define IRST 74,
#define IST 75,
#define JST 76,
#define KGT 77,
#define KOST 78,
#define KST 79,
#define KZT 80,
#define LHST 81,
#define LINT 82,
#define MART 83,
#define MAWT 84,
#define MHT 85,
#define MMT 86,
#define MSK 87,
#define MST 88,
#define MUT 89,
#define MVT 90,
#define MYT 91,
#define NCT 92,
#define NFT 93,
#define NOVT 94,
#define NPT 95,
#define NRT 96,
#define NST 97,
#define NUT 98,
#define NZST 99,
#define OMST 100,
#define PET 101,
#define PETT 102,
#define PGT 103,
#define PHOT 104,
#define PHT 105,
#define PKT 106,
#define PM 107,
#define PST 108,
#define PWT 109,
#define PYT 110,
#define SAKT 111,
#define SAMT 112,
#define SAST 113,
#define SBT 114,
#define SGT 115,
#define SRT 116,
#define SST 117,
#define TAHT 118,
#define TJT 119,
#define TKT 120,
#define TLT 121,
#define TMT 122,
#define TOT 123,
#define TRT 124,
#define TWT 125,
#define ULAT 126,
#define UTC 127,
#define UYT 128,
#define UZT 129,
#define VET 130,
#define VLAT 131,
#define VUT 132,
#define WAT 133,
#define WET 134,
#define WIB 135,
#define WIT 136,
#define WITA 137,
#define YAKT 138,
#define YEKT 139,
#define YT 140,

const char *ntz_abbrev[] = {
  "ACST",
  "ACT",
  "ACWST",
  "AEST",
  "AFT",
  "AKST",
  "AMT",
  "ART",
  "AST",
  "AWST",
  "AZOT",
  "AZT",
  "BOT",
  "BRT",
  "BST",
  "BTT",
  "CAT",
  "CET",
  "CHAST",
  "ChST",
  "CKT",
  "CLT",
  "COT",
  "CST",
  "CVT",
  "DAVT",
  "EAST",
  "EAT",
  "ECT",
  "EET",
  "EST",
  "FJT",
  "FKST",
  "FNT",
  "GALT",
  "GAMT",
  "GET",
  "GFT",
  "GILT",
  "GMT",
  "GMT+1",
  "GMT-1",
  "GMT+10",
  "GMT-10",
  "GMT+11",
  "GMT-11",
  "GMT+12",
  "GMT-12",
  "GMT+13",
  "GMT+14",
  "GMT+2",
  "GMT-2",
  "GMT+3",
  "GMT-3",
  "GMT+4",
  "GMT-4",
  "GMT+5",
  "GMT-5",
  "GMT+6",
  "GMT-6",
  "GMT+7",
  "GMT-7",
  "GMT+8",
  "GMT-8",
  "GMT+9",
  "GMT-9",
  "GST",
  "GYT",
  "HAST",
  "HKT",
  "HOVT",
  "ICT",
  "IOT",
  "IRKT",
  "IRST",
  "IST",
  "JST",
  "KGT",
  "KOST",
  "KST",
  "KZT",
  "LHST",
  "LINT",
  "MART",
  "MAWT",
  "MHT",
  "MMT",
  "MSK",
  "MST",
  "MUT",
  "MVT",
  "MYT",
  "NCT",
  "NFT",
  "NOVT",
  "NPT",
  "NRT",
  "NST",
  "NUT",
  "NZST",
  "OMST",
  "PET",
  "PETT",
  "PGT",
  "PHOT",
  "PHT",
  "PKT",
  "PM",
  "PST",
  "PWT",
  "PYT",
  "SAKT",
  "SAMT",
  "SAST",
  "SBT",
  "SGT",
  "SRT",
  "SST",
  "TAHT",
  "TJT",
  "TKT",
  "TLT",
  "TMT",
  "TOT",
  "TRT",
  "TWT",
  "ULAT",
  "UTC",
  "UYT",
  "UZT",
  "VET",
  "VLAT",
  "VUT",
  "WAT",
  "WET",
  "WIB",
  "WIT",
  "WITA",
  "YAKT",
  "YEKT",
  "YT",
};
#else

#define   ACST
#define   ACT
#define   ACWST
#define   AEST
#define   AFT
#define   AKST
#define   AMT
#define   ART
#define   AST
#define   AWST
#define   AZOT
#define   AZT
#define   BOT
#define   BRT
#define   BST
#define   BTT
#define   CAT
#define   CET
#define   CHAST
#define   ChST
#define   CKT
#define   CLT
#define   COT
#define   CST
#define   CVT
#define   DAVT
#define   EAST
#define   EAT
#define   ECT
#define   EET
#define   EST
#define   FJT
#define   FKST
#define   FNT
#define   GALT
#define   GAMT
#define   GET
#define   GFT
#define   GILT
#define   GMT
#define   GMTp1
#define   GMTm1
#define   GMTp10
#define   GMTm10
#define   GMTp11
#define   GMTm11
#define   GMTp12
#define   GMTm12
#define   GMTp13
#define   GMTp14
#define   GMTp2
#define   GMTm2
#define   GMTp3
#define   GMTm3
#define   GMTp4
#define   GMTm4
#define   GMTp5
#define   GMTm5
#define   GMTp6
#define   GMTm6
#define   GMTp7
#define   GMTm7
#define   GMTp8
#define   GMTm8
#define   GMTp9
#define   GMTm9
#define   GST
#define   GYT
#define   HAST
#define   HKT
#define   HOVT
#define   ICT
#define   IOT
#define   IRKT
#define   IRST
#define   IST
#define   JST
#define   KGT
#define   KOST
#define   KST
#define   KZT
#define   LHST
#define   LINT
#define   MART
#define   MAWT
#define   MHT
#define   MMT
#define   MSK
#define   MST
#define   MUT
#define   MVT
#define   MYT
#define   NCT
#define   NFT
#define   NOVT
#define   NPT
#define   NRT
#define   NST
#define   NUT
#define   NZST
#define   OMST
#define   PET
#define   PETT
#define   PGT
#define   PHOT
#define   PHT
#define   PKT
#define   PM
#define   PST
#define   PWT
#define   PYT
#define   SAKT
#define   SAMT
#define   SAST
#define   SBT
#define   SGT
#define   SRT
#define   SST
#define   TAHT
#define   TJT
#define   TKT
#define   TLT
#define   TMT
#define   TOT
#define   TRT
#define   TWT
#define   ULAT
#define   UTC
#define   UYT
#define   UZT
#define   VET
#define   VLAT
#define   VUT
#define   WAT
#define   WET
#define   WIB
#define   WIT
#define   WITA
#define   YAKT
#define   YEKT
#define   YT

#endif

const struct ntz_iana ntz_db[] = {
  {0xf781, GMT 0, RULE_0_________},
  {0x1f0f, GMT 0, RULE_0_________},
  {0x817d, EAT 18, RULE_0_________},
  {0x95f1, CET 6, RULE_0_________},
  {0xce3b, EAT 18, RULE_0_________},
  {0x53c0, EAT 18, RULE_0_________},
  {0x4b5a, GMT 0, RULE_0_________},
  {0xc7a0, WAT 6, RULE_0_________},
  {0xf320, GMT 0, RULE_0_________},
  {0x6425, GMT 0, RULE_0_________},
  {0xe1cc, CAT 12, RULE_0_________},
  {0x1b72, WAT 6, RULE_0_________},
  {0xff81, CAT 12, RULE_0_________},
  {0xf6da, EET 12, RULE_1c_D06DDa6},
  {0x05dd, WET 0, RULE_0_________},
  {0x83c1, CET 6, RULE_1cJDa1CDa1},
  {0x106c, GMT 0, RULE_0_________},
  {0x5929, GMT 0, RULE_0_________},
  {0x5f76, EAT 18, RULE_0_________},
  {0x8968, EAT 18, RULE_0_________},
  {0x5fc0, WAT 6, RULE_0_________},
  {0x8203, WET 0, RULE_1cCDa1CDa1},
  {0xea61, GMT 0, RULE_0_________},
  {0x21cf, CAT 12, RULE_0_________},
  {0x2b45, CAT 12, RULE_0_________},
  {0xd269, SAST 12, RULE_0_________},
  {0xbba5, CAT 12, RULE_0_________},
  {0xa685, EAT 18, RULE_0_________},
  {0x7e34, CAT 12, RULE_0_________},
  {0x830b, CAT 12, RULE_0_________},
  {0xf9f6, WAT 6, RULE_0_________},
  {0x73fb, WAT 6, RULE_0_________},
  {0x9bd1, WAT 6, RULE_0_________},
  {0xc4e6, GMT 0, RULE_0_________},
  {0xed6f, WAT 6, RULE_0_________},
  {0xc458, CAT 12, RULE_0_________},
  {0x967b, CAT 12, RULE_0_________},
  {0x09cd, WAT 6, RULE_0_________},
  {0x2620, CAT 12, RULE_0_________},
  {0x8619, SAST 12, RULE_0_________},
  {0x0fcf, SAST 12, RULE_0_________},
  {0xed17, EAT 18, RULE_0_________},
  {0xe99d, GMT 0, RULE_0_________},
  {0xe127, EAT 18, RULE_0_________},
  {0x30c7, WAT 6, RULE_0_________},
  {0x8f0b, WAT 6, RULE_0_________},
  {0x4e15, GMT 0, RULE_0_________},
  {0x88ce, GMT 0, RULE_0_________},
  {0x2c53, WAT 6, RULE_0_________},
  {0x99c2, GMT 0, RULE_0_________},
  {0xc2c0, GMT 0, RULE_0_________},
  {0xc250, EET 12, RULE_0_________},
  {0xfd7c, CET 6, RULE_0_________},
  {0xe494, CAT 12, RULE_0_________},
  {0x816d, HAST - 60, RULE_1bKD11CD21},
  {0x1647, AKST - 54, RULE_1bKD11CD21},
  {0x725d, AST - 24, RULE_0_________},
  {0xc7c7, AST - 24, RULE_0_________},
  {0x8453, BRT - 18, RULE_0_________},
  {0x9ee0, ART - 18, RULE_0_________},
  {0x3c6b, ART - 18, RULE_0_________},
  {0xd42a, ART - 18, RULE_0_________},
  {0x2428, ART - 18, RULE_0_________},
  {0x2bbc, ART - 18, RULE_0_________},
  {0x9478, ART - 18, RULE_0_________},
  {0x36be, ART - 18, RULE_0_________},
  {0x2faa, ART - 18, RULE_0_________},
  {0xea04, ART - 18, RULE_0_________},
  {0x28b7, ART - 18, RULE_0_________},
  {0x0c9a, ART - 18, RULE_0_________},
  {0xf47d, ART - 18, RULE_0_________},
  {0xcc26, ART - 18, RULE_0_________},
  {0x9711, AST - 24, RULE_0_________},
  {0xd50b, PYT - 18, RULE_0_________},
  {0x22c4, EST - 30, RULE_0_________},
  {0x444d, HAST - 60, RULE_1bKD11CD21},
  {0xbcce, BRT - 18, RULE_0_________},
  {0x7fcc, CST - 36, RULE_0_________},
  {0x0ad9, AST - 24, RULE_0_________},
  {0x259c, BRT - 18, RULE_0_________},
  {0x2fc2, CST - 36, RULE_0_________},
  {0xd831, AST - 24, RULE_0_________},
  {0xd857, AMT - 24, RULE_0_________},
  {0xd0ea, COT - 30, RULE_0_________},
  {0x3791, MST - 42, RULE_1bKD11CD21},
  {0xd738, ART - 18, RULE_0_________},
  {0x1ca0, MST - 42, RULE_1bKD11CD21},
  {0x1aa1, AMT - 24, RULE_0_________},
  {0x1f58, EST - 30, RULE_0_________},
  {0xc5e3, VET - 24, RULE_0_________},
  {0xb77b, ART - 18, RULE_0_________},
  {0xb70b, GFT - 18, RULE_0_________},
  {0xca7e, EST - 30, RULE_0_________},
  {0x4b13, CST - 36, RULE_1bKD11CD21},
  {0x18e6, CST - 36, RULE_0_________},
  {0x6d15, MST - 42, RULE_1bKD11CD21},
  {0x6e31, EST - 30, RULE_0_________},
  {0x6ce1, ART - 18, RULE_0_________},
  {0xdd33, CST - 36, RULE_0_________},
  {0x0805, CLT - 18, RULE_0_________},
  {0xc9f9, YT - 42, RULE_0_________},
  {0xf58b, AMT - 24, RULE_0_________},
  {0x45b6, AST - 24, RULE_0_________},
  {0xf3e4, GMT 0, RULE_0_________},
  {0x6fbc, YT - 42, RULE_0_________},
  {0xa3a3, YT - 42, RULE_0_________},
  {0x3a45, MST - 42, RULE_1bKD11CD21},
  {0xd5ee, EST - 30, RULE_1bKD11CD21},
  {0xe8b2, AST - 24, RULE_0_________},
  {0x65a1, MST - 42, RULE_1bKD11CD21},
  {0xa140, ACT - 30, RULE_0_________},
  {0x7d32, CST - 36, RULE_0_________},
  {0x4775, PST - 48, RULE_1bKD11CD21},
  {0xac80, YT - 42, RULE_0_________},
  {0xcf46, EST - 30, RULE_1bKD11CD21},
  {0x6089, BRT - 18, RULE_0_________},
  {0x6cea, AST - 24, RULE_1bKD11CD21},
  {0xa26e, GMTm2 - 12, RULE_1bJDa1CAx7},
  {0x9187, AST - 24, RULE_1bKD11CD21},
  {0xfa2c, EST - 30, RULE_1bKD11CD21},
  {0x1000, AST - 24, RULE_0_________},
  {0xec8e, AST - 24, RULE_0_________},
  {0x7c4c, CST - 36, RULE_0_________},
  {0x2fc7, ECT - 30, RULE_0_________},
  {0x3f99, GYT - 24, RULE_0_________},
  {0x80c1, AST - 24, RULE_1bKD11CD21},
  {0xa457, CST - 30, RULE_1bKD11CD21},
  {0x32be, MST - 42, RULE_0_________},
  {0xf488, EST - 30, RULE_1bKD11CD21},
  {0xc28d, CST - 36, RULE_1bKD11CD21},
  {0xa772, EST - 30, RULE_1bKD11CD21},
  {0x6f7c, EST - 30, RULE_1bKD11CD21},
  {0x9dc7, CST - 36, RULE_1bKD11CD21},
  {0x89a8, EST - 30, RULE_1bKD11CD21},
  {0x9d72, EST - 30, RULE_1bKD11CD21},
  {0x06ab, EST - 30, RULE_1bKD11CD21},
  {0x5dd5, EST - 30, RULE_1bKD11CD21},
  {0xa91a, MST - 42, RULE_1bKD11CD21},
  {0xc0dc, EST - 30, RULE_1bKD11CD21},
  {0x73c7, EST - 30, RULE_0_________},
  {0xc7f6, ART - 18, RULE_0_________},
  {0xd1cf, AKST - 54, RULE_1bKD11CD21},
  {0x1e70, EST - 30, RULE_1bKD11CD21},
  {0x2878, EST - 30, RULE_1bKD11CD21},
  {0xe036, CST - 36, RULE_1bKD11CD21},
  {0x5dca, AST - 24, RULE_0_________},
  {0x8d19, BOT - 24, RULE_0_________},
  {0xb83a, PET - 30, RULE_0_________},
  {0xca12, PST - 48, RULE_1bKD11CD21},
  {0x39c4, EST - 30, RULE_1bKD11CD21},
  {0x44ba, AST - 24, RULE_0_________},
  {0x74c0, BRT - 18, RULE_0_________},
  {0xc7d3, CST - 36, RULE_0_________},
  {0x591c, AMT - 24, RULE_0_________},
  {0x67ec, AST - 24, RULE_0_________},
  {0x97d4, AST - 24, RULE_0_________},
  {0xd7fd, CST - 36, RULE_1bKD11CD21},
  {0xff8a, MST - 42, RULE_0_________},
  {0x8976, ART - 18, RULE_0_________},
  {0x7552, CST - 36, RULE_1bKD11CD21},
  {0x8f5e, CST - 36, RULE_0_________},
  {0x657c, AKST - 54, RULE_1bKD11CD21},
  {0xa666, CST - 36, RULE_0_________},
  {0xf7c2, PM - 18, RULE_1bKD11CD21},
  {0x529a, AST - 24, RULE_1bKD11CD21},
  {0x2ff4, CST - 36, RULE_0_________},
  {0xf368, UYT - 18, RULE_0_________},
  {0xf314, EST - 30, RULE_1bKD11CD21},
  {0x6e54, AST - 24, RULE_0_________},
  {0xd5ca, EST - 30, RULE_1bKD11CD21},
  {0xec3d, EST - 30, RULE_1bKD11CD21},
  {0x5143, EST - 30, RULE_1bKD11CD21},
  {0x836d, AKST - 54, RULE_1bKD11CD21},
  {0x1a6d, FNT - 12, RULE_0_________},
  {0xf69c, CST - 36, RULE_1bKD11CD21},
  {0xb5ea, CST - 36, RULE_1bKD11CD21},
  {0xec6c, CST - 36, RULE_1bKD11CD21},
  {0x0789, GMTm2 - 12, RULE_1bJDa1CAx7},
  {0x79bc, CST - 36, RULE_1bKD11CD21},
  {0x10cd, EST - 30, RULE_0_________},
  {0x72e5, EST - 30, RULE_1bKD11CD21},
  {0xdf1d, SRT - 18, RULE_0_________},
  {0x6de3, MST - 42, RULE_0_________},
  {0x6deb, AST - 24, RULE_0_________},
  {0x76b4, EST - 30, RULE_1bKD11CD21},
  {0x2eb5, ACT - 30, RULE_0_________},
  {0x567b, AMT - 24, RULE_0_________},
  {0xa3a2, AST - 24, RULE_0_________},
  {0xb251, CLT - 18, RULE_0_________},
  {0xf48c, CST - 36, RULE_1bKD11CD21},
  {0x25ef, CST - 36, RULE_1bKD11CD21},
  {0x2b26, BRT - 18, RULE_0_________},
  {0x05d8, CST - 36, RULE_0_________},
  {0xb69f, CST - 36, RULE_1bKD11CD21},
  {0x49d5, ACT - 30, RULE_0_________},
  {0x37fa, ART - 18, RULE_0_________},
  {0x8b9f, PST - 48, RULE_1bKD11CD21},
  {0x76bf, BRT - 18, RULE_0_________},
  {0xd300, CLT - 24, RULE_1bDAb1IAb1},
  {0x3bfe, AST - 24, RULE_0_________},
  {0x4ffc, BRT - 18, RULE_0_________},
  {0x6b22, GMTm2 - 12, RULE_1bJDa1CAx7},
  {0xfed1, MST - 42, RULE_1bKD11CD21},
  {0x0c31, AKST - 54, RULE_1bKD11CD21},
  {0x7e8a, AST - 24, RULE_0_________},
  {0x54cf, NST - 21, RULE_1bKD11CD21},
  {0x6706, AST - 24, RULE_0_________},
  {0xc8f2, AST - 24, RULE_0_________},
  {0xda32, AST - 24, RULE_0_________},
  {0x6ca5, AST - 24, RULE_0_________},
  {0xd16c, CST - 36, RULE_0_________},
  {0x96ba, CST - 36, RULE_0_________},
  {0x5077, AST - 24, RULE_1bKD11CD21},
  {0xae20, EST - 30, RULE_1bKD11CD21},
  {0xbfb2, PST - 48, RULE_1bKD11CD21},
  {0x64c7, EST - 30, RULE_1bKD11CD21},
  {0x0011, AST - 24, RULE_0_________},
  {0x896e, PST - 48, RULE_1bKD11CD21},
  {0xb4a3, AST - 24, RULE_0_________},
  {0xe103, YT - 42, RULE_0_________},
  {0x42d8, CST - 36, RULE_1bKD11CD21},
  {0x0d76, AKST - 54, RULE_1bKD11CD21},
  {0xc80a, MST - 42, RULE_1bKD11CD21},
  {0xc694, AWST 48, RULE_0_________},
  {0x121f, DAVT 42, RULE_0_________},
  {0xfbc1, PGT 60, RULE_0_________},
  {0x03da, AEST 60, RULE_1cDD11JD11},
  {0x47c0, MAWT 30, RULE_0_________},
  {0xf47c, NZST 72, RULE_1cDD11IDa1},
  {0xc4ae, CLT - 18, RULE_0_________},
  {0xac47, CLT - 18, RULE_0_________},
  {0x126e, NZST 72, RULE_1cDD11IDa1},
  {0xc4cb, AST 18, RULE_0_________},
  {0xa121, GMT 0, RULE_1cJDa1CDa1},
  {0x7827, MAWT 30, RULE_0_________},
  {0xc382, CET 6, RULE_1cJDa1CDa1},
  {0x7074, AST 18, RULE_0_________},
  {0xd61d, KZT 30, RULE_0_________},
  {0xb6d4, GMTp3 18, RULE_0_________},
  {0xfacf, PETT 72, RULE_0_________},
  {0x596e, KZT 30, RULE_0_________},
  {0xddca, KZT 30, RULE_0_________},
  {0x35d9, TMT 30, RULE_0_________},
  {0xb7b6, TMT 30, RULE_0_________},
  {0x096b, KZT 30, RULE_0_________},
  {0xb9d3, AST 18, RULE_0_________},
  {0xf330, AST 18, RULE_0_________},
  {0xfa01, AZT 24, RULE_0_________},
  {0xe3cd, ICT 42, RULE_0_________},
  {0x80c9, NOVT 42, RULE_0_________},
  {0xc332, EET 12, RULE_1cJDa1CDa1},
  {0xc585, KGT 36, RULE_0_________},
  {0xccb0, MYT 48, RULE_0_________},
  {0xe3d1, IST 33, RULE_0_________},
  {0xfe14, YAKT 54, RULE_0_________},
  {0xa352, ULAT 48, RULE_0_________},
  {0x18f8, CST 48, RULE_0_________},
  {0xb506, CST 48, RULE_0_________},
  {0xb46f, IST 33, RULE_0_________},
  {0xfe33, BST 36, RULE_0_________},
  {0x58bd, GMTp3 18, RULE_0_________},
  {0xef68, BST 36, RULE_0_________},
  {0xac2d, TLT 54, RULE_0_________},
  {0x4f06, GST 24, RULE_0_________},
  {0x131f, TJT 30, RULE_0_________},
  {0x4f97, EET 12, RULE_1cJDa1CDa1},
  {0xffb6, EET 12, RULE_1cJAx7CAx7},
  {0x0725, CST 48, RULE_0_________},
  {0xab31, EET 12, RULE_1cJAx7CAx7},
  {0xb533, ICT 42, RULE_0_________},
  {0x53ea, HKT 48, RULE_0_________},
  {0x101c, HOVT 42, RULE_0_________},
  {0x26cd, IRKT 48, RULE_0_________},
  {0x4788, TRT 18, RULE_0_________},
  {0x89b6, WIB 42, RULE_0_________},
  {0x20f0, WIT 54, RULE_0_________},
  {0xe417, IST 12, RULE_1cJDa1CAw6},
  {0xd92c, AFT 27, RULE_0_________},
  {0xbe3e, PETT 72, RULE_0_________},
  {0xe828, PKT 30, RULE_0_________},
  {0x9ad5, CST 36, RULE_0_________},
  {0xd4cf, NPT 34, RULE_0_________},
  {0x8891, NPT 34, RULE_0_________},
  {0x1f2c, YAKT 54, RULE_0_________},
  {0x568c, IST 33, RULE_0_________},
  {0x7543, NOVT 42, RULE_0_________},
  {0x0145, SGT 48, RULE_0_________},
  {0x36ee, MYT 48, RULE_0_________},
  {0xaa58, AST 18, RULE_0_________},
  {0x39b7, CST 48, RULE_0_________},
  {0x77bd, CST 48, RULE_0_________},
  {0xa14f, SAKT 66, RULE_0_________},
  {0x34a0, WITA 48, RULE_0_________},
  {0x2ed7, PHT 48, RULE_0_________},
  {0x2e66, GST 24, RULE_0_________},
  {0x9310, EET 12, RULE_1cJDa1CDa1},
  {0xfc65, NOVT 42, RULE_0_________},
  {0xcf07, NOVT 42, RULE_0_________},
  {0x3d14, OMST 36, RULE_0_________},
  {0x164e, KZT 30, RULE_0_________},
  {0x3dff, ICT 42, RULE_0_________},
  {0x01f7, WIB 42, RULE_0_________},
  {0x1290, KST 54, RULE_0_________},
  {0xece1, AST 18, RULE_0_________},
  {0xcb9e, KZT 30, RULE_0_________},
  {0xa427, KZT 30, RULE_0_________},
  {0x7aeb, MMT 39, RULE_0_________},
  {0x0740, AST 18, RULE_0_________},
  {0xaef0, ICT 42, RULE_0_________},
  {0x1be3, SAKT 66, RULE_0_________},
  {0x8918, UZT 30, RULE_0_________},
  {0x9cb6, KST 54, RULE_0_________},
  {0x46cd, CST 48, RULE_0_________},
  {0x343d, SGT 48, RULE_0_________},
  {0xf91e, SAKT 66, RULE_0_________},
  {0xd406, TWT 48, RULE_0_________},
  {0x50eb, UZT 30, RULE_0_________},
  {0x42d0, GET 24, RULE_0_________},
  {0x3c37, IRST 21, RULE_0_________},
  {0xc19f, IST 12, RULE_1cJDa1CAw6},
  {0x07a9, BTT 36, RULE_0_________},
  {0xdbb1, BTT 36, RULE_0_________},
  {0x3adb, JST 54, RULE_0_________},
  {0x68fc, NOVT 42, RULE_0_________},
  {0x0d5c, WITA 48, RULE_0_________},
  {0xfe01, ULAT 48, RULE_0_________},
  {0x75e9, ULAT 48, RULE_0_________},
  {0x92f6, CST 36, RULE_0_________},
  {0xab60, VLAT 60, RULE_0_________},
  {0x533d, ICT 42, RULE_0_________},
  {0xb15e, VLAT 60, RULE_0_________},
  {0x83ac, YAKT 54, RULE_0_________},
  {0x767b, MMT 39, RULE_0_________},
  {0x6c57, YEKT 30, RULE_0_________},
  {0x0c0b, AMT 24, RULE_0_________},
  {0xa4f7, AZOT - 6, RULE_1cJDa1CDa1},
  {0x6beb, AST - 24, RULE_1cKD11CD21},
  {0x8b96, WET 0, RULE_1cJDa1CDa1},
  {0xa761, CVT - 6, RULE_0_________},
  {0x8834, WET 0, RULE_1cJDa1CDa1},
  {0xc128, WET 0, RULE_1cJDa1CDa1},
  {0x33e6, CET 6, RULE_1cJDa1CDa1},
  {0x7f0b, WET 0, RULE_1cJDa1CDa1},
  {0x00b7, GMT 0, RULE_0_________},
  {0x6320, GST - 12, RULE_0_________},
  {0x257d, GMT 0, RULE_0_________},
  {0x5c85, FKST - 18, RULE_0_________},
  {0x3286, AEST 60, RULE_1cDD11JD11},
  {0xddbb, ACST 57, RULE_1cDD11JD11},
  {0xe03f, AEST 60, RULE_0_________},
  {0x6222, ACST 57, RULE_1cDD11JD11},
  {0x2694, AEST 60, RULE_1cDD11JD11},
  {0x8757, AEST 60, RULE_1cDD11JD11},
  {0x6c85, ACST 57, RULE_0_________},
  {0xb576, ACWST 52, RULE_0_________},
  {0x6aa4, AEST 60, RULE_1cDD11JD11},
  {0xefb3, LHST 63, RULE_1cDD11JD11},
  {0xf2a4, AEST 60, RULE_0_________},
  {0x7191, LHST 63, RULE_1cDD11JD11},
  {0xccf2, AEST 60, RULE_1cDD11JD11},
  {0xe1ea, ACST 57, RULE_0_________},
  {0xbb8d, AEST 60, RULE_1cDD11JD11},
  {0xc88f, AWST 48, RULE_0_________},
  {0x776c, AEST 60, RULE_0_________},
  {0x660b, ACST 57, RULE_1cDD11JD11},
  {0x8bf6, AEST 60, RULE_1cDD11JD11},
  {0xc46b, AEST 60, RULE_1cDD11JD11},
  {0x4197, AEST 60, RULE_1cDD11JD11},
  {0xf4b9, AWST 48, RULE_0_________},
  {0xddf0, ACST 57, RULE_1cDD11JD11},
  {0xc90c, ACT - 30, RULE_0_________},
  {0x486a, FNT - 12, RULE_0_________},
  {0x8fb2, BRT - 18, RULE_0_________},
  {0x1d46, AMT - 24, RULE_0_________},
  {0xbede, AST - 24, RULE_1bKD11CD21},
  {0x2ab0, CST - 36, RULE_1bKD11CD21},
  {0xfe8b, EST - 30, RULE_1bKD11CD21},
  {0x7614, MST - 42, RULE_1bKD11CD21},
  {0xb531, NST - 21, RULE_1bKD11CD21},
  {0x8cf4, PST - 48, RULE_1bKD11CD21},
  {0xc8a7, CST - 36, RULE_0_________},
  {0x2991, YT - 42, RULE_0_________},
  {0xa8b7, CET 6, RULE_1cJDa1CDa1},
  {0x9594, CLT - 24, RULE_1bDAb1IAb1},
  {0xff7e, EAST - 36, RULE_1cDD17ID17},
  {0x5e7a, CST - 36, RULE_1bKD11CD21},
  {0xa906, CST - 30, RULE_1bKD11CD21},
  {0x37fb, EET 12, RULE_1cJDa1CDa1},
  {0x4df9, EET 12, RULE_1c_D06DDa6},
  {0x98fc, GMT 0, RULE_1cCDa1JDa1},
  {0x9c6f, EST - 30, RULE_0_________},
  {0x91fd, EST - 30, RULE_1bKD11CD21},
  {0x3151, UTC 0, RULE_0_________},
  {0xf312, GMT 0, RULE_0_________},
  {0x6ab2, GMTp1 6, RULE_0_________},
  {0xb438, GMTp10 60, RULE_0_________},
  {0xf3ee, GMTp11 66, RULE_0_________},
  {0xf928, GMTp12 72, RULE_0_________},
  {0xa674, GMTp13 78, RULE_0_________},
  {0x452c, GMTp14 84, RULE_0_________},
  {0x33ed, GMTp2 12, RULE_0_________},
  {0xebe6, GMTp3 18, RULE_0_________},
  {0xd7d7, GMTp4 24, RULE_0_________},
  {0x7812, GMTp5 30, RULE_0_________},
  {0xe380, GMTp6 36, RULE_0_________},
  {0xf5b8, GMTp7 42, RULE_0_________},
  {0x9405, GMTp8 48, RULE_0_________},
  {0x04ab, GMTp9 54, RULE_0_________},
  {0xad24, GMT 0, RULE_0_________},
  {0xa52c, GMTm1 - 6, RULE_0_________},
  {0x65c5, GMTm10 - 60, RULE_0_________},
  {0x7cec, GMTm11 - 66, RULE_0_________},
  {0x24d9, GMTm12 - 72, RULE_0_________},
  {0x4782, GMTm2 - 12, RULE_0_________},
  {0xc7ac, GMTm3 - 18, RULE_0_________},
  {0xe2a6, GMTm4 - 24, RULE_0_________},
  {0xa998, GMTm5 - 30, RULE_0_________},
  {0x7a17, GMTm6 - 36, RULE_0_________},
  {0x84bf, GMTm7 - 42, RULE_0_________},
  {0xe2be, GMTm8 - 48, RULE_0_________},
  {0x2a92, GMTm9 - 54, RULE_0_________},
  {0xb39f, GMT 0, RULE_0_________},
  {0x0950, GMT 0, RULE_0_________},
  {0xb8cc, UTC 0, RULE_0_________},
  {0x5c46, UTC 0, RULE_0_________},
  {0x0b86, UTC 0, RULE_0_________},
  {0x6bf5, UTC 0, RULE_0_________},
  {0xda25, CET 6, RULE_1cJDa1CDa1},
  {0x5d02, CET 6, RULE_1cJDa1CDa1},
  {0x3bca, SAMT 24, RULE_0_________},
  {0xb0b6, EET 12, RULE_1cJDa1CDa1},
  {0xf136, GMT 0, RULE_1cJDa1CDa1},
  {0x59f8, CET 6, RULE_1cJDa1CDa1},
  {0x3004, CET 6, RULE_1cJDa1CDa1},
  {0x4cfc, CET 6, RULE_1cJDa1CDa1},
  {0x45dd, CET 6, RULE_1cJDa1CDa1},
  {0xe357, EET 12, RULE_1cJDa1CDa1},
  {0x11a3, CET 6, RULE_1cJDa1CDa1},
  {0xf370, CET 6, RULE_1cJDa1CDa1},
  {0x024f, EET 12, RULE_1cJDa1CDa1},
  {0x2771, CET 6, RULE_1cJDa1CDa1},
  {0x667b, GMT 0, RULE_1cCDa1JDa1},
  {0x7759, CET 6, RULE_1cJDa1CDa1},
  {0x133e, GMT 0, RULE_1cJDa1CDa1},
  {0xb539, EET 12, RULE_1cJDa1CDa1},
  {0x7f42, GMT 0, RULE_1cJDa1CDa1},
  {0x39d4, TRT 18, RULE_0_________},
  {0x49c5, GMT 0, RULE_1cJDa1CDa1},
  {0xe855, EET 12, RULE_0_________},
  {0x06c8, EET 12, RULE_1cJDa1CDa1},
  {0x0a06, MSK 18, RULE_0_________},
  {0xf6a0, EET 12, RULE_1cJDa1CDa1},
  {0x0423, WET 0, RULE_1cJDa1CDa1},
  {0xac9b, CET 6, RULE_1cJDa1CDa1},
  {0xe3fe, GMT 0, RULE_1cJDa1CDa1},
  {0x4c98, CET 6, RULE_1cJDa1CDa1},
  {0x1694, CET 6, RULE_1cJDa1CDa1},
  {0x520c, CET 6, RULE_1cJDa1CDa1},
  {0x1709, EET 12, RULE_1cJDa1CDa1},
  {0x7fa8, MSK 18, RULE_0_________},
  {0xc2eb, CET 6, RULE_1cJDa1CDa1},
  {0x8066, MSK 18, RULE_0_________},
  {0xef09, EET 12, RULE_1cJDa1CDa1},
  {0xcde5, CET 6, RULE_1cJDa1CDa1},
  {0xb03f, CET 6, RULE_1cJDa1CDa1},
  {0xf5b3, CET 6, RULE_1cJDa1CDa1},
  {0x9daf, CET 6, RULE_1cJDa1CDa1},
  {0x5a5e, EET 12, RULE_1cJDa1CDa1},
  {0x986c, CET 6, RULE_1cJDa1CDa1},
  {0xbfff, SAMT 24, RULE_0_________},
  {0x5ac8, CET 6, RULE_1cJDa1CDa1},
  {0x3ad3, CET 6, RULE_1cJDa1CDa1},
  {0xcef6, SAMT 24, RULE_0_________},
  {0x8797, MSK 18, RULE_0_________},
  {0x04b8, CET 6, RULE_1cJDa1CDa1},
  {0x0421, EET 12, RULE_1cJDa1CDa1},
  {0x5af7, CET 6, RULE_1cJDa1CDa1},
  {0xfaaf, EET 12, RULE_1cJDa1CDa1},
  {0x9e02, CET 6, RULE_1cJDa1CDa1},
  {0xa6a8, EET 12, RULE_1cJDa1CDa1},
  {0x455e, SAMT 24, RULE_0_________},
  {0xf6cd, EET 12, RULE_1cJDa1CDa1},
  {0xf803, CET 6, RULE_1cJDa1CDa1},
  {0xd12c, CET 6, RULE_1cJDa1CDa1},
  {0x863a, CET 6, RULE_1cJDa1CDa1},
  {0x88ef, EET 12, RULE_1cJDa1CDa1},
  {0x2e8a, MSK 18, RULE_0_________},
  {0x3a7d, CET 6, RULE_1cJDa1CDa1},
  {0x4b6b, CET 6, RULE_1cJDa1CDa1},
  {0x891d, EET 12, RULE_1cJDa1CDa1},
  {0x5dae, CET 6, RULE_1cJDa1CDa1},
  {0xb407, GMT 0, RULE_1cJDa1CDa1},
  {0xbe77, GMT 0, RULE_1cJDa1CDa1},
  {0x7db8, UTC 0, RULE_0_________},
  {0x17ca, UTC 0, RULE_0_________},
  {0xc2be, UTC 0, RULE_0_________},
  {0xdace, UTC 0, RULE_0_________},
  {0x03fa, GMT 0, RULE_0_________},
  {0x0439, HKT 48, RULE_0_________},
  {0x6ca8, HAST - 60, RULE_0_________},
  {0x00c6, GMT 0, RULE_0_________},
  {0x78e2, EAT 18, RULE_0_________},
  {0xb657, IOT 36, RULE_0_________},
  {0x7885, ICT 42, RULE_0_________},
  {0x8937, MMT 39, RULE_0_________},
  {0x1e55, EAT 18, RULE_0_________},
  {0x73ac, MVT 30, RULE_0_________},
  {0x9714, GST 24, RULE_0_________},
  {0xdb99, MVT 30, RULE_0_________},
  {0x4e44, MUT 24, RULE_0_________},
  {0xe74e, EAT 18, RULE_0_________},
  {0xf1de, GST 24, RULE_0_________},
  {0x6e0d, IRST 21, RULE_0_________},
  {0x70b4, IST 12, RULE_1cJDa1CAw6},
  {0xe3f6, EST - 30, RULE_0_________},
  {0x3603, JST 54, RULE_0_________},
  {0xb86e, MHT 72, RULE_0_________},
  {0xd2aa, EET 12, RULE_0_________},
  {0x995c, CET 6, RULE_1cJDa1CDa1},
  {0xa9c7, PST - 48, RULE_1bKD11CD21},
  {0x0e0c, MST - 42, RULE_0_________},
  {0xcf99, CST - 36, RULE_0_________},
  {0x1302, MST - 42, RULE_0_________},
  {0x6445, MST - 42, RULE_1bKD11CD21},
  {0x4139, MST - 42, RULE_1bKD11CD21},
  {0xaf95, NZST 72, RULE_1cDD11IDa1},
  {0x7b52, CHAST 76, RULE_1cDD11IDa1},
  {0xf236, SST 78, RULE_0_________},
  {0x9825, NZST 72, RULE_1cDD11IDa1},
  {0xf529, BST 66, RULE_0_________},
  {0x1954, CHAST 76, RULE_1cDD11IDa1},
  {0x31c5, PGT 60, RULE_0_________},
  {0x2825, EAST - 36, RULE_1cDD17ID17},
  {0x5629, VUT 66, RULE_0_________},
  {0xb3ba, PHOT 78, RULE_0_________},
  {0x74a7, TKT 78, RULE_0_________},
  {0xa225, FJT 72, RULE_0_________},
  {0xf238, GILT 72, RULE_0_________},
  {0x1ecb, GALT - 36, RULE_0_________},
  {0xdce5, GAMT - 54, RULE_0_________},
  {0xbe1f, SBT 66, RULE_0_________},
  {0xe464, ChST 60, RULE_0_________},
  {0x283c, HAST - 60, RULE_0_________},
  {0x959d, HAST - 60, RULE_0_________},
  {0xf321, PHOT 78, RULE_0_________},
  {0x3f22, LINT 84, RULE_0_________},
  {0x6915, KOST 66, RULE_0_________},
  {0x06ac, MHT 72, RULE_0_________},
  {0x753d, GILT 72, RULE_0_________},
  {0x8d5a, MART - 57, RULE_0_________},
  {0x439b, SST - 66, RULE_0_________},
  {0xbdfd, NRT 72, RULE_0_________},
  {0xa980, NUT - 66, RULE_0_________},
  {0x1140, NFT 66, RULE_1cDD11JD11},
  {0x37c1, NCT 66, RULE_0_________},
  {0xd26c, SST - 66, RULE_0_________},
  {0x992b, PWT 54, RULE_0_________},
  {0xb6ed, PST - 48, RULE_0_________},
  {0x2ba8, KOST 66, RULE_0_________},
  {0xfb00, SBT 66, RULE_0_________},
  {0x3385, PGT 60, RULE_0_________},
  {0xa6ee, CKT - 60, RULE_0_________},
  {0x6b39, ChST 60, RULE_0_________},
  {0x8ebb, SST - 66, RULE_0_________},
  {0x1280, TAHT - 60, RULE_0_________},
  {0xdad9, GILT 72, RULE_0_________},
  {0xb35e, TOT 78, RULE_0_________},
  {0x0dd7, PGT 60, RULE_0_________},
  {0xa7dc, GILT 72, RULE_0_________},
  {0x2d27, GILT 72, RULE_0_________},
  {0xc879, PGT 60, RULE_0_________},
  {0xa130, CET 6, RULE_1cJDa1CDa1},
  {0xb81f, WET 0, RULE_1cJDa1CDa1},
  {0x7cf5, CST 48, RULE_0_________},
  {0x9628, PST - 48, RULE_1bKD11CD21},
  {0x62ba, TWT 48, RULE_0_________},
  {0xe5ec, KST 54, RULE_0_________},
  {0xae72, SGT 48, RULE_0_________},
  {0xe534, TRT 18, RULE_0_________},
  {0x5647, UTC 0, RULE_0_________},
  {0xd331, UTC 0, RULE_0_________},
  {0xf25c, AKST - 54, RULE_1bKD11CD21},
  {0xeba8, HAST - 60, RULE_1bKD11CD21},
  {0x0a14, MST - 42, RULE_0_________},
  {0x4ca7, CST - 36, RULE_1bKD11CD21},
  {0xaa1f, EST - 30, RULE_1bKD11CD21},
  {0x426f, EST - 30, RULE_1bKD11CD21},
  {0x5122, HAST - 60, RULE_0_________},
  {0x516d, CST - 36, RULE_1bKD11CD21},
  {0x401e, EST - 30, RULE_1bKD11CD21},
  {0x7a4b, MST - 42, RULE_1bKD11CD21},
  {0x3b43, PST - 48, RULE_1bKD11CD21},
  {0xac4f, SST - 66, RULE_0_________},
  {0xb871, UTC 0, RULE_0_________},
  {0xcb07, MSK 18, RULE_0_________},
  {0x96f9, WET 0, RULE_1cJDa1CDa1},
  {0x110d, UTC 0, RULE_0_________},
  {00, UTC 0, RULE_0_________}
};

const struct ntz_iana *ntz_find_tz_key( uint16_t key )
{
  int i;
  for ( i = 0; ntz_db[i].iana_name_hash != 0; i++ )
    if ( ntz_db[i].iana_name_hash == key )
      break;
  if ( ntz_db[i].iana_name_hash != 0 )
    return ( &ntz_db[i] );
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
  uint16_t key = hash[0] << 8 | hash[1];
  return ( ntz_find_tz_key( key ) );
}

#endif

#endif
