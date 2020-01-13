#pragma once

#include "sprite.hpp"

class BridgeMonsterProjectileSprite : public Sprite
{
	public:
		BridgeMonsterProjectileSprite( int x, int y, double dy, double dx );
		~BridgeMonsterProjectileSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
	
	private:
		const double angle_;
};