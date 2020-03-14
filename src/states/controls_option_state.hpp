#pragma once

#include "game_state.hpp"
#include "input.hpp"
#include "options_bg.hpp"
#include "option_selection_vertical.hpp"
#include "texture_box.hpp"

class ControlsOptionState final : public GameState
{
	public:
		ControlsOptionState();
		~ControlsOptionState();

		void stateUpdate() override;
		void stateRender() override;
		void init() override;
		void backFromPop() override;

	private:
		OptionSelectionVertical selection_;
		TextureBox title_;
		TextureBox text_;
		TextureBox highlights_[ Input::NUM_O_ACTIONS ];
		OptionsBG bg_;

		void updateSelection();
		void updateInput();
		void renderOptions() const;
};
