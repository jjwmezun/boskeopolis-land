#pragma once

#include "sprite.hpp"

class SpawnAnchorMissileSprite final : public Sprite
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
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		const AMType type_;
		int timer_;
		int target_time_;
};
