#pragma once

#include "sprite.hpp"

class PolloDelAireStillSprite : public Sprite
{
	public:
		friend class PolloDelAireSprite;
		PolloDelAireStillSprite( int x, int y );
		~PolloDelAireStillSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health ) override;
		void deathAction( Camera& camera ) override;
		void reset() override;

	private:
		int animation_counter_;
};