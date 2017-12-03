#pragma once

#include "sprite.hpp"
#include <memory>

class CannonSprite : public Sprite
{
	public:
		CannonSprite( int x, int y, Direction::Vertical dir, int map_id );
		~CannonSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
	
	private:
		bool deactivated_;
		int shoot_time_;
		int shoot_counter_;
		int hp_;
		int flash_time_;

		int shootTime() const;
		void handleShooting( SpriteSystem& sprites, Direction::Simple dir );
		void hurt();
		
		static int typeGFX( Direction::Vertical dir );
};