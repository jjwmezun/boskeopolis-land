#pragma once

#include "sprite.hpp"

class DownhillGemSprite final : public Sprite
{
	public:
		DownhillGemSprite( int x, int y );
		~DownhillGemSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};