#pragma once

#include "sprite.hpp"

class WallCrawlerSprite final : public Sprite
{
	public:
		WallCrawlerSprite( int x, int y );
		~WallCrawlerSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
        void render( const Camera& camera ) const override;

    private:
        sdl2::SDLRect getFirstRect() const;
        sdl2::SDLRect getSecondRect() const;

        int fall_delay_;
        char scratch_[ 3000 ];
};
