#pragma once

#include "sprite.hpp"

class WaterdropSpawnerSprite final : public Sprite
{
	public:
		WaterdropSpawnerSprite( int x, int y );
		~WaterdropSpawnerSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );
		
	private:
		int timer_;
};