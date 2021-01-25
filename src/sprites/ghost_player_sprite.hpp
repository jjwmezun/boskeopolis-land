#pragma once

class Block;

#include <array>
#include "player_sprite.hpp"

class GhostPlayerSprite final : public PlayerSprite
{
	public:
		GhostPlayerSprite();
		~GhostPlayerSprite();
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );

    private:
        std::vector<std::array<bool, Input::NUM_O_ACTIONS>> player_sequence_;
        int timer_;
        char scratch_[ 3000 ];
};