#pragma once

#include "sprite.hpp"

class SwampPoleSprite : public Sprite
{
	public:
		SwampPoleSprite( int x, int y, int id );
		~SwampPoleSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

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
