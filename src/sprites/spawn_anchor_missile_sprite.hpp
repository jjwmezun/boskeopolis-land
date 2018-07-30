#pragma once

#include "sprite.hpp"

class SpawnAnchorMissileSprite : public Sprite
{
	public:
		enum class AMType
		{
			LEFT_AND_RIGHT = 0,
			UP             = 1,
			RIGHT          = 2,
			DOWN           = 3,
			LEFT           = 4
		};

		SpawnAnchorMissileSprite( int x, int y, AMType type );
		~SpawnAnchorMissileSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;

	private:
		const AMType type_;
		int timer_;
		int target_time_;
};
