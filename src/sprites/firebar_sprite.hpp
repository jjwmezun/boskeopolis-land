#pragma once

#include <array>
#include "circle_movement.hpp"
#include "sprite.hpp"

class FirebarSprite : public Sprite
{
	public:
		FirebarSprite( int x, int y );
		~FirebarSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( Camera& camera, bool priority ) override;

	private:
		static constexpr int NUMBER_OF_EXTRA_FIREBALLS = 2;
		std::array<CircleMovement, NUMBER_OF_EXTRA_FIREBALLS> circles_;
};
