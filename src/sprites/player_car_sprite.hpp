#pragma once

#include "sprite.hpp"

class PlayerCarSprite : public Sprite
{
	public:
		PlayerCarSprite( int x, int y );
		~PlayerCarSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		double angle_;
		double speed_;
		double acceleration_;
};