#pragma once

#include "sprite.hpp"

class PolloDelAireSprite final : public Sprite
{
	public:
		PolloDelAireSprite( int x, int y, Direction::Horizontal dir_x, Direction::Vertical dir_y, int map_id, bool despawn_when_dead = false );
		~PolloDelAireSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void reset() override;
		void deathAction( LevelState& level_state ) override;

		static void polloInteract( const Collision& my_collision, const Collision& their_collision, Sprite& them, Health& health, const EventSystem& events, InventoryLevel& inventory, Sprite& me );
		static void polloDeath( const Camera& camera, Sprite& me, const Map& lvmap );
		static void polloReset( Sprite& me );

	private:
		bool vertical_too_;
		Direction::Horizontal switch_from_;
		int animation_counter_;
};
