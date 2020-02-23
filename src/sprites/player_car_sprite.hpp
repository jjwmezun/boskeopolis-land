#pragma once

#include "sprite.hpp"

class PlayerCarSprite : public Sprite
{
	public:
		PlayerCarSprite( int x, int y );
		~PlayerCarSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( Camera& camera, bool priority ) override;
		void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap ) override;

	private:
		double angle_;
		double speed_;
		double acceleration_;
		std::vector<const Block*> block_list_;

		bool testForCollisions( int x, int y, const Camera& camera, const BlockSystem& blocks, const EventSystem& events, const Health& health );
};