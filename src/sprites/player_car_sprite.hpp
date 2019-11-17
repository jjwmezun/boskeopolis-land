#pragma once

#include "sprite.hpp"

class PlayerCarSprite : public Sprite
{
	public:
		PlayerCarSprite( int x, int y );
		~PlayerCarSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void render( Camera& camera, bool priority ) override;

	private:
		double angle_;
		double speed_;
		double acceleration_;
		double previous_angle_[ 2 ];
		int x_prev_prev_[ 2 ];
		int y_prev_prev_[ 2 ];
};