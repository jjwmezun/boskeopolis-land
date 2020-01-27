#pragma once

#include "game_state.hpp"
#include "texture_box.hpp"

class TimeStartState : public GameState
{
	public:
		TimeStartState( const Palette& palette );
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
		State state_;
		SDL_Texture* textures_[ NUMBER_OF_COLORS ];
		sdl2::SDLRect src_;
		sdl2::SDLRect dest_;
};