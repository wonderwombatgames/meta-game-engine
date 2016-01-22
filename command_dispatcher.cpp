/**
  *
  */

#include "command_dispatcher.hpp"

namespace W2E
{
// using namespace std;
using namespace Utils;

HashMultiMap< CommandType, ICommandee* > CommandDispatcher::subscribers;

CommandDispatcher::CommandDispatcher(){};

CommandDispatcher::~CommandDispatcher(){};

CommandDispatcher* CommandDispatcher::instance()
{
  LOCALPERSISTENT_ CommandDispatcher* sinstance_ = nullptr;
  if(nullptr == sinstance_)
  {
    sinstance_ = new CommandDispatcher();
  }
  return sinstance_;
}

ErrorCode CommandDispatcher::subscribe(CommandType type, ICommandee* cmd)
{
  auto range = subscribers.equal_range(type);
  for(auto it = range.first; it != range.second; ++it)
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

ErrorCode CommandDispatcher::unSubscribe(const CommandType type, const ICommandee* cmd)
{
  auto range = subscribers.equal_range(type);
  for(auto it = range.first; it != range.second; ++it)
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

void CommandDispatcher::dispatch(const CommandMsg& msg)
{
  auto range = subscribers.equal_range(msg.type);
  for(auto it = range.first; it != range.second; ++it)
  {
    it->second->run(msg);
  }
}

} // end namespace W2E
