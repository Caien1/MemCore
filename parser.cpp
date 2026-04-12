#include <array>
#include <cmath>
#include <cstdint>
enum RespDataType : uint8_t {
  SIMPLE_STRING,    //+
  SIMPLE_ERROR,     //-
  SIMPLE_INTEGER,   //:
  SIMPLE_NULL,      //_(underscore)
  SIMPLE_BOOL,      // #
  SIMPLE_DOUBLE,    //,
  SIMPLE_BIGNUMBER, //(

};

// TODO add AggregateType later
// enum AggregateType : uint8_t { STE, EW };

constexpr std::array<RespDataType, 256> makeSimpleTable() {
  std::array<RespDataType, 256> t{};
  t['+'] = SIMPLE_STRING;
  t['-'] = SIMPLE_ERROR;
  t[':'] = SIMPLE_INTEGER;
  t['_'] = SIMPLE_NULL;
  t['#'] = SIMPLE_BOOL;
  t[','] = SIMPLE_DOUBLE;
  t['('] = SIMPLE_BIGNUMBER;
  return t;
}
constexpr auto simpleDispatchTable = makeSimpleTable();

// TODO do some parsing union u {

//
