#pragma once

#include "sprite.hpp"

class BridgeMonsterProjectileSprite : public Sprite
{
	public:
		BridgeMonsterProjectileSprite( int x, int y, double dy, double dx );
		~BridgeMonsterProjectileSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
	
	private:
		const double angle_;
};