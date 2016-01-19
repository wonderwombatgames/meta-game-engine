# TO DO
https://trello.com/b/vs9Ajlx5/game-engine-ecs#

* [X] ~~move parts of backend_graphic_sdl.cpp that do not directly depend on SDL to another file: system_graphic.cpp~~

* [X] ~~change the namespace Engine to W2E and BackEnd to BE~~

* [X] ~~rename all statics to either GLOBAL or INTERNAL or PERSISTENT~~

* [X] ~~remove addComponent from EntitiesManager and add to Systems::BaseInterface a method to add an entity binded to an assetId~~

* [X] ~~add camera control to the graphic system - changed  GraphicInterface::paint() and added Graphics::setCameraTransform.~~

* [X] PARTIAL: replace all bool return for ErrorCode! - missing more error enumerations

* [X] ~~find better way to initialize the POD's - Only transformPod is not initialized. Colours and Vectors are purposefully let without default to allow list instantiation. Constants were set for init with a copy constructor~~

* [ ] PARTIAL: update unit tests -> add graphic system rendering and transform to the entity then remove manual rendering - WORKING but need to hand initialize graphicPOD and transformPOD!

* [ ] reevaluate all the friend uses through out the code

* [ ] change image load to use stb_image and remove sdl_image

* [ ] load texture from atlas

* [ ] load texture from network

* [ ] add custom allocator to the types alias in utils_types.hpp

* [ ] change the RNG to Twist Mersene

* [ ] find a way to remove Image::computeClipRects(const GraphicPod & component, BoxBoundXYWH & src, BoxBoundXYWH & dst, Vector2 & center);

* [ ] remove backend.hpp file

* [ ] reduce usage of STL
