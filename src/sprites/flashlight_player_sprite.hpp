#pragma once

#include <array>
#include "player_sprite.hpp"
#include "sprite_graphics.hpp"

class FlashlightPlayerSprite : public PlayerSprite
{
	public:
		FlashlightPlayerSprite( int x, int y );
		~FlashlightPlayerSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void render( Camera& camera, bool priority = false ) override;

	private:
		double angle_;
		SpriteGraphics flashlight_gfx_;
		sdl2::SDLRect flashlight_box_;
		double ekwmflwmfkfsm;

		int x2( int center_x, double offset = 0.0 ) const;
		int y2( int center_y, double offset = 0.0 ) const;
		double angle() const;
		bool collideWithFlashLight( const Sprite& them, const Collision& their_collision ) const;
		std::array<Line, 3> getLines() const;
};
