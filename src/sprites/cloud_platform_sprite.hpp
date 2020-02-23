#pragma once

#include "sprite.hpp"

class CloudPlatformSprite : public Sprite
{
	public:
		CloudPlatformSprite( int x, int y, Direction::Horizontal direction );
		~CloudPlatformSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		bool started_;
};