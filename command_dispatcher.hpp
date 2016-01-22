/**
  * singleton routes commands to subscribers.
  * the addresses for the commands are defined by their subscription key
  */

#ifndef COMMAND_DISPATCHER_HPP
#define COMMAND_DISPATCHER_HPP

#include "command_interface.hpp"

namespace W2E
{
using namespace std;

class CommandDispatcher
{
public:
  CommandDispatcher(CommandDispatcher& other) = delete;
  ~CommandDispatcher();

  CLASS_METHOD CommandDispatcher* instance();

  ErrorCode subscribe(CommandType type, ICommandee* cmd);
  ErrorCode unSubscribe(const CommandType type, const ICommandee* cmd);
  void dispatch(const CommandMsg& msg);

protected:
  CommandDispatcher();
  GLOBAL HashMultiMap< CommandType, ICommandee* > subscribers;
};

} // end namespace W2E

#endif // COMMAND_DISPATCHER_HPP
