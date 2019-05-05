#pragma once

#include <array>
#include "circle_movement.hpp"
#include "sprite.hpp"

class FirebarSprite : public Sprite
{
	public:
		FirebarSprite( int x, int y );
		~FirebarSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void render( Camera& camera, bool priority ) override;

	private:
		static constexpr int NUMBER_OF_EXTRA_FIREBALLS = 2;
		std::array<CircleMovement, NUMBER_OF_EXTRA_FIREBALLS> circles_;
};
