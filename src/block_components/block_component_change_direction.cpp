#include "audio.hpp"
#include "block_component_change_direction.hpp"
#include "sprite.hpp"

BlockComponentChangeDirection::BlockComponentChangeDirection( Direction::Simple direction )
:
	direction_ ( direction )
{};

BlockComponentChangeDirection::~BlockComponentChangeDirection() {};

void BlockComponentChangeDirection::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites )
{
	switch ( direction_ )
	{
		case ( Direction::Simple::LEFT ):
		case ( Direction::Simple::RIGHT ):
			sprite.direction_x_ = Direction::simpleToHorizontal( direction_ );
			sprite.direction_y_ = Direction::Vertical::__NULL;
		break;

		case ( Direction::Simple::UP ):
		case ( Direction::Simple::DOWN ):
			sprite.direction_y_ = Direction::simpleToVertical( direction_ );
			sprite.direction_x_ = Direction::Horizontal::__NULL;
		break;
	}

	if ( sprite.hasType( Sprite::SpriteType::HERO ) )
	{
		Audio::playSound( Audio::SoundType::BOUNCE );
	}
};
