#pragma once

#include "sprite.hpp"
#include "sprite_component.hpp"

class PufferbeeSprite final : public Sprite
{
    public:
        PufferbeeSprite
		(
			int x,
			int y,
			std::unique_ptr<SpriteComponent> component = nullptr,
            Unit::Layer layer = Unit::Layer::SPRITES_1
		);
        ~PufferbeeSprite();
        void customUpdate( LevelState& level_state ) override;
        void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
        void reset() override;

    private:
        Direction::Rotation randomDirection() const;

        std::unique_ptr<SpriteComponent> component_;
};
