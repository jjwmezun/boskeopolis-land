#include "level_state.hpp"
#include "sillyfish_sprite.hpp"
#include "sprite_graphics.hpp"

SillyfishSprite::SillyfishSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/sillyfish.png" ), x, y, 16, 32, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 600, 1400, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	animation_frame_ (),
	animation_timer_ ()
{};

SillyfishSprite::~SillyfishSprite() {};

void SillyfishSprite::customUpdate( LevelState& level_state )
{
	turnOnEdge( level_state.blocks() );
	turnOnCollide();
	moveInDirectionX();
	updateGraphics();
};

void SillyfishSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};

void SillyfishSprite::updateGraphics()
{
	flipGraphicsOnRight();
	if ( animation_timer_.hit() )
	{
		++animation_frame_;
		graphics_->current_frame_x_ = ( animation_frame_() == 0 ) ? 16 : 0;
	}
	animation_timer_.update();
};
