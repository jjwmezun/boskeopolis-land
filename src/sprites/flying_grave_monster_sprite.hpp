#pragma once

#include "sprite.hpp"

class FlyingGraveMonsterSprite final : public Sprite
{
	public:
		FlyingGraveMonsterSprite( int x, int y );
		~FlyingGraveMonsterSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        char scatch_[ 3000 ];
};
