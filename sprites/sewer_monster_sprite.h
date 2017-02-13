


// Name
//===================================
//
// SewerMonsterSprite
//

#ifndef SEWER_MONSTER_SPRITE_H
#define SEWER_MONSTER_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    class Input;


// DEPENDENCIES
//===================================

    #include "sprite.h"


// CLASS
//===================================

    class SewerMonsterSprite : public Sprite
    {
        public:
            SewerMonsterSprite( int x, int y );
            ~SewerMonsterSprite();
            void customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites );

        private:
            static const int DELAY_TIME = 64;
            bool awake_;
            bool attacking_;
            bool after_attack_;
            TimerSimple delay_;
            TimerSimple attack_length_;
            TimerSimple after_attack_delay_;
    };


#endif // SEWER_MONSTER_SPRITE_H

