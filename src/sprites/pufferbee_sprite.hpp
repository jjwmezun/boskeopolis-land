#ifndef PUFFERBEE_SPRITE_H
#define PUFFERBEE_SPRITE_H

class Block;

#include "sprite.hpp"

class PufferbeeSprite : public Sprite
{
    public:
        PufferbeeSprite
		(
			int x,
			int y,
			std::unique_ptr<SpriteComponent> component = nullptr
		);
        ~PufferbeeSprite();
        void customUpdate( LevelState& level_state );
        void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );

    private:
        Direction::Rotation randomDirection() const;
};

#endif // PUFFERBEE_SPRITE_H
