#pragma once

#include "game_state.hpp"
#include <string>
#include "texture_box.hpp"

class TimeStartState final : public GameState
{
	public:
		TimeStartState( const Palette& palette, std::u32string text, int inventory_time_x );
		~TimeStartState();
		TimeStartState( const TimeStartState& ) = delete;
		TimeStartState( TimeStartState&& ) = delete;
		TimeStartState& operator=( const TimeStartState& ) = delete;
		TimeStartState& operator=( TimeStartState&& ) = delete;

		void stateUpdate() override;
		void stateRender() override;
		void init() override;

	private:
		static constexpr int NUMBER_OF_COLORS = 3;
		enum class State
		{
			GOING_DOWN,
			BUMPING_UPWARD,
			BUMPING_BACK_DOWN,
			BLINKING,
			SHRINKING,
			DONE
		};

		int y_;
		int x_;
		int size_;
		int timer_;
		int color_;
		int inventory_time_x_;
		int movement_per_frame_x_;
		State state_;
		SDL_Texture* textures_[ NUMBER_OF_COLORS ];
		sdl2::SDLRect src_;
		sdl2::SDLRect dest_;
		std::u32string text_;
};