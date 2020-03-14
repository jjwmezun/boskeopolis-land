#pragma once

#include "sprite.hpp"
#include "timer_random.hpp"

class CowpokerSprite final : public Sprite
{
	public:
		static std::unique_ptr<Sprite> TallCowpokerSprite( int x, int y, int map_id );
		static std::unique_ptr<Sprite> ShortCowpokerSprite( int x, int y, int map_id );
		~CowpokerSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;

	private:
		enum class CowpokerType
		{
			SHORT,
			TALL,
			STRONGER
		};

		CowpokerSprite( int x, int y, int w, int h, CowpokerType type, std::unique_ptr<SpriteGraphics>&& gfx, int map_id );
		void handleGraphics();
		void handleThrowing( SpriteSystem& sprites );

		CowpokerType type_;
		bool is_shooting_;
		int is_shooting_counter_;
		TimerRandom<80, 10> throw_timer_;
};
