
// Name
//===================================
//
// SpriteMovement
//


// DEPENDENCIES
//===================================

    #include "camera.h"
    #include "sprite_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SpriteGraphics::SpriteGraphics
    (
        std::string&& texture,
        int current_frame_x,
        int current_frame_y,
        bool flip_x,
        bool flip_y,
        double rotation,
        bool priority,
        int x_adjustment,
        int y_adjustment,
        int w_adjustment,
        int h_adjustment,
        int animation_speed
    )
    :
        current_frame_x_ ( current_frame_x ),
        current_frame_y_ ( current_frame_y ),
        flip_x_ ( flip_x ),
        flip_y_ ( flip_y ),
        rotation_ ( rotation ),
        priority_ ( priority ),
        x_adjustment_ ( x_adjustment ),
        y_adjustment_ ( y_adjustment ),
        w_adjustment_ ( w_adjustment ),
        h_adjustment_ ( h_adjustment ),
        texture_ ( texture ),
        animation_timer_ ( { animation_speed } ),
		prev_frame_x_ ( 0 ),
		prev_frame_y_ ( 0 )
    {};

    SpriteGraphics::~SpriteGraphics() {};

    void SpriteGraphics::update( Sprite& sprite )
    {
    };

    void SpriteGraphics::update()
    {
    };

    void SpriteGraphics::render( Graphics& graphics, const sdl2::SDLRect& bound_box, Camera* camera, bool priority ) const
    {
        masterRender( graphics, bound_box, current_frame_x_, current_frame_y_, camera, priority );
    };

    void SpriteGraphics::masterRender( Graphics& graphics, const sdl2::SDLRect& bound_box, int current_frame_x, int current_frame_y, Camera* camera, bool priority ) const
    {
        if ( visible_ && ( ( priority && priority_ ) || ( !priority && !priority_ ) ) )
        {
            sdl2::SDLRect dest = adjustBoundBox( bound_box );

            sdl2::SDLRect source =
            {
                current_frame_x,
                current_frame_y,
                dest.w,
                dest.h
            };

            if ( camera != nullptr )
            {
                if ( camera->onscreenPixels( dest ) )
                {
                    dest =
                    {
                        camera->relativeX( dest ),
                        camera->relativeY( dest ),
                        dest.w,
                        dest.h
                    };

                    source =
                    {
                        current_frame_x,// + cutoffLeft( bound_box, *camera ),
                        current_frame_y,// + cutoffTop ( bound_box, *camera ),
                        dest.w,
                        dest.h
                    };
                }
                else
                {
                    return; // If not onscreen, don't draw; just quit function now.
                }
            }

            SDL_RendererFlip flip = SDL_FLIP_NONE;

            if ( flip_x_ && flip_y_ )
                flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
            else if ( flip_x_ )
                flip = SDL_FLIP_HORIZONTAL;
            else if ( flip_y_ )
                flip = SDL_FLIP_VERTICAL;

            graphics.renderObject( texture_, source, dest, flip, rotation_ );

        }
    };

    int SpriteGraphics::cutoffLeft( int object_x, int camera_x ) const
    {
        if ( object_x < camera_x )
            return camera_x - object_x;
        else
            return 0;
    };

    int SpriteGraphics::cutoffTop( int object_y, int camera_y ) const
    {
        if ( object_y < camera_y )
            return camera_y - object_y;
        else
            return 0;
    };

    int SpriteGraphics::cutoffRight( int object_x, int camera_x, int object_w, int camera_w ) const
    {
        if ( object_x > camera_x + camera_w - object_w )
            return object_x - ( camera_x + camera_w - object_w );
        else
            return 0;
    };

    int SpriteGraphics::cutoffBottom( int object_y, int camera_y, int object_h, int camera_h ) const
    {
        if ( object_y > camera_y + camera_h - object_h )
            return object_y - ( camera_y + camera_h - object_h );
        else
            return 0;
    };

    int SpriteGraphics::cutoffLeft( sdl2::SDLRect& obj_r, Camera& c ) const
    {
        return cutoffLeft( obj_r.x, c.x() );
    };

    int SpriteGraphics::cutoffTop( sdl2::SDLRect& obj_r, Camera& c ) const
    {
        return cutoffTop( obj_r.y, c.y() );
    };

    int SpriteGraphics::cutoffRight( sdl2::SDLRect& obj_r, Camera& c ) const
    {
        return cutoffRight( obj_r.x, c.x(), obj_r.w, c.widthPixels() );
    };

    int SpriteGraphics::cutoffBottom( sdl2::SDLRect& obj_r, Camera& c ) const
    {
        return cutoffBottom( obj_r.y, c.y(), obj_r.h, c.heightPixels() );
    };

    sdl2::SDLRect SpriteGraphics::adjustBoundBox( const sdl2::SDLRect& r ) const
    {
        return
        {
            r.x + x_adjustment_,
            r.y + y_adjustment_,
            r.w + w_adjustment_,
            r.h + h_adjustment_
        };
    };
