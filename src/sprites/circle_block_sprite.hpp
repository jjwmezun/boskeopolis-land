#pragma once

#include "sprite.hpp"

class CircleBlockSprite : public Sprite
{
	public:
		CircleBlockSprite( int x, int y );
		~CircleBlockSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		static constexpr int NUMBER_OF_FRAMES = 12;
		const int frames_[ NUMBER_OF_FRAMES ];
		Direction::Simple direction_;
		int animation_timer_;
};
