/**
  *
  */


#include "command_router.hpp"

namespace Engine
{
  //using namespace std;
  using namespace Utils;

  HashMultiMap< CommandType, ICommand * > CommandRouter::subscribers;

  CommandRouter::CommandRouter(){};

  CommandRouter::~CommandRouter(){};

  CommandRouter * CommandRouter::instance()
  {
    static CommandRouter * s_instance = nullptr;
    if (nullptr == s_instance)
    {
      s_instance = new CommandRouter();
    }
    return s_instance;
  }


  bool CommandRouter::subscribe(CommandType type, ICommand * cmd)
  {
    auto range = subscribers.equal_range(type);
    for (auto it = range.first; it != range.second; ++it)
    {
      // one cannot subscribe more than once
      if(it->second == cmd)
      {
        return false;
      }
    }
    subscribers.emplace(type, cmd);

    return true;
  }


  bool CommandRouter::unSubscribe(const CommandType type, const ICommand * cmd)
  {
    auto range = subscribers.equal_range(type);
    for (auto it = range.first; it != range.second; ++it)
    {
      // have to find the exact pair
      if(it->second == cmd)
      {
        subscribers.erase(it);
        return true;
      }
    }

    // did not found that pair!
    return false;
  }


  void CommandRouter::dispatch(const CommandMsg & msg)
  {
    auto range = subscribers.equal_range(msg.type);
    for (auto it = range.first; it != range.second; ++it)
    {
        it->second->run(msg);
    }
  }


} // end namespace Engine
