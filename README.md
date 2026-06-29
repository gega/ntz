# ntz

Nano IANA timezone library for embedded systems.

`ntz` is a single-header, heap-free timezone library that provides compact support for nearly 600 IANA timezones, including automatic daylight saving time (DST) handling.

The library is designed for microcontrollers, wearables, IoT devices, and other memory-constrained systems that need local time conversion from Unix timestamps.

## Features

* Supports ~600 IANA timezone identifiers
* Automatic DST calculation
* Single-header library (`#define NTZ_IMPLEMENTATION`)
* No dynamic memory allocation
* No external dependencies
* Small footprint:

  * ~2.7 KB database without timezone abbreviations
  * ~4.4 KB database with abbreviations enabled
* Optimized for embedded firmware

## Quick Start

```c
#define NTZ_IMPLEMENTATION
#include "ntz.h"

const struct ntz_iana *tz =
    ntz_find_tz_name("Europe/Budapest", 15);

struct ntz_tm tm;

ntz_epoch_to_localtime(
    1782345678,
    &tm,
    tz
);
```

## Configuration

```c
#define NTZ_ABBREV   0   /* disable timezone abbreviations */
#define NTZ_NAME_API 1   /* enable name lookup */
#include "ntz.h"
```

### NTZ_ABBREV

Enables timezone abbreviation storage (CET, PST, JST, ...).

* `0` = disabled (smaller database)
* `1` = enabled

### NTZ_NAME_API

Controls whether timezone lookup by name is included.

* `0` = hash lookup only
* `1` = hash and name lookup

## Data Structures

### `struct ntz_iana`

Represents a timezone entry.

```c
struct ntz_iana
{
    uint16_t iana_name_hash;
    int8_t   offset;
    uint8_t  dst_rule;
};
```

Contains the timezone hash, standard UTC offset (in 10 minutes units), and DST rule identifier.
The database generator verifies that all included timezone names produce unique 16-bit hashes.

### `struct ntz_tm`

Compact calendar time structure.

```c
struct ntz_tm
{
    int16_t tm_year; /* years since 1900 */
    int8_t  tm_mon;  /* 0-11 */
    int8_t  tm_mday; /* 1-31 */
    int8_t  tm_hour; /* 0-23 */
    int8_t  tm_min;  /* 0-59 */
    int8_t  tm_sec;  /* 0-59 */
};
```
_NOTE:_ Same convention as C's struct tm. Year 2026 is stored as 126.

## API Reference

### `ntz_find_tz_key()`

```c
const struct ntz_iana *
ntz_find_tz_key(uint16_t key);
```

Find a timezone using its 16-bit IANA name hash.

Returns a pointer to the matching timezone entry or `NULL` if not found.

---

### `ntz_find_tz_name()`

```c
const struct ntz_iana *
ntz_find_tz_name(const char *name, int len);
```

Find a timezone by its IANA name closed with '\n'.
_NOTE:_ Invalid names can result in valid hash match! Use only valid IANA
names!

Example:

```c
ntz_find_tz_name("America/New_York\n", 17);
```

Available only when `NTZ_NAME_API` is enabled.

---

### `ntz_day_of_week()`

```c
int ntz_day_of_week(
    int year,
    int month,
    int day
);
```

Calculates the day of week for a calendar date.

Returns values `1..7`:
1 = Sunday
2 = Monday
...
7 = Saturday

---

### `ntz_epoch_to_tm()`

```c
int ntz_epoch_to_tm(
    int64_t epoch,
    struct ntz_tm *tm,
    const struct ntz_iana *iana
);
```

Converts a Unix timestamp into local standard time using the timezone's base UTC offset.

DST is not applied.

---

### `ntz_get_dst_offset_min()`

```c
int ntz_get_dst_offset_min(
    int64_t epoch,
    const struct ntz_iana *iana
);
```

Returns the DST adjustment in minutes for a given Unix timestamp. Expects local standard time produced by ntz_epoch_to_tm().

Typically returns `0` or `1`.

---

### `ntz_dst_offset_min()`

```c
int ntz_dst_offset_min(
    struct ntz_tm *tm,
    const struct ntz_iana *iana
);
```

Determines the DST offset from an already converted calendar time.

---

### `ntz_epoch_to_localtime()`

```c
int ntz_epoch_to_localtime(
    int64_t epoch,
    struct ntz_tm *tm,
    const struct ntz_iana *iana
);
```

Converts a Unix timestamp into fully adjusted local time, including DST.

This is the function most applications should use.

## Typical Workflow

```c
const struct ntz_iana *tz =
    ntz_find_tz_name("Europe/Budapest", 15);

struct ntz_tm tm;

ntz_epoch_to_localtime(
    unix_epoch,
    &tm,
    tz
);
```

## Generator Tool

The repository includes a helper script that updates `ntz.h` with the latest IANA Time Zone Database. The script downloads the current tzdata release directly from IANA, converts it into the compact internal representation used by NTZ, regenerates all lookup tables, and replaces the generated sections inside `ntz.h` while leaving the manually written code untouched.

### Usage

```sh
.ntz_generate_tzdata.sh path/to/ntz.h
```

The script expects a single argument: the path to the `ntz.h` file to update. If the generation completes successfully, the file is modified in place.

### What the script does

* Downloads the latest IANA Time Zone Database.
* Converts the IANA data to iCalendar format using `vzic` (from the `libical` submodule).
* Generates the compact DST rule database used by NTZ.
* Extracts timezone abbreviations and standard UTC offsets.
* Computes a collision-free 16-bit hash mapping for all IANA timezone names.
* Regenerates all generated lookup tables and definitions.
* Replaces the corresponding generated blocks inside `ntz.h`.

### Requirements

Before running the generator:

* Initialize and update the repository submodules (`libical` is required).
* Ensure common Unix tools are available, including `bash`, `curl`, `awk`, `perl`, `sed`, `find`, `sort`, `uniq`, `cut`, `dos2unix`, `make`, and either `sha1sum` (default) or another compatible hash utility if the script is modified.

The first run will automatically build `vzic` if it has not already been compiled.


The resulting `tm` structure contains the local date and time with timezone and DST corrections applied.
