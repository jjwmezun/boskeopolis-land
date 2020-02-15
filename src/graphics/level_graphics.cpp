#include "level_graphics.hpp"
#include "render.hpp"

void LevelGraphics::render( const Camera* camera ) const
{
    switch ( type_ )
    {
        case( Type::SPRITE ):
        {
            Render::renderObject
            (
                value_.sprite_.texture_,
                value_.sprite_.source_,
                value_.sprite_.dest_,
                value_.sprite_.flip_,
                value_.sprite_.rotation_,
                value_.sprite_.alpha_,
                camera,
                value_.sprite_.blend_mode_
            );
        }
        break;
    }
};