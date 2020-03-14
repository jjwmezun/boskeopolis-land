#pragma once

#include "sprite.hpp"

class SparkSprite final : public Sprite
{
	public:
		SparkSprite( int x, int y );
		~SparkSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void reset() override;

	private:
		int hit_wall_position_;
		int sdfsaf;
		double njaksdfnfas;
		double jknwerwejkrn;
		double cdsancsajknc;
};
