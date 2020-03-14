#pragma once

#include "sprite.hpp"
#include <memory>

class CannonSprite final : public Sprite
{
	public:
		CannonSprite( int x, int y, Direction::Vertical dir, int map_id );
		~CannonSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
	
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