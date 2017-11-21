#pragma once

#include "block_condition.hpp"
#include "block.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

class EventSystem;
class Sprite;

namespace BlockConditionFunctions
{
	bool CollideAny   ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health );
	bool CollideBottom( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health );
	bool CollideLeft  ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health );
	bool CollideRight ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health );
	bool CollideTop   ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health );
	bool Enemy        ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health );
	bool FadeCloud    ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health );
	bool Hero         ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health );
	bool Key          ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health );
	bool NotDucking   ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health );
	bool OnGround     ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health );
	bool Rival        ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health );
	bool SwitchOff    ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health );

	template<int threshold>
	bool FadeOut( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
	{
		if ( block.type() != nullptr && block.type()->graphics() != nullptr )
		{
			return block.type()->graphics()->alpha_ > threshold;
		}
		else
		{
			return false;
		}
	};

	template<int left, int right, int top, int bottom>
	bool CollideSpecific( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events, const Health& health )
	{
		return
			( collision.overlapXLeft()   > left  ||
			collision.overlapXRight()  > right ) &&
			( collision.overlapYTop()    > top   ||
			collision.overlapYBottom() > bottom );
	};
};