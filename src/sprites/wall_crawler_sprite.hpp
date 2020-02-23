#pragma once

#include "sprite.hpp"

class WallCrawlerSprite : public Sprite
{
	public:
		WallCrawlerSprite( int x, int y );
		~WallCrawlerSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
        void render( Camera& camera, bool priority ) override;

    private:
        sdl2::SDLRect getFirstRect() const;
        sdl2::SDLRect getSecondRect() const;

        int fall_delay_;
        int asfnkasdnfkjsn;
        double alnmflksdmfkl;
        double klmnwlkrmlwkerm;
        double alkmflksdmf;
        double klmlkmlkm;
        double alkmdsflksmdlkf;
        double alkdmsflksdmfl;
        double asdknflskanflsmf;
};
