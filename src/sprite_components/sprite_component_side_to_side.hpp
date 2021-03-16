#pragma once

#include "direction.hpp"
#include "sprite_component.hpp"

class SpriteComponentSideToSide final : public SpriteComponent
{
    public:
        SpriteComponentSideToSide( int move_space_before_change, Direction::Simple direction );
        ~SpriteComponentSideToSide();
        void update( Sprite& sprite, LevelState& state ) override;
        void reset();

    private:
        const int past_top_for_change_;
        const int past_bottom_for_change_;
        Direction::Simple direction_;
        const Direction::Simple start_direction_;
};
