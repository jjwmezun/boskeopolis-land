


// Name
//===================================
//
// SpriteComponentCircle
//


// DEPENDENCIES
//===================================

    #include "sprite.hpp"
    #include "sprite_component_circle.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SpriteComponentCircle::SpriteComponentCircle( int radius, int speed, bool before_update )
    :
        SpriteComponent ( before_update ),
        radius_ ( radius ),
        speed_ ( speed ),
        angle_( 0 )
    {};

    SpriteComponentCircle::~SpriteComponentCircle() {};

    void SpriteComponentCircle::update( Sprite& sprite, SpriteGraphics& graphics )
    {
        sprite.changeX( sprite.originalXSubPixels() + ( sprite.widthSubPixels() / 2 ) + ( std::cos( angle() ) * radius() ) );
        sprite.changeY( sprite.originalYSubPixels() + ( sprite.heightSubPixels() / 2 ) + ( std::sin( angle() ) * radius() ) );

        angle_ += speed_;
    };

    double SpriteComponentCircle::angle() const
    {
        return (double)angle_ / 1000;
    };

    int SpriteComponentCircle::radius() const
    {
        return Unit::PixelsToSubPixels( radius_ );
    };
