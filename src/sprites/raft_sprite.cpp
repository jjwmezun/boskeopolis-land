#include "raft_sprite.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

static constexpr int determineSpeed( int x )
{
	return ( x > Unit::BlocksToPixels( 10 ) ) ? 500 : 2000;
}

RaftSprite::RaftSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/raft.png" ), x, y, 72, 16, { SpriteType::RAFT }, 200, determineSpeed( x ), 0, 0, Direction::Horizontal::RIGHT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED )
{};

RaftSprite::~RaftSprite() {};

void RaftSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	moveInDirectionX();
};

void RaftSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		them.collideStopAny( their_collision );

		if ( their_collision.collideBottom() )
		{
			them.hit_box_.x += vx_;
		}
	}
	else if ( them.hasType( SpriteType::RAFT ) && their_collision.collideAny() )
	{
		if ( them.top_speed_ < top_speed_ )
		{
			them.top_speed_ += 500;
			top_speed_ += 500;
			direction_x_ = Direction::Horizontal::LEFT;
		}
	}
};
