

// Name
//===================================
//
// OverworldPlayerSprite
//

#ifndef OVERWORLD_PLAYER_SPRITE_H
#define OVERWORLD_PLAYER_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    class Input;
    class Sprite;


// DEPENDENCIES
//===================================

    #include "sprite.h"


// CLASS
//===================================

    class OverworldPlayerSprite : public Sprite
    {
        public:
            OverworldPlayerSprite( int x, int y );
            ~OverworldPlayerSprite();
            void customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
			
		protected:
			void deathAction( Camera& camera );
			
		private:
			static constexpr int DEATH_SPIN_SPEED = 20;
			int death_spins_;
    };


#endif // OVERWORLD_PLAYER_SPRITE_H
