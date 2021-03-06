#pragma once

class Block;

#include "sprite.hpp"

class LockedDoorSprite final : public Sprite
{
	public:
		LockedDoorSprite( int x, int y, int map_id );
		~LockedDoorSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( const Camera& camera ) const override;
		
	private:
		bool has_key_;
		bool opening_;
};