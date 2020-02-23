#pragma once

#include "sprite.hpp"

class RisingPackageSprite : public Sprite
{
	public:
		RisingPackageSprite( int x, int y );
		~RisingPackageSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void reset() override;

	private:
		bool rise_;
};
