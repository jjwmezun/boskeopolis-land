#pragma once

#include "sprite.hpp"

class StatueLaserSprite final : public Sprite
{
	public:
		StatueLaserSprite( int x, int y );
		~StatueLaserSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
        void render( Camera& camera, bool priority ) override;

    private:
        enum class LaserState
        {
            DORMANT,
            START_CHARGING,
            CHARGING,
            CHARGING2,
            SHOOTING,
            START_FADING,
            FADING,
            FADING2
        };
        LaserState laser_state_;
        int timer_;
        char scratch_[ 3000 ];

        bool testInFrontOfLaser( const Sprite& them ) const;
        bool testTouchingLaser( const Sprite& them ) const;
        void changeStateAfterDelay( int delay, LaserState next_state );
};
