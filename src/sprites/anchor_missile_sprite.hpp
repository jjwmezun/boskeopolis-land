#pragma once

#include "direction.hpp"
#include "sprite.hpp"

class AnchorMissileSprite : public Sprite
{
	public:
		AnchorMissileSprite( int x, int y, Direction::Simple direction );
		~AnchorMissileSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		const Direction::Simple direction_;
};
