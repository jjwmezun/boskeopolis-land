



// Name
//===================================
//
// MapLayer
//

#ifndef MAP_LAYER_H
#define MAP_LAYER_H


// FORWARD DECLARATIONS
//===================================

    class Camera;
    class Graphics;


// DEPENDENCIES
//===================================


// CLASS
//===================================

    class MapLayer
    {
        public:
            virtual void update() = 0;
            virtual void render( Graphics& graphics, Camera& camera ) const = 0;
    };

#endif // MAP_LAYER_IMAGE_H
