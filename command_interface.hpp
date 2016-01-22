/**
  * helper functions to abstract command interface
  * usually this is implemented on system and managers
  */

#ifndef COMMAND_INTERFACE_HPP
#define COMMAND_INTERFACE_HPP

#include "utils_types.hpp"

namespace W2E
{
// using namespace std;
using namespace Utils;

enum eArgTag
{
  CHAR8,
  INT64,
  UINT64,
  DOUBLE,
  STRINGPTR_
};

struct Args
{
  eArgTag tag;
  union {
    char c[8];
    i64 i;
    u64 u;
    f64 d;
    String* s;
  } data;
};

struct CommandMsg
{
  CommandType type;
  CommandOp op;
  Args arguments[8];
};

class ICommandee
{
public:
  friend class CommandDispatcher;
  virtual ~ICommandee() {}

  const CommandType& getType() const { return type_; }

protected:
  CommandType type_;

  void run(const CommandMsg& cmd) { onCommand(cmd); }
  virtual void onCommand(const CommandMsg& cmd) = 0;
};

} // end namespace W2E

#endif // COMMAND_INTERFACE_HPP
