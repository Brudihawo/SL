#include "stdio.h"

#define SL_IMPLEMENTATION
#include "sl.h"

#define RED   "\033[1;31m"
#define RED_N "\033[0;31m"
#define GRN   "\033[1;32m"
#define GRN_N "\033[0;32m"
#define RST   "\033[0m"

#define TEST_PASS(name) \
  fprintf(stdout, GRN "PASSED: %s in %s %s:%i"RST"\n",\
      name, __func__, __FILE__, __LINE__)
#define TEST_FAIL(name) \
  fprintf(stdout, RED "FAILED: %s in %s %s:%i"RST"\n",\
      name, __func__, __FILE__, __LINE__)

#define TEST_FAIL_MSG(name, msg, ...) \
  fprintf(stdout, RED "FAILED: %s in %s %s:%i"RST"\n" msg "\n",\
      name, __func__, __FILE__, __LINE__, __VA_ARGS__)

#define TEST(name, cond) cond; cond ? TEST_PASS(name) : TEST_FAIL(name);

#define TEST_SL_EQ(arg, expected, name) SL_eq(arg, expected); \
  SL_eq(arg, expected) ? TEST_PASS(name) : TEST_FAIL_MSG(name, "-> Expected '%.*s' but got '%.*s'", expected.len, expected.start, arg.len, arg.start)


bool test_trim_len(void) {
  bool suc1 = TEST_SL_EQ(SL_trim_len(SL_NEW("123456"), 3), SL_NEW("456"),
                             "Positive amount");
  bool suc2 = TEST_SL_EQ(SL_trim_len(SL_NEW("123456"), -3), SL_NEW("123"),
                             "Negative amount");
  return suc1 && suc2;
}

bool test_chop_delim(void) {
  SL result = SL_chop_delim(SL_NEW("abc def"), ' ');
  bool ret = TEST_SL_EQ(result, SL_NEW("abc"), "Default");
  return ret;
}

bool test_chop_delim_right(void) {
  SL result = SL_chop_delim_right(SL_NEW("abc defg"), ' ');
  bool ret = TEST_SL_EQ(result, SL_NEW("defg"), "Default");
  return ret;
}

bool test_chop_slice(void) {
  SL result1 = SL_chop_slice(SL_NEW("test test // comment"), SL_NEW("//"));
  SL exp1 = SL_NEW("test test ");
  bool ret1 = TEST_SL_EQ(result1, exp1, "Default");

  SL result2 = SL_chop_slice(SL_NEW("xx // x test test // comment2"), SL_NEW("//"));
  SL exp2 = SL_NEW("xx ");
  bool ret2 = TEST_SL_EQ(result2, exp2, "Length 2");

  SL result3 = SL_chop_slice(SL_NEW("x test test comment2//"), SL_NEW("//"));
  SL exp3 = SL_NEW("x test test comment2");
  bool ret3 = TEST_SL_EQ(result3, exp3, "End of Line");
  return ret1 && ret2 && ret3;
}

bool test_chop_slice_right(void) {
  SL result1 = SL_chop_slice_right(SL_NEW("x test test // comment"), SL_NEW("//"));
  SL exp1 = SL_NEW(" comment");
  bool ret1 = TEST_SL_EQ(result1, exp1, "Length 1");

  SL result2 = SL_chop_slice_right(SL_NEW("xx // x test test // comment2"), SL_NEW("//"));
  SL exp2 = SL_NEW(" comment2");
  bool ret2 = TEST_SL_EQ(result2, exp2, "Length 2");

  SL result3 = SL_chop_slice_right(SL_NEW("// x test test comment2"), SL_NEW("//"));
  SL exp3 = SL_NEW(" x test test comment2");
  bool ret3 = TEST_SL_EQ(result3, exp3, "Begin of Line");
  return ret1 && ret2 && ret3;
}

bool test_trim_whitespace(void) {
  SL result = SL_trim_whitespace(SL_NEW("   aaaaaa"));
  bool ret = TEST_SL_EQ(result, SL_NEW("aaaaaa"), "Default");
  return ret;
}

bool test_trim_whitespace_right(void) {
  SL result = SL_trim_whitespace_right(SL_NEW("   aaaaaa    "));
  bool ret = TEST_SL_EQ(result, SL_NEW("   aaaaaa"), "Default");
  return ret;
}

bool test_chop_line(void) {
  SL result = SL_chop_line(SL_NEW("This is a line.\nThis is another.\n"));
  bool ret = TEST_SL_EQ(result, SL_NEW("This is a line."), "Default");
  return ret;
}

bool test_begins_with(void) {
  bool ret1 = TEST("Positive", SL_begins_with(SL_NEW("abcd"), SL_NEW("ab")));
  bool ret2 = TEST("Negative", !SL_begins_with(SL_NEW("abcd"), SL_NEW("balls")));
  return ret1 && ret2;
}

bool test_ends_with(void) {
  bool ret1 = TEST("Positive", SL_ends_with(SL_NEW("abcd"), SL_NEW("cd")));
  bool ret2 = TEST("Negative", !SL_ends_with(SL_NEW("abcd"), SL_NEW("balls")));
  return ret1 && ret2;
}

bool test_sslice_eq(void) {
  bool ret1 = TEST("Equality", SL_eq(SL_NEW("test"), SL_NEW("test")));
  bool ret2 = TEST("Inequality", !SL_eq(SL_NEW("test"), SL_NEW("te2st")));
  return ret1 && ret2;
}

typedef bool(*test_func)(void);

int main(void) {
  test_func funcs[] = {
    &test_sslice_eq,
    &test_trim_len,
    &test_chop_delim,
    &test_chop_delim_right,
    &test_chop_slice,
    &test_chop_slice_right,
    &test_trim_whitespace,
    &test_trim_whitespace_right,
    &test_chop_line,
    &test_begins_with,
    &test_ends_with,
  };

  for (int i = 0; i < sizeof(funcs) / sizeof(funcs[0]); i++) {
    if (!funcs[i]()) break;
  }
}
