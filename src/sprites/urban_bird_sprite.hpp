#pragma once

#include "sprite.hpp"

class UrbanBirdSprite : public Sprite
{
	public:
		UrbanBirdSprite( int x, int y );
		~UrbanBirdSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		int reset_timer_;
		int reset_delay_;
		int remember_x_;
		int remember_y_;
		void setNewPosition( const Camera& camera, const Map& lvmap );
};
