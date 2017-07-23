#pragma once

class Block;

class Sprite;

#include "sprite.hpp"

class OverworldPlayerSprite : public Sprite
{
	public:
		OverworldPlayerSprite( int x, int y );
		~OverworldPlayerSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		void OWUpdate( Camera& camera, Map& lvmap );
		void sharedMovement( Camera& camera, Map& lvmap );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );

	protected:
		void deathAction( Camera& camera );

	private:
		static constexpr int DEATH_SPIN_SPEED = 20;
		int death_spins_;
};