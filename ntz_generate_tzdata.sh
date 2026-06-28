#!/bin/bash

HASH="sha1sum"

NTZ_H=$(realpath $1)

if [ ! -f "$NTZ_H" ]; then
  echo "Usage: $0 ntz.h"
  echo "  update ntz.h with the newest IANA TZ dataset"
  exit 1
fi

# env
if [ ! -d libical/vzic/ ]; then
  echo "Update submodules to get libical"
  exit 1
fi
ICAL2GG=$(realpath tools/ical2gg.pl)

replace_generated_block()
{
    local file="$1"
    local marker="$2"
    local text="$3"
    awk -v marker="$marker" -v text="$text" '
        BEGIN {
            start = "{" marker
            end   = "}" marker
            inside = 0
        }
        index($0, start) {
            print
            printf "%s", text
            if (length(text) && substr(text, length(text), 1) != "\n")
                print ""
            inside = 1
            next
        }
        index($0, end) {
            inside = 0
            print
            next
        }
        !inside {
            print
        }
    ' "$file" > "$file.tmp" &&
    mv "$file.tmp" "$file"
}

# compile vzic
if [ ! -f libical/vzic/vzic ]; then
  pushd libical/vzic
  make
  popd
fi
VZIC=$(realpath libical/vzic/vzic)

TMP=$(mktemp -d)
pushd $TMP

echo "Downloading iana dataset"
rm -rf tzdata
mkdir tzdata
cd tzdata
curl -LO https://data.iana.org/time-zones/tzdata-latest.tar.gz 2>/dev/null
tar xzf tzdata-latest.tar.gz
cd ..

# data conv
echo "Converting IANA to ical"
rm -rf ical
mkdir ical
"$VZIC" --olson-dir tzdata/ --output-dir ical

echo "Converting ical to gg dst rules"
rm -rf out
mkdir out
find ical -name "*ics"|awk '{print "cat " $1 " | \"'$ICAL2GG'\" > out/HHH" $1 }'|sed 's#HHHical/\(.*\).ics$#\1#g'|perl -pe 's{(?<=out/).*}{$& =~ tr{/}{@}r}e'|bash
cat out/* |sort |uniq >RULES

echo "Extracting abbrevated names"
rm -rf abr
mkdir abr
ls -1 out|awk '{print "cat ical/$(echo " $1 "|tr \"@\" \"/\").ics|grep TZNAME|awk \"{ print length, \\$0 }\" | sort -n -s | cut -d\" \" -f2-|head -1|cut -d: -f2 >abr/" $1}'|bash

echo "Generating name hashes"
rm -rf hash
mkdir hash
# try all offsets from -c1-4 -c37-40 hashoffs=0..36 cutoffs=hashoffs+1 .. hashoffs+4
hashmax=36
hashoffs=0
while true; do
  echo "  trying hash '$HASH' offset $hashoffs"
  cutoff1=$((hashoffs * 1 + 1))
  cutoff2=$((hashoffs * 1 + 4))
  ls -1 out|awk '{print "echo -en \"$(echo " $1 "|tr \"@\" \"/\")\" |'$HASH'|cut -c'$cutoff1'-'$cutoff2'"}'|bash|sort | uniq -c|awk '{print $1}'|grep -qv 1
  ST=$?
  [[ $ST -ne 0 ]] && break
  hashoffs=$((hashoffs + 1))
  [[ $hashoffs -gt $hashmax ]] && break
done
if [ $hashoffs -gt $hashmax ]; then
  echo "ERROR: hash collision! choose another hash!"
  exit 1
fi
ls -1 out|awk '{print "echo -en \"$(echo " $1 "|tr \"@\" \"/\")\" |'$HASH'|cut -c'$cutoff1'-'$cutoff2' > hash/" $1}'|bash

echo "Calculating TZ offsets"
rm -rf offs
mkdir offs
ls -1 out|dos2unix|awk '{p=$1; gsub("@","/",p); print p ".ics >offs/o" $1}'|sed -e 's#^#awk "/BEGIN:STANDARD/ { std=1 } /END:STANDARD/ { std=0 } std \&\& /^TZOFFSETTO:/ { print substr(\\\$0,12); exit }" ical/#g'|bash
ls -1 out|dos2unix|awk '{print $1 " >offs/m" $1}'|sed -e "s#^#awk -F '' '{ m=(\$2*10*60+\$3*60+\$4*10+\$5)/1; print \$1 m;}' offs/o#g"|bash

# code gen
echo "Generating code snippets"
echo -e "#define NTZ_HASH_SHIFT (${hashoffs})\n#define NTZ_HASH \"${HASH}\"" >defines
echo "#define NTZ_TZDATA_VERSION \"$(cat tzdata/version)\"" >>defines
cat RULES|tr '@-' '__'|awk '{print "  RULE_" $1 ","}' >enum_ntz_rulelist
paste enum_ntz_rulelist RULES|awk '{print "  [" $1 "] = \"" $2 "\""}'|tr -d ','|sed 's/$/,/g' >const_char_ntz_rules
rm -f RULES
cat offs/m*|sort -n|uniq|dos2unix|awk '{o=$1; gsub("+","p",o); gsub("-","m",o); print "  [NTZ_OFFSET_" o "] = " $1 "," }' >ntz_minute_offsets
cat offs/m*|sort -n|uniq|dos2unix|awk '{o=$1; gsub("+","p",o); gsub("-","m",o); print "#define NTZ_OFFSET_" o " " NR-1 }' >ntz_minute_offset_defines
cat abr/*  |sort   |uniq|dos2unix|sed -e 's/$/\",/g' -e 's/^/  \"/g' >const_char_ntz_abbrev
cat abr/*  |sort   |uniq|dos2unix|tr '+-' 'pm'|cat -n|awk '{print "#define " $2 " " $1-1 ","}' >define_abbrev
cat abr/*  |sort   |uniq|dos2unix|tr '+-' 'pm'|cat -n|awk '{print "#define " $2 " " }' >define_empty_abbrev
ls -1 out|awk '{print "echo \"  {0x$(cat hash/" $1 "), $(cat abr/" $1 "|dos2unix|tr \"+-\" \"pm\") NTZ_OFFSET_$(cat offs/m" $1 "|tr \"+-\" \"pm\"), RULE_$(cat out/" $1 "|tr \"@-\" \"__\")},\t// $(echo \"" $1 "\"|tr \"@\" \"/\")\""}'|bash|sort > const_struct_ntz_iana_ntz_db

# Template code to src
value=$(<defines)
replace_generated_block "$NTZ_H" "ntz::defines" "$value"
value=$(<enum_ntz_rulelist)
replace_generated_block "$NTZ_H" "ntz::enum_ntz_rulelist" "$value"
value=$(<const_char_ntz_rules)
replace_generated_block "$NTZ_H" "ntz::const_char_ntz_rules" "$value"
value=$(<const_char_ntz_abbrev)
replace_generated_block "$NTZ_H" "ntz::const_char_ntz_abbrev" "$value"
value=$(<define_abbrev)
replace_generated_block "$NTZ_H" "ntz::define_abbrev" "$value"
value=$(<define_empty_abbrev)
replace_generated_block "$NTZ_H" "ntz::define_empty_abbrev" "$value"
value=$(<const_struct_ntz_iana_ntz_db)
replace_generated_block "$NTZ_H" "ntz::const_struct_ntz_iana_ntz_db" "$value"
value=$(<ntz_minute_offsets)
replace_generated_block "$NTZ_H" "ntz::ntz_minute_offsets" "$value"
value=$(<ntz_minute_offset_defines)
replace_generated_block "$NTZ_H" "ntz::ntz_minute_offset_defines" "$value"

# cleanup
#rm -rf $TMP
popd
