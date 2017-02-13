
// Name
//===================================
//
// Object
//

#ifndef OBJECT_H
#define OBJECT_H


// FORWARD DECLARATIONS
//===================================

    class Camera;
    class Collision;
    class Graphics;
    class Input;


// DEPENDENCIES
//===================================

    #include "direction.h"
    #include <SDL2/SDL.h>
    #include "unit.h"


// CLASS
//===================================

    class Object
    {
        public:
            Object
            (
                int x      = 0,
                int y      = 0,
                int width  = 16,
                int height = 16
            );
            virtual ~Object();

            virtual void render( Graphics& graphics, Camera& camera, bool priority = false ) = 0;

            void changeX( int new_x_pixels );
            void changeY( int new_y_pixels );
            void addToX ( int addition_x_pixels );
            void addToY ( int addition_y_pixels );

            const Collision collide( Object* other ) const;

            const sdl2::SDLRect& hitBox();
            const sdl2::SDLRect& originalHitBox();

            int originalXSubPixels() const;
            int originalYSubPixels() const;

            int xPixels()    const;
            int yPixels()    const;
            int xSubPixels() const;
            int ySubPixels() const;

            int xPrevSubPixels() const;
            int yPrevSubPixels() const;
            int xPrevPixels()    const;
            int yPrevPixels()    const;

            int heightPixels()    const;
            int widthPixels()     const;
            int heightSubPixels() const;
            int widthSubPixels()  const;

            int halfWidthSubPixels()  const;
            int halfHeightSubPixels() const;
            int centerXSubPixels()    const;
            int centerYSubPixels()    const;

            int halfWidthPixels()  const;
            int halfHeightPixels() const;
            int centerXPixels() const;
            int centerYPixels() const;

            int leftSubPixels()   const;
            int rightSubPixels()  const;
            int topSubPixels()    const;
            int bottomSubPixels() const;

            int rightPixels()  const;
            int bottomPixels() const;

            int prevLeftSubPixels()   const;
            int prevRightSubPixels()  const;
            int prevTopSubPixels()    const;
            int prevBottomSubPixels() const;

            int prevRightPixels()  const;
            int prevBottomPixels() const;


        protected:
            int x_prev_ = -123456789;
            int y_prev_ = -123456789;
            sdl2::SDLRect hit_box_;
            const sdl2::SDLRect original_hit_box_;
    };

#endif // OBJECT_H

