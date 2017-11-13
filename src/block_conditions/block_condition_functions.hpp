#pragma once

#include "block_condition.hpp"
#include "block.hpp"
#include "collision.hpp"
#include "sprite_graphics.hpp"

class EventSystem;
class Sprite;

namespace BlockConditionFunctions
{
	bool CollideAny   ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events );
	bool CollideBottom( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events );
	bool CollideLeft  ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events );
	bool CollideRight ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events );
	bool CollideTop   ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events );
	bool Enemy        ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events );
	bool FadeCloud    ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events );
	bool Hero         ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events );
	bool Key          ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events );
	bool NotDucking   ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events );
	bool OnGround     ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events );
	bool Rival        ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events );
	bool SwitchOff    ( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events );

	template<int threshold>
	bool FadeOut( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events )
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
	bool CollideSpecific( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events )
	{
		return
			( collision.overlapXLeft()   > left  ||
			collision.overlapXRight()  > right ) &&
			( collision.overlapYTop()    > top   ||
			collision.overlapYBottom() > bottom );
	};
};