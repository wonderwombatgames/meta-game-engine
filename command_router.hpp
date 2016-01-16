/**
  * singleton routes commands to subscribers.
  * the addresses for the commands are defined by their subscription key
  */

#ifndef COMMAND_ROUTER_HPP
#define COMMAND_ROUTER_HPP


#include "command_interface.hpp"

namespace Engine
{
  using namespace std;

  class CommandRouter
  {
  public:
    CommandRouter(CommandRouter & other) = delete;
    ~CommandRouter();

    static CommandRouter * instance();

    bool subscribe(CommandType type, ICommand * cmd);
    bool unSubscribe(const CommandType type, const ICommand * cmd);
    void dispatch(const CommandMsg & msg);

  protected:
    CommandRouter();
    static HashMultiMap< CommandType, ICommand * > subscribers;
  };

} // end namespace Engine

#endif // COMMAND_ROUTER_HPP
