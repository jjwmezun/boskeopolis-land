#pragma once

class Block;
class Sprite;

#include "sprite.hpp"

class MazePlayerSprite final : public Sprite
{
	public:
		MazePlayerSprite( int x, int y );
		~MazePlayerSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap );

	private:
		int death_spins_;
};
