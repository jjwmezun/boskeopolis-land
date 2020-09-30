#pragma once

#include "sprite.hpp"

class TruckPlatformSprite final : public Sprite
{
	public:
		TruckPlatformSprite( int x, int y );
		~TruckPlatformSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void reset() override;
		void render( const Camera& camera ) const override;
};
