#pragma once

class Block;

#include "player_sprite.hpp"

class ShooterPlayerSprite : public PlayerSprite
{
	public:
		ShooterPlayerSprite( int x, int y );
		~ShooterPlayerSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;

	private:
		int shoot_delay_count_;
};