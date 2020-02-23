#pragma once

class Block;

#include "sprite.hpp"

class FallingFistSprite : public Sprite
{
	public:
		FallingFistSprite( int x, int y );
		~FallingFistSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( Camera& camera, bool priority = false ) override;

	private:
		void drawCrane( const Camera& camera );
};