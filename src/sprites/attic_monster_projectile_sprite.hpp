#pragma once

#include "sprite.hpp"

class AtticMonsterProjectileSprite final : public Sprite
{
	public:
		AtticMonsterProjectileSprite( int x, int y, double dx, double dy );
		~AtticMonsterProjectileSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        const double angle_;
        char scratch_[3000];
};
