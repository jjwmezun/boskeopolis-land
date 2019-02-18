#pragma once

class Block;

#include "sprite.hpp"

class PufferbeeSquareSprite : public Sprite
{
    public:
        PufferbeeSquareSprite
		(
			int x,
			int y,
            Direction::Simple start_direction
		);
        ~PufferbeeSquareSprite();
        void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
        void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
        void reset() override;

    private:
        const Direction::Simple start_direction_;
        Direction::Simple direction_;
};
