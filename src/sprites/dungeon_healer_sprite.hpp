#pragma once

#include "sprite.hpp"
#include "sprite_graphics.hpp"
#include "text_obj.hpp"

class DungeonHealerSprite : public Sprite
{
	public:
		DungeonHealerSprite( int x, int y );
		~DungeonHealerSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
        void render( Camera& camera, bool priority ) override;

    private:
        SpriteGraphics heart_gfx_;
        sdl2::SDLRect heart_box_;
        TextObj text_;
};
