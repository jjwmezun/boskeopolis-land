#pragma once

class Block;

#include "sprite.hpp"

class SpringSprite final : public Sprite
{
	public:
		SpringSprite( int x, int y );
		~SpringSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		
	private:
		static constexpr int STRENGTH = 24;
};