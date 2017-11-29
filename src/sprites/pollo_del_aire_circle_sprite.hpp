#pragma once

#include "sprite.hpp"

class PolloDelAireCircleSprite : public Sprite
{
	public:
		friend class PolloDelAireSprite;
		PolloDelAireCircleSprite( int x, int y, bool clockwise = true );
		~PolloDelAireCircleSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health ) override;
		void deathAction( Camera& camera, EventSystem& events ) override;
		void reset() override;

	private:
		static constexpr int ANGLE_SPEED_CONVERSION = 1000;
		static constexpr int RADIUS = 5000;
		static constexpr int SPEED = 500;

		bool clockwise_;
		int animation_counter_;
		int angle_;

		inline double angle()
		{
        	return ( double )( angle_ ) / ANGLE_SPEED_CONVERSION;
		};
};