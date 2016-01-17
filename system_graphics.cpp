#include "system_graphics.hpp"

namespace Engine
{

namespace System
{

  GraphicResourceBinder::GraphicResourceBinder(
      SystemsInterface * system,
      Component::GraphicInterface * resource,
      ComponentsHashMap * components)
          : _system(system)
          , _resource(resource)
          , _components(components)
  {}

  bool GraphicResourceBinder::toEntity(EntityBase * entity)
  {
    if(_system != nullptr)
    {
      EntityID id = entity->addComponent(*_system);
      auto it = this->_components->find(id);
      if (it != this->_components->end())
      {
        it->second.resource = _resource;
        return true;
      }
    }
    return false;
  }

  void Graphics::insert(Component::EntityPod & entity)
  {
    Component::TransformPod * transform = entity.transform;
    if (transform != nullptr)
    {
      Component::GraphicPod pod;
      this->_components.emplace(entity.entityId, pod);
    }
  }

  void Graphics::remove(const Component::EntityPod & entity)
  {
    auto it = this->_components.find(entity.entityId);
    if (it != this->_components.end())
    {
      this->_components.erase(it);
    }
  }

  void Graphics::tick(TimeDim delta)
  {

  }

  ResourceBinderPtr Graphics::getResourceBinder(ResourceID resourceId)
  {
    ResourceBinderPtr retVal;
    auto it = _resources.find(resourceId);
    if(it != _resources.end())
    {
      retVal.reset(
        new GraphicResourceBinder(this, it->second.get(), &_components)
      );
    }
    return retVal;
  }


} // end namespace System

} // end namespace Engine
