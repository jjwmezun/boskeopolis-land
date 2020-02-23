#pragma once

#include "input.hpp"
#include "sprite.hpp"

class UrbanBirdSprite : public Sprite
{
	public:
		UrbanBirdSprite( int x, int y, Sprite* hero_address );
		~UrbanBirdSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		enum class InputType
		{
			__NULL,
			RIGHT = Input::Action::MOVE_RIGHT,
			LEFT = Input::Action::MOVE_LEFT
		};

		bool left_pressed_before_;
		bool right_pressed_before_;
		bool jump_pressed_before_;
		int reset_timer_;
		int reset_delay_;
		int remember_x_;
		int remember_y_;
		int struggle_counter_;
		Sprite* hero_address_;

		void setNewPosition( const Camera& camera, const Map& lvmap );
};
