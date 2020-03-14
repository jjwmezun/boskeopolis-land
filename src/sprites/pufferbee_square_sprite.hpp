#pragma once

class Block;

#include "sprite.hpp"

class PufferbeeSquareSprite final : public Sprite
{
    public:
        PufferbeeSquareSprite
		(
			int x,
			int y,
            Direction::Simple start_direction
		);
        ~PufferbeeSquareSprite();
        void customUpdate( LevelState& level_state ) override;
        void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
        void reset() override;

    private:
        const Direction::Simple start_direction_;
        Direction::Simple direction_;
};
