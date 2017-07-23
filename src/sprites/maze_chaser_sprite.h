




// Name
//===================================
//
// MazeChaserSprite
//

#ifndef MAZE_CHASER_SPRITE_H
#define MAZE_CHASER_SPRITE_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "sprite.h"


// CLASS
//===================================

    class MazeChaserSprite : public Sprite
    {
        public:
			enum class Type
			{
				SHADOW,
				AMBUSH,
				RANDO,
				OBFUSCATING
			};
			
			enum class ChaserState
			{
				CHASE,
				SCATTER
			};
		
            MazeChaserSprite( int x, int y, Type type );
            ~MazeChaserSprite();
            void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
			
			
		private:
			// For std::pairs.
			static constexpr int PAIR_POS_X = 0;
			static constexpr int PAIR_POS_Y = 1;
			
			static constexpr int SPEED_SLOW = 1000;
			static constexpr int SPEED_MEDIUM = 2000;
			static constexpr int SPEED_FAST = 3000;
			
			static constexpr int STATE_LENGTH_SECONDS = 15;
			static constexpr int STATE_LENGTH = 60 * STATE_LENGTH_SECONDS; // N seconds * FPS.
			
			
			Type type_;
			TimerRepeat state_timer_;
			std::pair<int, int> current_tile_;
			std::pair<int, int> target_;
			int speed_;
			Direction::Simple direction_prev_;
			std::array<bool, Direction::SIMPLE_SIZE> directions_possible_;
			bool can_turn_;
			
			
			std::array<bool, Direction::SIMPLE_SIZE> defaultPossibleDirections() const;
			
			const std::pair<int, int> getChaserPosition() const;
			const std::pair<int, int> getNextChaserPosition() const;
			const std::pair<int, int> getNextChaserPosition( Direction::Simple direction ) const;
			const std::pair<int, int> getPlayerPosition( const Object& player ) const;
			const std::pair<int, int> getPosition( const Object& obj ) const;
			const std::pair<int, int> randomTile() const;
			
			bool blockInPosition( const std::pair<int, int>& coordinates, const BlockSystem& blocks ) const;
			int distanceOfCoordinates( const std::pair<int, int>& one, const std::pair<int, int>& two ) const;
			
			const bool evenTile();
			void testDirection( const BlockSystem& blocks );
    };


#endif // MAZE_CHASER_SPRITE_H
