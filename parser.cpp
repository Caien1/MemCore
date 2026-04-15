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

  AGGREGATE_BULK_STRINGS,  // $
  AGGREGATE_ARRAY,         // *
  AGGREGATE_BULK_ERROR,    // !
  AGGREGATE_BULK_VERBATIM, // =
  AGGREGATE_MAP,           // %
  AGGREGATE_SET,           // ~
  AGGREGATE_ATTRIBUTE,     // |
  AGGREGATE_PUSH           // >

};
// TODO add funtion pointer directly mabybe or stay for switch for flexiblity
constexpr std::array<RespDataType, 256> makeSimpleTable() {
  std::array<RespDataType, 256> t{};
  t['+'] = SIMPLE_STRING;
  t['-'] = SIMPLE_ERROR;
  t[':'] = SIMPLE_INTEGER;
  t['_'] = SIMPLE_NULL;
  t['#'] = SIMPLE_BOOL;
  t[','] = SIMPLE_DOUBLE;
  t['('] = SIMPLE_BIGNUMBER;
  t['$'] = AGGREGATE_BULK_STRINGS;
  t['*'] = AGGREGATE_ARRAY;
  t['!'] = AGGREGATE_BULK_ERROR;
  t['='] = AGGREGATE_BULK_VERBATIM;
  t['%'] = AGGREGATE_MAP;
  t['~'] = AGGREGATE_SET;
  t['|'] = AGGREGATE_ATTRIBUTE;
  t['>'] = AGGREGATE_PUSH;
  return t;
}
constexpr auto simpleDispatchTable = makeSimpleTable();

// TODO Figure out a proper method to parse bufers from network
void findRequestType(char bufferChar) {

  RespDataType type = simpleDispatchTable[bufferChar];

  switch (type) {

  case RespDataType::SIMPLE_STRING:
    // do action

    break;

  case RespDataType::SIMPLE_ERROR:

    // do action
    break;

  case RespDataType::SIMPLE_INTEGER:

    // do action
    break;

  case RespDataType::SIMPLE_NULL:
    // do action
    break;

  case RespDataType::SIMPLE_BOOL:
    // do action
    break;

  case RespDataType::SIMPLE_DOUBLE:
    // do action
    break;
  case RespDataType::SIMPLE_BIGNUMBER:
    // do action
    break;

  case RespDataType::AGGREGATE_ARRAY:

    break;
  case RespDataType::AGGREGATE_ATTRIBUTE:

    break;
  case RespDataType::AGGREGATE_BULK_ERROR:

    break;
  case RespDataType::AGGREGATE_MAP:

    break;
  case RespDataType::AGGREGATE_BULK_STRINGS:

    break;
  case RespDataType::AGGREGATE_PUSH:

    break;
  case RespDataType::AGGREGATE_SET:

    break;
  case RespDataType::AGGREGATE_BULK_VERBATIM:

    break;

  default:

    break;
  }
}
