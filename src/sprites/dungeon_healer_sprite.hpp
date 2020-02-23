#pragma once

#include "sprite.hpp"
#include "sprite_graphics.hpp"
#include "text_obj.hpp"

class DungeonHealerSprite : public Sprite
{
	public:
		DungeonHealerSprite( int x, int y );
		~DungeonHealerSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
        void render( Camera& camera, bool priority ) override;
        void reset() override;

    private:
        bool invincibilityFlickerOff() const;

        SpriteGraphics laser_gfx_;
        SpriteGraphics heart_gfx_;
        sdl2::SDLRect laser_box_;
        sdl2::SDLRect heart_box_;
        TextObj text_;
        int timer_;
};
