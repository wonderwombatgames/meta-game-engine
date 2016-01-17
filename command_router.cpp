/**
  *
  */


#include "command_router.hpp"

namespace W2E
{
  //using namespace std;
  using namespace Utils;

  HashMultiMap< CommandType, ICommand * > CommandRouter::subscribers;

  CommandRouter::CommandRouter(){};

  CommandRouter::~CommandRouter(){};

  CommandRouter * CommandRouter::instance()
  {
    LOCAL_PERSISTENT CommandRouter * s_instance = nullptr;
    if (nullptr == s_instance)
    {
      s_instance = new CommandRouter();
    }
    return s_instance;
  }


  ErrorCode CommandRouter::subscribe(CommandType type, ICommand * cmd)
  {
    auto range = subscribers.equal_range(type);
    for (auto it = range.first; it != range.second; ++it)
    {
      // one cannot subscribe more than once
      if(it->second == cmd)
      {
        return UNKNOWN_ERROR;
      }
    }
    subscribers.emplace(type, cmd);

    return NO_ERROR;
  }


  ErrorCode CommandRouter::unSubscribe(const CommandType type, const ICommand * cmd)
  {
    auto range = subscribers.equal_range(type);
    for (auto it = range.first; it != range.second; ++it)
    {
      // have to find the exact pair
      if(it->second == cmd)
      {
        subscribers.erase(it);
        return NO_ERROR;
      }
    }

    // did not found that pair!
    return UNKNOWN_ERROR;
  }


  void CommandRouter::dispatch(const CommandMsg & msg)
  {
    auto range = subscribers.equal_range(msg.type);
    for (auto it = range.first; it != range.second; ++it)
    {
        it->second->run(msg);
    }
  }


} // end namespace W2E
