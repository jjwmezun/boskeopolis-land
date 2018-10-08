#pragma once

#include "input.hpp"
#include "sprite.hpp"

class UrbanBirdSprite : public Sprite
{
	public:
		UrbanBirdSprite( int x, int y );
		~UrbanBirdSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		enum class InputType
		{
			__NULL,
			RIGHT = Input::Action::MOVE_RIGHT,
			LEFT = Input::Action::MOVE_LEFT
		};

		int reset_timer_;
		int reset_delay_;
		int remember_x_;
		int remember_y_;
		int struggle_counter_;
		InputType last_pressed_;
		void setNewPosition( const Camera& camera, const Map& lvmap );
};
