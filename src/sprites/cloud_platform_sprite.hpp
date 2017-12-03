#pragma once

#include "sprite.hpp"
#include "timers/timer_simple.hpp"

class CloudPlatformSprite : public Sprite
{
	public:
		CloudPlatformSprite( int x, int y, Direction::Horizontal direction );
		~CloudPlatformSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		bool started_;
};