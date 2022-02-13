# String slice STB-Style Library in C

## How to:

For including just the definitions, do:
```C
#include "sl.h"
```

If you want to include the Implementations as well, use
```c
#define SL_IMPLEMENTATION
#include "sl.h"
```

in your includes.

Current functions are

```c
typedef struct {
  const char *start;
  int len;
} SL;

#define SL_AT(sslice, idx) sslice.start[idx]
#define SL_FP(sslice) sslice.len, sslice.start
#define SL_NEW(content) (SL) { .start = content, .len = strlen(content) }
#define SL_NWL(content, size)\ { .start = content, .len = size }

SL SL_trim_len(SL to_trim, int len);
SL SL_chop_delim(SL text, char delim);
SL SL_chop_delim_right(SL text, char delim);
SL SL_chop_slice(SL text, SL delim);
SL SL_chop_slice_right(SL text, SL delim);
SL SL_trim_whitespace(SL line);
SL SL_trim_whitespace_right(SL line);
SL SL_chop_line(SL text);
bool SL_begins_with(SL slice, SL begin);
bool SL_ends_with(SL slice, SL end);
bool SL_eq(SL a, SL b);
```
