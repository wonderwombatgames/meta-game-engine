/**
  * helper functions to abstract command interface
  * usually this is implemented on system and managers
  */

#ifndef COMMAND_INTERFACE_HPP
#define COMMAND_INTERFACE_HPP

#include "utils_types.hpp"

namespace Engine
{
  // using namespace std;
  using namespace Utils;

  using  CommandType = uint16_t;
  using  CommandOp   = uint16_t;

  struct Args
  {
      enum{CHAR8, INT64, UINT64, DOUBLE, STRING_PTR} tag;
      union
      {
          char c[8];
          int64_t i;
          uint64_t u;
          double d;
          String * s;
      } data;
  };


  struct CommandMsg
  {
    CommandType type;
    CommandOp op;
    Args arguments[8];
  };


  class ICommand
  {
  public:
    friend class CommandRouter;
    virtual ~ICommand(){}

    const CommandType & getType() const
    {
      return _type;
    }

  protected:
    CommandType _type;

    void run(const CommandMsg & cmd)
    {
      onCommand(cmd);
    }
    virtual void onCommand(const CommandMsg & cmd) = 0;
  };


} // end namespace Engine

#endif // COMMAND_INTERFACE_HPP
