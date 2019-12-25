#pragma once

#include "sprite.hpp"

class SwampPoleSprite : public Sprite
{
	public:
		SwampPoleSprite( int x, int y, int id );
		~SwampPoleSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

    private:
        int bottomLimit() const;

        Sprite* pair_;
        double ajfkndsjknfk;
        double akjdfnaskjdnf;
        double nflkmsdlfkamns;
        double alkfdmnlksdmfl;
        double alkfmddlksmfal;
        double alkfnsldknf;
        double lkmewrlkmwelkrmwelk;
};
