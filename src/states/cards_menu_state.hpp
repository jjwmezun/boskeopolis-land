#pragma once

#include "checkered_background.hpp"
#include "counter_flip.hpp"
#include "frame.hpp"
#include "game_state.hpp"
#include "texture_box.hpp"
#include "wtext_highlight.hpp"
#include "wtext_obj.hpp"

struct TradingCardGFX
{
	int id;
	WTextObj text;
};

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
		static constexpr int CARDS_PER_ROW = 2;
		static constexpr int CARDS_PER_PAGE = CARDS_PER_COLUMN * CARDS_PER_ROW;
		int selection_x_;
		int selection_y_;
		int input_delay_;
		int selection_;
		int cols_on_last_page_;
		int rows_on_last_page_;
		int current_page_;
		int next_page_;
		int arrow_timer_;
		std::vector<int> card_level_ids_;
        std::vector<WTextObj> card_text_;
		CheckeredBackground bg_;
        Frame message_frame_;
		WTextHighlight<> title_;
		std::vector<std::vector<TradingCardGFX>> card_pages_;
		TextureBox pages_gfx_;
		char scratch_[3000];
};