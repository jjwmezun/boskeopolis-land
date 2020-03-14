#pragma once

#include "timers/timer_simple.hpp"
class Collision;
class Sprite;

class BumpUnderBlockSprite final
{
	public:
		BumpUnderBlockSprite();
		void update( Sprite& sprite );
		bool testHit( Sprite& me, Sprite& sprite, const Collision& collision );

	private:
		TimerSimple delay_;
		bool ready_;
};
