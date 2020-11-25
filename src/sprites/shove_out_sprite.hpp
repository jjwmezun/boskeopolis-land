#pragma once

#include "sprite.hpp"

class ShoveOutSprite final : public Sprite
{
	public:
		ShoveOutSprite( int x, int y );
		~ShoveOutSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( const Camera& camera ) const override;

    private:
        enum class Status
        {
            WAITING_BEFORE_GOING_OUT,
            GOING_OUT,
            WAITING_BEFORE_GOING_IN,
            GOING_IN
        };
        Status status_;
        int timer_;
        sdl2::SDLRect image_box_;
        char scratch_[3000];
};
