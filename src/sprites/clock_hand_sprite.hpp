#pragma once

#include "circle_movement.hpp"
#include "sprite.hpp"

class ClockHandSprite final : public Sprite
{
	public:
		ClockHandSprite( int x, int y, Direction::Clockwise direction );
		~ClockHandSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( const Camera& camera ) const override;

    private:
        CircleMovement inner_left_;
        CircleMovement inner_right_;
        CircleMovement outer_left_;
        CircleMovement outer_right_;
        sdl2::SDLRect image_box_;
        double graphics_rotation_amount_;
        char scratch_[3000];
};
