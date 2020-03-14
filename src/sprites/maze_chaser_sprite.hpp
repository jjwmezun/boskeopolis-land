#pragma once

#include "sprite.hpp"
#include "timer_repeat_t.hpp"

class MazeChaserSprite final : public Sprite
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
		void customUpdate( LevelState& level_state );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state );

	private:
		// For std::pairs.
		static constexpr int PAIR_POS_X = 0;
		static constexpr int PAIR_POS_Y = 1;
		static constexpr int SPEED_SLOW = 1000;
		static constexpr int SPEED_MEDIUM = 2000;
		static constexpr int SPEED_FAST = 3000;
		static constexpr int STATE_LENGTH_SECONDS = 15;
		static constexpr int STATE_LENGTH = 60 * STATE_LENGTH_SECONDS; // N seconds * FPS.

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
		void updateGraphics();

		bool can_turn_;
		Type type_;
		int speed_;
		Direction::Simple direction_;
		Direction::Simple direction_prev_;
		std::pair<int, int> current_tile_;
		std::pair<int, int> target_;
		TimerRepeatT<STATE_LENGTH, true> state_timer_;
		std::array<bool, Direction::SIMPLE_SIZE> directions_possible_;
};
