#include "block_condition_functions.hpp"
#include "event_system.hpp"
#include "sprite.hpp"

namespace BlockConditionFunctions
{
	bool CollideAny( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
	{
		return collision.collideAny();
	};
	bool CollideBottom( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
	{
		return sprite.collideBottomOnly( collision, block );
	};
	bool CollideLeft( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
	{
		return collision.collideLeft();
	};
	bool CollideRight( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
	{
		return collision.collideRight();
	};
	bool CollideTop( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
	{
		return sprite.collideTopOnly( collision, block );
	};
	bool Enemy( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
	{
		return sprite.hasType( Sprite::SpriteType::ENEMY );
	};
	bool FadeCloud( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
	{
		if ( block.type() != nullptr && block.type()->graphics() != nullptr )
		{
			return ( block.type()->graphics()->current_frame_x_ != 4*16 || block.type()->graphics()->current_frame_y_ != 16 ) &&
				( block.type()->graphics()->current_frame_y_ != 16*3 );
		}
		else
		{
			return false;
		}
	};
	bool Hero( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
	{
		return sprite.hasType( Sprite::SpriteType::HERO );
	};
	bool Key( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
	{
		return events.hasKey();
	};
	bool NotDucking( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
	{
		return !sprite.isDucking();
	};
	bool OnGround( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
	{
		return sprite.onGround();
	};
	bool Rival( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
	{
		return sprite.hasType( Sprite::SpriteType::RIVAL );
	};
	bool SwitchOff( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
	{
		return !events.switch_;
	};
};