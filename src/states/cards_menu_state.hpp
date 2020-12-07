#pragma once

#include "checkered_background.hpp"
#include "counter_flip.hpp"
#include "frame.hpp"
#include "game_state.hpp"
#include "level.hpp"
#include "wtext_highlight.hpp"
#include "wtext_obj.hpp"

class CardsMenuState final : public GameState
{
	public:
		CardsMenuState();
		~CardsMenuState();
		CardsMenuState( const CardsMenuState& c ) = delete;
		const CardsMenuState& operator=( const CardsMenuState& c ) = delete;
		void stateUpdate() override;
		void stateRender() override;
		void init() override;

	private:
        WTextObj card_text_[ Level::NUMBER_OF_CYCLES ][ Level::NUMBER_OF_THEMES ];
		WTextHighlight<> title_;
		CheckeredBackground bg_;
        Frame message_frame_;
		CounterFlip<Level::NUMBER_OF_THEMES - 1> selection_x_;
		CounterFlip<Level::NUMBER_OF_CYCLES - 1> selection_y_;
		int input_delay_;
        char scratch_[3000];

		void doGeneralSelection();
};