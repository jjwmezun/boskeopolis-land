#pragma once

#include "sprite.hpp"

class PolloDelAireCircleSprite : public Sprite
{
	public:
		friend class PolloDelAireSprite;
		PolloDelAireCircleSprite( int x, int y, bool clockwise, int map_id, bool despawn_when_dead = false );
		~PolloDelAireCircleSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap ) override;
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
