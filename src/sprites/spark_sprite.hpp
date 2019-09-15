#pragma once

#include "sprite.hpp"

class SparkSprite : public Sprite
{
	public:
		SparkSprite( int x, int y );
		~SparkSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void reset() override;

	private:
		int hit_wall_position_;
		int sdfsaf;
		double njaksdfnfas;
		double jknwerwejkrn;
		double cdsancsajknc;
};
