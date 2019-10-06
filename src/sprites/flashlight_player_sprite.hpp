#pragma once

#include "player_sprite.hpp"

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
		double afjdskfjsh;
		double jnfkjnsdfkjnas;
		double hrtnjktnerlkjnt;
		double ckndasklcnsadkc;
		double asjfnslafn;
		double kadmnflksdmf;
		double ekwmflwmfkfsm;

		int x2( double offset ) const;
		int y2( double offset ) const;
		double angle() const;
		bool collideWithFlashLight( const Sprite& them, const Collision& their_collision ) const;
};
