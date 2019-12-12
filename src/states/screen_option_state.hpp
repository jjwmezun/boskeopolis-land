#pragma once

#include "game_state.hpp"
#include "options_bg.hpp"
#include "option_selection_vertical.hpp"
#include "option_system.hpp"
#include "texture_box.hpp"

class ScreenOptionState : public GameState
{
	public:
		ScreenOptionState();
		~ScreenOptionState();

		void stateUpdate() override;
		void stateRender() override;
		void init() override;


	private:
		OptionsBG bg_;
		TextureBox title_;
		OptionBox fullscreen_option_;
		OptionBox window_option_;
		OptionSelectionVertical selection_;
		std::vector<OptionBox> other_options_;
		int max_options_;

		void updateOptions();
		void updateInput();
		void setNullifiedFullOrWindowOption();
		void setNullifiedSizeOptions();
};
