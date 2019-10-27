#pragma once

#include "sprite.hpp"

class GhostCoffinSprite : public Sprite
{
	public:
		GhostCoffinSprite( int x, int y );
		~GhostCoffinSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void reset() override;

    private:
        bool done_;
};
