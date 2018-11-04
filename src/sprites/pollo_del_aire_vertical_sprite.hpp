#pragma once

#include "sprite.hpp"

class PolloDelAireVerticalSprite : public Sprite
{
	public:
		friend class PolloDelAireSprite;
		PolloDelAireVerticalSprite( int x, int y, Direction::Vertical dir_y, int map_id, bool despawn_when_dead = false );
		~PolloDelAireVerticalSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap ) override;
		void reset() override;

	private:
		int animation_counter_;
};
