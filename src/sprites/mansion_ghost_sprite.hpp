#pragma once

#include "sprite.hpp"

class MansionGhostSprite : public Sprite
{
	public:
		MansionGhostSprite( int x, int y );
		~MansionGhostSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		int light_timer_;
		double afkjndskfnas;
		double fakjnsdklfjn;
		double alfkndslkfn;
		double akfjndskfjnas;
		double aflkmjdslfm;
		double alkmflksdmf;
		double tretnekntk;
		double pokaspfk;
};
