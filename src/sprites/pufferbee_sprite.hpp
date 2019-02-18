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
        void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
        void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );

    private:
        Direction::Rotation randomDirection() const;
};

#endif // PUFFERBEE_SPRITE_H
