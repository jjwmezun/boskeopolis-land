#pragma once

#include "sprite.hpp"

class AtticMonsterSprite final : public Sprite
{
	public:
		AtticMonsterSprite( int x, int y );
		~AtticMonsterSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void reset() override;

    private:
		enum class State
		{
			DORMANT,
			RISING,
			BEFORE_THROW,
			THROW,
			AFTER_THROW,
			LOWERING,
			DELAYED
		};

		bool testIsNear( const Sprite& them ) const;

        int timer_;
		State state_;
        char scratch_[ 3000 ];
};
