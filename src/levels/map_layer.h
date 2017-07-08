



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
	class EventSystem;
    class Graphics;
	class Sprite;


// DEPENDENCIES
//===================================


// CLASS
//===================================

    class MapLayer
    {
        public:
			virtual ~MapLayer() {};
            virtual void update( EventSystem& events ) {};
            virtual void render( Graphics& graphics, Camera& camera ) const = 0;
			virtual void interact( Sprite& sprite ) {};
    };

#endif // MAP_LAYER_IMAGE_H
