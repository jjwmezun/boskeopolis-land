#include "capn_clearbeard_sprite.hpp"
#include "sprite_graphics.hpp"

CapnClearbeardSprite::CapnClearbeardSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/capn-clearbeard.png" ), x, y, 18, 30, { SpriteType::ENEMY, SpriteType::BOPPABLE }, 50, 5000, 2000, 5000, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT ),
	jump_timer_ ( 0 )
{};

CapnClearbeardSprite::~CapnClearbeardSprite() {};

void CapnClearbeardSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	graphics_->flip_x_ = direction_x_ == Direction::Horizontal::RIGHT;
	moveInDirectionX();

	if ( collide_left_ || collide_left_prev_ )
	{
		jump_timer_ = 15;
		//direction_x_ = Direction::Horizontal::RIGHT;
	}

	if ( jump_timer_ > 0 )
	{
		jump();
		++jump_timer_;
	}
};

void CapnClearbeardSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{};
