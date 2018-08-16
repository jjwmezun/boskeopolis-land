#include "block_component.hpp"
#include "block_system.hpp"
#include "circle_block_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

CircleBlockSprite::CircleBlockSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/killer-choque.png" ), x, y, 24, 24, {}, 8000, 8000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY ),
	frames_ ( { 0, 1, 2, 3, 4, 5, 5, 4, 3, 2, 1, 0 } ),
	direction_ ( Direction::Simple::LEFT ),
	animation_timer_ ( 0 )
{};

CircleBlockSprite::~CircleBlockSprite() {};

void CircleBlockSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	switch ( direction_ )
	{
		case ( Direction::Simple::UP ):
			moveUp();
			if ( collide_bottom_ || collide_bottom_prev_ )
			{
				fullStopY();
				direction_ = Direction::Simple::LEFT;
			}
		break;
		case ( Direction::Simple::RIGHT ):
			moveRight();
			if ( blocks.blocksInTheWay( { rightSubPixels() + 1000, hit_box_.y, 4000, 4000 }, BlockComponent::Type::SOLID ) )
			{
				fullStopX();
				direction_ = Direction::Simple::UP;
			}
		break;
		case ( Direction::Simple::DOWN ):
			moveDown();
			if ( collide_top_ || collide_top_prev_ )
			{
				fullStopY();
				direction_ = Direction::Simple::RIGHT;
			}
		break;
		case ( Direction::Simple::LEFT ):
			moveLeft();
			if ( collide_left_ || collide_left_prev_ )
			{
				fullStopX();
				direction_ = Direction::Simple::DOWN;
			}
		break;
	}

	if ( animation_timer_ % 8 == 0 )
	{
		if ( animation_timer_ > NUMBER_OF_FRAMES * 8 )
		{
			animation_timer_ = 0;
		}
		graphics_->current_frame_x_ = 24 * frames_[ ( unsigned int )( floor( animation_timer_ / 8 ) ) ];
	}
	++animation_timer_;
};

void CircleBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );
		them.kill();
	}
};
