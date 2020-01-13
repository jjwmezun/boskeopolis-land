#pragma once

#include "sprite.hpp"

class MoveableBlockSprite : public Sprite
{
	public:
		MoveableBlockSprite( int x, int y );
		~MoveableBlockSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
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
