#pragma once

#include "game_state.hpp"
#include "options_bg.hpp"
#include "option_selection_vertical.hpp"
#include "option_system.hpp"
#include "texture_box.hpp"

class ScreenOptionState final : public GameState
{
	public:
		ScreenOptionState();
		~ScreenOptionState();

		void stateUpdate() override;
		void stateRender() override;
		void init() override;


	private:
		int max_options_;
		std::vector<OptionBox> other_options_;
		OptionSelectionVertical selection_;
		TextureBox title_;
		OptionBox window_option_;
		OptionBox fullscreen_option_;
		OptionsBG bg_;

		void updateOptions();
		void updateInput();
		void setNullifiedFullOrWindowOption();
		void setNullifiedSizeOptions();
};
