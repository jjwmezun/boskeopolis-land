#pragma once

#include "animation_counter_no_repeat.hpp"
#include "sprite.hpp"

class MansionDoorSprite : public Sprite
{
	public:
		MansionDoorSprite( int x, int y );
		~MansionDoorSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
    
    private:
        static constexpr int NUMBER_OF_FRAMES = 8;

        enum class MansionDoorState
        {
            CLOSED,
            OPENING,
            OPEN
        };
        AnimationCounterNoRepeat<NUMBER_OF_FRAMES> animation_counter_;
        MansionDoorState state_;
};
