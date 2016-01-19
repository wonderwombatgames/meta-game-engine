#include "system_graphics.hpp"

#include <iostream>

namespace W2E
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
  {
  }

  ErrorCode GraphicResourceBinder::toEntity(EntityBase * entity)
  {
    if(_system != nullptr)
    {
      EntityID id = entity->registerIntoSystem(*_system);

      auto it = this->_components->find(id);
      if (it != this->_components->end())
      {
        it->second.resource = _resource;
        return NO_ERROR;
      }
    }
    return UNKNOWN_ERROR;
  }

  void Graphics::insert(Component::EntityPod & entity)
  {
    if (entity.transform)
    {
      Component::GraphicPod pod;
      this->_components.emplace(entity.entityId, pod);
      this->_components[entity.entityId].transformData = entity.transform;
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

  ErrorCode Graphics::setCameraTransform(Component::TransformPod & transformData)
  {
    this->_camera = transformData;
    return NO_ERROR;
  }

  void Graphics::tick(TimeDim delta)
  {
    for(auto comp : this->_components)
    {
      if(comp.second.isVisible &&
         comp.second.resource )
      {
        comp.second.resource->paint(comp.second, this->_camera);
      }
    }
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

} // end namespace W2E
