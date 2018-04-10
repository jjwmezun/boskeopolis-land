#pragma once

#include "sprite.hpp"

class RisingIceCubeSprite : public Sprite
{
	public:
		RisingIceCubeSprite( int x, int y, int height_limit = 8 );
		~RisingIceCubeSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		enum class State
		{
			PAUSED,
			RISING,
			FALLING
		};
		State state_;
		int height_limit_;
};