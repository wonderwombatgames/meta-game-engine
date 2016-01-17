# TO DO
https://trello.com/b/vs9Ajlx5/game-engine-ecs#

* [ ] - Not Done
* [X] - Done


* [ ] update unit tests -> add graphic system rendering and transform to the entity then remove manual rendering

* [X] ~~rename all statics to either GLOBAL or INTERNAL or PERSISTENT~~

* [ ] add custom allocator to the types alias in utils_types.hpp

* [ ] change image load to use stb_image and remove sdl_image

* [X] ~~remove addComponent from EntitiesManager and add to Systems::BaseInterface a method to add an entity binded to an assetId~~

* [ ] load texture from atlas

* [ ] load texture from network

* [ ] change the RNG to Twist Mersene

* [X] PARTIAL: replace all bool return for ErrorCode! - missing more error enumerations

* [ ] find better way to initialize the POD's

* [X] ~~move parts of backend_graphic_sdl.cpp that do not directly depend on SDL to another file: system_graphic.cpp~~

* [X] ~~change the namespace Engine to W2E and BackEnd to BE~~

* [ ] find a way to remove Image::void computeClipRects(const GraphicPod & component, BoxBoundXYWH & src, BoxBoundXYWH & dst, Vector2 & center);

* [ ] remove backend.hpp file

* [ ] reduce usage of STL
