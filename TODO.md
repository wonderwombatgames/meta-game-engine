# TO DO
https://trello.com/b/vs9Ajlx5/game-engine-ecs#

* [X] ~~move parts of backend_graphic_sdl.cpp that do not directly depend on SDL to another file: system_graphic.cpp~~

* [X] ~~change the namespace Engine to W2E and BackEnd to BE~~

* [X] ~~rename all statics to either GLOBAL or INTERNAL or PERSISTENT~~

* [X] ~~remove addComponent from EntitiesManager and add to Systems::BaseInterface a method to add an entity binded to an assetId~~

* [X] PARTIAL: replace all bool return for ErrorCode! - missing more error enumerations

* [ ] PARTIAL: add camera control to the graphic system - changed  GraphicInterface::paint() and added Graphics::setCameraTransform.

* [ ] PARTIAL: update unit tests -> add graphic system rendering and transform to the entity then remove manual rendering - NOT WORKING!

* [ ] change image load to use stb_image and remove sdl_image

* [ ] load texture from atlas

* [ ] load texture from network

* [ ] add custom allocator to the types alias in utils_types.hpp

* [ ] change the RNG to Twist Mersene

* [ ] find better way to initialize the POD's

* [ ] find a way to remove Image::void computeClipRects(const GraphicPod & component, BoxBoundXYWH & src, BoxBoundXYWH & dst, Vector2 & center);

* [ ] remove backend.hpp file

* [ ] reduce usage of STL
