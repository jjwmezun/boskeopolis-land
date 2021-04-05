#pragma once

#include "sprite.hpp"

class WaterfallPlatformsSprite final : public Sprite
{
	public:
		WaterfallPlatformsSprite( int x, int y, Direction::Vertical direction, int space_between_platforms = 4, int speed = 1500 );
		~WaterfallPlatformsSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
        void render( const Camera& camera ) const override;

    private:
        static constexpr int MAX_PLATFORMS = 5;

        sdl2::SDLRect platforms_[ MAX_PLATFORMS ];
        int space_between_platforms_;
        bool platforms_on_camera_[ MAX_PLATFORMS ];
        char scratch_[ 3000 ];
};
