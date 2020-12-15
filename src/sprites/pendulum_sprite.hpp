#pragma once

#include "circle_movement.hpp"
#include "sprite.hpp"

class PendulumSprite final : public Sprite
{
	public:
		PendulumSprite( int x, int y, Direction::Horizontal start );
		~PendulumSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
    void render( const Camera& camera ) const override;

    private:
        CircleMovement movement_;
        bool keep_player_;
        int prev_x_;
        bool keep_player_x_;
        bool keep_player_x2_;
        sdl2::SDLRect solid_ball_;
        int max_;
        int min_;
        char scratch_[3000];
};
