#pragma once

#include <array>
#include "player_sprite.hpp"
#include "sprite_graphics.hpp"

class FlashlightPlayerSprite : public PlayerSprite
{
	public:
		FlashlightPlayerSprite( int x, int y );
		~FlashlightPlayerSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( const Camera& camera ) const override;

	private:
		SpriteGraphics flashlight_gfx_;
		SpriteGraphics flash_beam_gfx_;
		sdl2::SDLRect flashlight_box_;
		double angle_;

		int x2( int center_x, double offset = 0.0 ) const;
		int y2( int center_y, double offset = 0.0 ) const;
		double angle() const;
		bool collideWithFlashLight( const Sprite& them, const Collision& their_collision ) const;
		std::array<Line, 3> getLines() const;
};
