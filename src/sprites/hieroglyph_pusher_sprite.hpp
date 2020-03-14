#pragma once

class Block;

#include "sprite.hpp"

class HieroglyphPusherSprite final : public Sprite
{
	public:
		HieroglyphPusherSprite( int x, int y );
		~HieroglyphPusherSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
};