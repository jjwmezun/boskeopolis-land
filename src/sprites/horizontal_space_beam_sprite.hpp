#pragma once

#include "sprite.hpp"

class HorizontalSpaceBeamSprite final : public Sprite
{
	public:
		HorizontalSpaceBeamSprite( int x, int y );
		~HorizontalSpaceBeamSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        enum class BeamState
        {
            INACTIVE,
            WAITING,
            WARNING,
            GROWING,
            SHOOTING,
            SHRINKING
        };
        BeamState beam_state_;
        int timer_;
        char scratch_[3000];

        void updateHeight( int height );
};
