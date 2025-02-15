#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include "../osscan.h"

struct expr_test {
  const char *expr;
  const char *val;
  bool result;
};
const struct expr_test tests[] = {
  {"M123", "M123", true},
  {"M123", "M120", false},
  {"M123", "0M123", false},
  {"M123", "M1234", false},
  {"A", "A", true},
  {"12345678", "12345678", true},
  {"123456780", "123456789", false},
  {"123456780", "123456780", true},
  {"123456780", "12345678001", false},
  {">A", "B", true},
  {">B", "A", false},
  {">B0", "C0", true},
  {">C0", "C0", false},
  {">C0", "B0", false},
  {">B0", "100", true},
  {"<B0", "100", false},
  {">10", "B00", true},
  {"<10", "1", true},
  {"<10", "9", true},
  {"A|B", "B", true},
  {"5-B", "B", true},
  {"5-B", "5", true},
  {"5-B", "6", true},
  {"5-B", "A", true},
  {"5-B", "4", false},
  {"5-B", "C", false},
  {"5-B", "51", false},
  {"5-B", "B2", false},
  {"5-B", "32", false},
  {"5-B", "9B", false},
  {"M123|B", "B", true},
  {"M123|B", "M123", true},
  {"B|M123", "B", true},
  {"B|M123", "M123", true},
  {"<A", "9", true},
  {"", "1", false},
  {"", "anything", false},
  {"", "1", false},
  {"|5", "5", true},
  {"5|", "5", true},
  {"|M", "M", true},
  {"M|", "M", true},
  {"|5", "1", false},
  {"|5", "M", false},
  {"|5", "M5", false},
  {"|5", "5M", false},
  {"5|", "1", false},
  {"5|", "M", false},
  {"5|", "M5", false},
  {"5|", "5M", false},
  {"|M", "1", false},
  {"|M", "N", false},
  {"|M", "M5", false},
  {"|M", "5M", false},
  {"M|", "1", false},
  {"M|", "N", false},
  {"M|", "M5", false},
  {"M|", "5M", false},
  {"A|B", "AB", false},
  {"A|B", "BA", false},
  {"A|B", "", false},
  {"A", "", false},
  {"<A", "", false},
  {">A", "", false},
  {"1-9", "", false},
  {"M[1-9]", "M2", true},
  {"M[<5]S", "M2S", true},
  {"M[>A]S", "MFS", true},
  {"M[>A7]S", "MA9S", true},
  {"M[>A7]S", "M111S", true},
  {"M[1]S", "M1S", true},
  {"M[1|A]S", "M1S", true},
  {"M[1|A]S", "MAS", true},
  {"M[1-9]", "MA", false},
  {"M[<5]S", "M9S", false},
  {"M[>A]S", "M2S", false},
  {"M[1]S", "M3S", false},
  {"M[1|A]S", "M1AS", false},
  {"M[1|A]S", "M9S", false},
  {"M[1-9]", "M2A", false},
  {"M[1-9]", "M2T", false},
  {"M[<5]S", "M2BS", false},
  {"M[<5]S", "M2B", false},
  {"M[>A7]S", "MFS", false},
  {"M[>A7]S", "MA6S", false},
  {"[1-9]", "2", true},
  {"[<5]S", "2S", true},
  {"[>A]S", "FS", true},
  {"[>A7]S", "A9S", true},
  {"[>A7]S", "111S", true},
  {"[1]S", "1S", true},
  {"[1|A]S", "1S", true},
  {"[1|A]S", "AS", true},
  {"[1-9]", "A", false},
  {"[<5]S", "9S", false},
  {"[>A]S", "2S", false},
  {"[1]S", "3S", false},
  {"[1|A]S", "1AS", false},
  {"[1|A]S", "9S", false},
  {"[1-9]", "2A", false},
  {"[1-9]", "2T", false},
  {"[<5]S", "2BS", false},
  {"[<5]S", "2B", false},
  {"[>A7]S", "FS", false},
  {"[>A7]S", "A6S", false},
  {"", "", true}
};

int main(int argc, char **argv)
{
  size_t num_tests = sizeof(tests) / sizeof(expr_test);
  size_t num_fail = 0;
  for (size_t i=0; i < num_tests; i++) {
    const char *val = tests[i].val;
    const char *expr = tests[i].expr;
    bool expected = tests[i].result;
    std::cout << i << '\r';
    if (expected != expr_match(val, 0, expr, 0, strchr(expr, '['))) {
      std::cout << "FAIL test " << i << ": " << val <<
      (expected ? " nomatch " : " badmatch ") << expr << std::endl;
      num_fail++;
    }
  }
  std::cout << "Ran " << num_tests << " tests. " << num_fail << " failures." << std::endl;
  return num_fail;
}
