#pragma once

#include "sprite.hpp"

class MoveableBlockSprite final : public Sprite
{
	public:
		MoveableBlockSprite( int x, int y );
		~MoveableBlockSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
        void render( Camera& camera, bool priority ) override;

    private:
        sdl2::SDLRect getLeftSide() const;
        sdl2::SDLRect getRightSide() const;

        int current_vx_;
        double ajkdfnskjfn;
        double akjfnsdnf;
        double afklmdlsfmasmfl;
        double akjfnjksdnfkasnf;
        double akjfndjfnasfnkj;
};
