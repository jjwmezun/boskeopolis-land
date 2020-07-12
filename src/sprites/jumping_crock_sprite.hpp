#pragma once

#include "sprite.hpp"

class JumpingCrockSprite final : public Sprite
{
	public:
		JumpingCrockSprite( int x, int y );
		~JumpingCrockSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

    private:
        enum class CrockState
        {
            STILL,
            JUMPING,
            CRESTING,
            FALLING
        };
        CrockState crock_state_;
        char scratch_[ 3000 ];
};
