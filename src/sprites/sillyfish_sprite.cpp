#include "sillyfish_sprite.hpp"
#include "sprite_graphics.hpp"

SillyfishSprite::SillyfishSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/sillyfish.png" ), x, y, 16, 32, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 600, 1400, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	animation_frame_ (),
	animation_timer_ ()
{};

SillyfishSprite::~SillyfishSprite() {};

void SillyfishSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	turnOnEdge( blocks );
	turnOnCollide();
	moveInDirectionX();
};

void SillyfishSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
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
