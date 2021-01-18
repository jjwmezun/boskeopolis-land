#pragma once

#include "checkered_background.hpp"
#include "counter_flip.hpp"
#include "frame.hpp"
#include "game_state.hpp"
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
		void doGeneralSelection();

		static constexpr int CARDS_PER_COLUMN = 16;
		int selection_x_;
		int selection_y_;
		int input_delay_;
		int number_of_rows_;
		std::vector<int> card_level_ids_;
        std::vector<WTextObj> card_text_;
		CheckeredBackground bg_;
        Frame message_frame_;
		WTextHighlight<> title_;
		char scratch_[3000];
};