#pragma once

class Block;
class Sprite;

#include "sprite.hpp"

class MazePlayerSprite : public Sprite
{
	public:
		MazePlayerSprite( int x, int y );
		~MazePlayerSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health ) override;

	protected:
		void deathAction( Camera& camera );

	private:
		static constexpr int DEATH_SPIN_SPEED = 20;
		int death_spins_;
};