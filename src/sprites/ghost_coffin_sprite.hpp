#pragma once

#include "sprite.hpp"

class GhostCoffinSprite final : public Sprite
{
	public:
		GhostCoffinSprite( int x, int y );
		~GhostCoffinSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void reset() override;

    private:
        bool done_;
};
