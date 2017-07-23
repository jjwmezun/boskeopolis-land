#pragma once

class Block;
class Input;
class Sprite;

#include "sprite.h"

class OverworldPlayerSprite : public Sprite
{
	public:
		OverworldPlayerSprite( int x, int y );
		~OverworldPlayerSprite();
		void customUpdate( const Input& input, Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		void OWUpdate( const Input& input, Camera& camera, Map& lvmap );
		void sharedMovement( const Input& input, Camera& camera, Map& lvmap );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );

	protected:
		void deathAction( Camera& camera );

	private:
		static constexpr int DEATH_SPIN_SPEED = 20;
		int death_spins_;
};