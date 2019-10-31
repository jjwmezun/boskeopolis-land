#pragma once

#include "sprite.hpp"

class MansionDoorSprite : public Sprite
{
	public:
		MansionDoorSprite( int x, int y );
		~MansionDoorSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
    
    private:
        enum class MansionDoorState
        {
            CLOSED,
            OPENING,
            OPEN
        };
        MansionDoorState state_;
        int animation_timer_;
        int current_animation_frame_;
};
