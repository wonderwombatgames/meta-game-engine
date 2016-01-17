/**
  * singleton routes commands to subscribers.
  * the addresses for the commands are defined by their subscription key
  */

#ifndef COMMAND_ROUTER_HPP
#define COMMAND_ROUTER_HPP


#include "command_interface.hpp"

namespace W2E
{
  using namespace std;

  class CommandRouter
  {
  public:
    CommandRouter(CommandRouter & other) = delete;
    ~CommandRouter();

    CLASS_METHOD CommandRouter * instance();

    ErrorCode subscribe(CommandType type, ICommand * cmd);
    ErrorCode unSubscribe(const CommandType type, const ICommand * cmd);
    void dispatch(const CommandMsg & msg);

  protected:
    CommandRouter();
    GLOBAL HashMultiMap< CommandType, ICommand * > subscribers;
  };

} // end namespace W2E

#endif // COMMAND_ROUTER_HPP
