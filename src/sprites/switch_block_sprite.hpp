#pragma once

#include "sprite.hpp"
#include "timers/timer_simple.hpp"

class SwitchBlockSprite : public Sprite
{
	public:
		SwitchBlockSprite( int x, int y );
		~SwitchBlockSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		TimerSimple delay_;
		bool ready_ = true;
};
