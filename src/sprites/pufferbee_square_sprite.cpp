#include "block_system.hpp"
#include "pufferbee_square_sprite.hpp"
#include "pufferbee_graphics.hpp"

static constexpr int MOVEMENT_SPEED = 4000;

PufferbeeSquareSprite::PufferbeeSquareSprite
(
	int x,
	int y,
	Direction::Simple start_direction
)
:
    Sprite
	(
		std::make_unique<PufferbeeGraphics> (),
		x + 6,
		y + 6,
		20,
		20,
		{ SpriteType::ENEMY },
		2000,
		2000,
		0,
		0,
		Direction::Horizontal::__NULL,
		Direction::Vertical::__NULL,
		nullptr,
		SpriteMovement::Type::FLOATING,
		CameraMovement::PERMANENT
	),
	start_direction_ ( start_direction ),
	direction_ ( start_direction )
{};

PufferbeeSquareSprite::~PufferbeeSquareSprite() {};

void PufferbeeSquareSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	switch ( direction_ )
	{
		case ( Direction::Simple::UP ):
		{
			hit_box_.y -= MOVEMENT_SPEED;
			if ( blocks.blocksInTheWay( { hit_box_.x, hit_box_.y - 6000, hit_box_.w, 6000 }, BlockComponent::Type::SOLID ) )
			{
				direction_ = Direction::Simple::RIGHT;
			}
		}
		break;
		case ( Direction::Simple::RIGHT ):
		{
			hit_box_.x += MOVEMENT_SPEED;
			if ( blocks.blocksInTheWay( { rightSubPixels(), hit_box_.y, 6000, hit_box_.h }, BlockComponent::Type::SOLID ) )
			{
				direction_ = Direction::Simple::DOWN;
			}
		}
		break;
		case ( Direction::Simple::DOWN ):
		{
			hit_box_.y += MOVEMENT_SPEED;
			if ( blocks.blocksInTheWay( { hit_box_.x, bottomSubPixels(), hit_box_.w, 6000 }, BlockComponent::Type::SOLID ) )
			{
				direction_ = Direction::Simple::LEFT;
			}
		}
		break;
		case ( Direction::Simple::LEFT ):
		{
			hit_box_.x -= MOVEMENT_SPEED;
			if ( blocks.blocksInTheWay( { hit_box_.x - 6000, hit_box_.y, 6000, hit_box_.h }, BlockComponent::Type::SOLID ) )
			{
				direction_ = Direction::Simple::UP;
			}
		}
		break;
	}
};

void PufferbeeSquareSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{};

void PufferbeeSquareSprite::reset()
{
	direction_ = start_direction_;
	hit_box_.x = original_hit_box_.x;
	hit_box_.y = original_hit_box_.y;
};
