#ifndef SL_H
#define SL_H

#include "stdbool.h"
#include "string.h"

typedef struct {
  const char *start;
  int len;
} SL; // Slice

#define SL_AT(sslice, idx) sslice.start[idx]
#define SL_FP(sslice) sslice.len, sslice.start
#define SL_NEW(content)                                                    \
  (SL) { .start = content, .len = strlen(content) }
#define SL_NWL(content, size)                                              \
  { .start = content, .len = size }

SL SL_trim_len(SL to_trim, int amount);
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

#ifdef SL_IMPLEMENTATION // INCLUDE IMPLEMENTATIONS
#include "stdio.h"
#include "stdlib.h"

SL SL_trim_len(SL to_trim, int amount) {
  if (amount > 0) {
    return (SL){&SL_AT(to_trim, amount), to_trim.len - amount};
  } else {
    return (SL){&SL_AT(to_trim, 0), to_trim.len + amount};
  }
}

SL SL_chop_delim(SL text, char delim) {
  int i = 0;
  while ((i < text.len) && (SL_AT(text, i) != delim)) {
    i++;
  }
  if (i == text.len) i = -1;

  return (SL){
      .start = &SL_AT(text, 0),
      .len = i,
  };
}

SL SL_chop_delim_right(SL text, char delim) {
  int i = text.len - 1;
  while ((i >= 0) && (SL_AT(text, i) != delim)) {
    i--;
  }
  if (i == text.len) i = -1; 

  return (SL) {
      .start = &SL_AT(text, i + 1),
      .len = i == -1 ? i : text.len - i - 1,
  };
}

SL SL_chop_slice(SL text, SL delim) {
  int i = 0;
  int j = 0;
  while (i < text.len && j < delim.len) {
    // This loop breaks if the number of matching chars is delim.len
    if (SL_AT(text, i) == SL_AT(delim, j)) {
      j++;
      i++;
    } else {
      if (j == 0) i++;
      else j = 0;
    }
  }

  return (SL) {
    .start = &SL_AT(text, 0),
    .len = j == 0 ? -1 : i - delim.len,
  };
}

SL SL_chop_slice_right(SL text, SL delim) {
  int i = text.len - 1;
  int j = delim.len - 1;
  while (i >= 0 && j >= 0) {
    // This loop breaks if the number of matching chars is delim.len
    if (SL_AT(text, i) == SL_AT(delim, j)) {
      j--;
      i--;
    } else {
      if (j == delim.len - 1) i--;
      else j = delim.len - 1;
    }
  }

  return (SL) {
    .start = &SL_AT(text, i + delim.len + 1),
    .len = j == 0 ? -1 : text.len - i - delim.len - 1,
  };
}

SL SL_chop_line(SL text) { return SL_chop_delim(text, '\n'); }

SL SL_trim_whitespace(SL line) {
  size_t cur_idx = 0;
  while (SL_AT(line, cur_idx) == ' ') {
    cur_idx++;
  }

  return (SL){
      .start = &SL_AT(line, cur_idx),
      .len = line.len - cur_idx,
  };
}

SL SL_trim_whitespace_right(SL line) {
  size_t cur_idx = 1;
  while (SL_AT(line, line.len - cur_idx) == ' ') {
    cur_idx++;
  }

  return (SL){
      .start = &SL_AT(line, 0),
      .len = line.len - cur_idx + 1,
  };
}

bool SL_begins_with(SL slice, SL begin) {
  if (slice.len < begin.len) return false;

  for (int idx = 0; idx < begin.len; idx++) {
    if (SL_AT(slice, idx) != SL_AT(begin, idx)) {
      return false;
    }
  }
  return true;
}

bool SL_ends_with(SL slice, SL end) {
  if (slice.len < end.len) return false;

  for (int idx = 0; idx < end.len; idx++) {
    if (SL_AT(slice, slice.len - end.len + idx) != SL_AT(end, idx)) {
      return false;
    }
  }
  return true;
}

bool SL_eq(SL a, SL b) {
  if (a.len != b.len) return false;

  for (int i = 0; i < a.len; i++) {
    if (a.start[i] != b.start[i])
      return false;
  }
  return true;
}

#endif // SL_IMPLEMENTATION

#endif // SL_H
