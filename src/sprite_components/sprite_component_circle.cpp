#include "sprite.hpp"
#include "sprite_component_circle.hpp"

SpriteComponentCircle::SpriteComponentCircle( int radius, int speed, double start_angle )
:
    SpriteComponent (),
    radius_ ( ( double )( Unit::PixelsToSubPixels( radius ) ) ),
    speed_ ( speed / 1000.0 ),
    angle_( start_angle )
{};

SpriteComponentCircle::~SpriteComponentCircle() {};

void SpriteComponentCircle::update( Sprite& sprite, LevelState& state )
{
    sprite.changeX( sprite.originalXSubPixels() + ( int )( ( ( double )( sprite.widthSubPixels() ) / 2.0 ) + ( std::cos( angle_ ) * radius_ ) ) );
    sprite.changeY( sprite.originalYSubPixels() + ( int )( ( ( double )( sprite.heightSubPixels() ) / 2.0 ) + ( std::sin( angle_ ) * radius_ ) ) );
    angle_ += speed_;
};
