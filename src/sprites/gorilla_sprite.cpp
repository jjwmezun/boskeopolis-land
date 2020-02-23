#include "animated_graphics.hpp"
#include "collision.hpp"
#include "gorilla_sprite.hpp"
#include "level_state.hpp"

GorillaSprite::GorillaSprite( int x, int y )
:
	Sprite( std::unique_ptr<SpriteGraphics> ( new AnimatedGraphics( "sprites/gorilla.png", { std::make_pair( 0, 0 ), std::make_pair( 16, 0 ) } ) ), x, y, 16, 16, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 800, 3000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT, true )
{};

GorillaSprite::~GorillaSprite() {};

void GorillaSprite::customUpdate( LevelState& level_state )
{
	turnOnCollide();
	moveInDirectionX();
	graphics_->flip_x_ = direction_x_ == Direction::Horizontal::RIGHT;
	if ( fellInBottomlessPit( level_state.currentMap() ) )
	{
		kill();
	}
};

void GorillaSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};
