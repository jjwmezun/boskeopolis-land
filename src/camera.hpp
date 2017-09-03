
// CAMERA
//===================================

#ifndef CAMERA_H
#define CAMERA_H


// FORWARD DECLARATIONS
//===================================

    class Map;
    class Sprite;


// DEPENDENCIES
//===================================

    #include "direction.hpp"
    #include "unit.hpp"
    #include <SDL2/SDL.h>


// CLASS
//===================================

    class Camera
    {
        public:
            enum class XPriority
            {
                __NULL,
                CENTER
            };

            enum class YPriority
            {
                __NULL,
                CENTER
            };

            constexpr Camera
            (
                int x = 0,
                int y = 0,
                int width = Unit::WINDOW_WIDTH_BLOCKS,
                int height = Unit::WINDOW_HEIGHT_BLOCKS - 2,
                int x_offset = 0,
                int y_offset = 0
            )
            :
                x_( x ),
                y_( y ),
                width_( width ),
                height_( height ),
                x_offset_ ( x_offset ),
                y_offset_ ( y_offset ),
                prev_x_ ( -87654 ),
                prev_y_ ( -87654 ),
                changed_ ( true )
            {};

            constexpr int widthBlocks() const
            {
                return width_;
            };

            constexpr int heightBlocks() const
            {
                return height_;
            };

            constexpr int widthPixels() const
            {
                return Unit::BlocksToPixels( width_ );
            };

            constexpr int heightPixels() const
            {
                return Unit::BlocksToPixels( height_ );
            };

            constexpr int x() const
            {
                return x_;
            };

            constexpr int y() const
            {
                return y_;
            };

            constexpr int xOffset() const
            {
                return x_offset_;
            };

            constexpr int yOffset() const
            {
                return y_offset_;
            };

            constexpr int prevX() const
            {
                return prev_x_;
            };

            constexpr int prevY() const
            {
                return prev_y_;
            };

            constexpr int right() const
            {
                return x_ + widthPixels();
            };

            constexpr int bottom() const
            {
                return y_ + heightPixels();
            };

            constexpr int left() const
            {
                return x_ + xOffset();
            };

            constexpr int top() const
            {
                return y_ + yOffset();
            };

            constexpr int boundaryTop() const
            {
                return y_ + ( heightPixels() * .15 );
            };

            constexpr int boundaryTopWeaker() const
            {
                return y_ + ( heightPixels() * .15 );
            };

            constexpr int boundaryCenterY() const
            {
                return y_ + ( heightPixels() * .5 );
            };

            constexpr int boundaryBottom() const
            {
                return y_ + ( heightPixels() * .75 );
            };

            constexpr int boundaryLeft() const
            {
                return x_ + ( widthPixels() * .35 );
            };

            constexpr int boundaryCenterX() const
            {
                return x_ + ( widthPixels() * .5 );
            };

            constexpr int boundaryRight() const
            {
                return x_ + ( widthPixels() * .65 );
            };

            constexpr bool offscreenTop( const sdl2::SDLRect& r, int padding = 0 ) const
            {
                return r.y + r.h < Unit::PixelsToSubPixels( y() - padding );
            };

            constexpr bool offscreenLeft( const sdl2::SDLRect& r, int padding = 0 ) const
            {
                return r.x + r.w < Unit::PixelsToSubPixels( x() - padding );
            };

            constexpr bool offscreenBottom( const sdl2::SDLRect& r, int padding = 0 ) const
            {
                return r.y > Unit::PixelsToSubPixels( bottom() + padding );
            };

            constexpr bool offscreenRight( const sdl2::SDLRect& r, int padding = 0 ) const
            {
                return r.x > Unit::PixelsToSubPixels( right() + padding );
            };

            constexpr bool offscreenPartwayTop( const sdl2::SDLRect& r ) const
            {
                return r.y < y();
            };

            constexpr bool offscreenPartwayRight( const sdl2::SDLRect& r ) const
            {
                return r.x + r.h > widthPixels();
            };

            constexpr bool offscreenPartwayBottom( const sdl2::SDLRect& r ) const
            {
                return r.y + r.w > heightPixels();
            };

            constexpr bool offscreenPartwayLeft( const sdl2::SDLRect& r ) const
            {
                return r.x < x();
            };

            constexpr int relativeX( const sdl2::SDLRect& r ) const
            {
                return r.x - x() + xOffset();
            };

            constexpr int relativeY( const sdl2::SDLRect& r ) const
            {
                return r.y - y() + yOffset();
            };

            constexpr int relativeX( int n ) const
            {
                return n - x() + xOffset();
            };

            constexpr int relativeY( int n ) const
            {
                return n - y() + yOffset();
            };

            bool onscreen( const sdl2::SDLRect& r, int padding = 0 )  const;
            bool onscreenPixels( const sdl2::SDLRect& r, int padding = 0 )  const;
            bool offscreen( const sdl2::SDLRect& r, int padding = 0, Direction::Simple direction = Direction::Simple::__NULL ) const;

            bool changed() const;
			void update();

            void adjust( Sprite& o, Map& m );
            void adjustCart( Sprite& o, Map& m );
            void setPrev();
            void moveDown( int amount = 1 );
            void moveUp( int amount = 1 );
            void moveLeft( int amount = 1 );
            void moveRight( int amount = 1 );
            void setPosition( int x, int y );

        private:
            int x_;
            int y_;
            const int x_offset_;
            const int y_offset_;
            const int width_;
            const int height_;
            int prev_x_;
            int prev_y_;
            bool changed_ = true;

            void contain( Map& m );
    };

#endif // CAMERA_H
