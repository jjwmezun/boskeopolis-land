#include "falling_dollar_sprite.hpp"
#include "falling_dollar_generator_sprite.hpp"
#include "collision.hpp"
#include "level_state.hpp"
#include "sprite_system.hpp"

FallingDollarGeneratorSprite::FallingDollarGeneratorSprite( int x, int y )
:
	Sprite( nullptr, x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
    timer_ ( 0 )
{};

FallingDollarGeneratorSprite::~FallingDollarGeneratorSprite() {};

void FallingDollarGeneratorSprite::customUpdate( LevelState& level_state )
{
    ++timer_;
    if ( timer_ == 80 )
    {
        level_state.sprites().spawn( std::unique_ptr<FallingDollarSprite> ( new FallingDollarSprite( xPixels(), yPixels() ) ) );
        timer_ = 0;
    }
};

void FallingDollarGeneratorSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};
