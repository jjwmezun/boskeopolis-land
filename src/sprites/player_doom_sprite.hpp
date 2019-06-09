#pragma once

#include "sprite.hpp"
#include "sprite_graphics.hpp"
#include <memory>

class PlayerDoomSprite : public Sprite
{
	public:
		static constexpr int CONVERSION_PRECISION = 1000000;

		PlayerDoomSprite( int x, int y );
		~PlayerDoomSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void render( Camera& camera, bool priority ) override;
		void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap ) override;

	private:
		SDL_Color screen_tint_;
		double posx_;
		double posy_;
		double ddirx_;
		double ddiry_;
		double planex_;
		double planey_;
		double prevdirx_;
		double prevdiry_;
		double angle_;
		int prevposx_;
		int prevposy_;
		int shoot_timer_;

		void rotate( double rotation_speed );
		double getAccelerationAdjustedByAngle( double angle );
		void moveStraight( double multiplier, double angle );
		void updateHurtAnimation( const Health& health );
};
