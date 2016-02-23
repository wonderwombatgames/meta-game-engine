# TO DO
https://trello.com/b/vs9Ajlx5/game-engine-ecs#

* [X] ~~move parts of backend_graphic_sdl.cpp that do not directly depend on SDL to another file: system_graphic.cpp~~

* [X] ~~change the namespace Engine to W2E and BackEnd to BE~~

* [X] ~~rename all statics to either GLOBAL or INTERNAL or PERSISTENT~~

* [X] ~~remove addComponent from EntitiesManager and add to Systems::BaseInterface a method to add an entity binded to an assetId~~

* [X] ~~add camera control to the graphic system - changed  GraphicInterface::paint() and added Graphics::setCameraTransform.~~

* [X] ~~find a way to remove Image::computeClipRects(const GraphicPod & component, BoxBoundXYWH & src, BoxBoundXYWH & dst, Vector2 & center);~~

* [X] ~~find better way to initialize the POD's - Only transformPod is not initialized. Colours and Vectors are purposefully let without default to allow list instantiation. Constants were set for init with a copy constructor~~

* [X] ~~update unit tests -> add graphic system rendering and transform to the entity then remove manual rendering~~

* [X] ~~reduce include dependency => reorganize files and use forward declaration~~

* [X] ~~remove backend.hpp file~~

* [X] ~~change the initializers to use init_list {} instead of constructor~~

* [X] PARTIAL: replace all bool return for ErrorCode! - missing more error enumerations

* [X] PARTIAL: add input system - 6 axis + 12 buttons + keyboard ring buffer - needs to support multiple components per entity!!!

* [ ] rename component_ and resources_  to dataPod_ and componentImpl_ at the Systems

* [ ] rename resource to componentImpl at the component_*.hpp Pod's

* [ ] rename ResourceId to ComponenteId

* [ ] reevaluate all the friend uses through out the code

* [ ] change image load to use stb_image and remove sdl_image

* [ ] load texture from atlas

* [ ] load texture from network

* [ ] add custom allocator to the types alias in utils_types.hpp

* [ ] change the RNG to Twisted Mersene

* [ ] replace enum for enum class where applicable

* [ ] reduce usage of STL
  * [ ] replace unordered_map for a custom Container
  * [ ] replace strings for a custom fixed size string
  * [ ] replace allocator
