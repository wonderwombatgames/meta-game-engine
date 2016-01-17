# TO DO

* rename all statics to either GLOBAL or INTERNAL or PERSISTENT

* add custom allocator to the types alias in utils_types.hpp

* change image load to use stb_image and remove sdl_image

* ~~remove addComponent from EntitiesManager and add to Systems::BaseInterface a method to add an entity binded to an assetId~~

* load texture from atlas

* load texture from network

* change the RNG to Twist Mersene

* replace all bool return for ErrorCode!

* find better way to initialize the POD's

* move parts of backend_graphic_sdl.cpp that do not directly depend on SDL to another file: system_graphic.cpp
