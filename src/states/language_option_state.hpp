#pragma once

#include "game_state.hpp"
#include "options_bg.hpp"
#include "option_system.hpp"
#include "texture_box.hpp"

class LanguageOptionState : public GameState
{
	public:
		LanguageOptionState();
		~LanguageOptionState();

		void stateUpdate() override;
		void stateRender() override;
		void init() override;

	private:
		OptionsBG bg_;
		TextureBox title_;
		OptionSystem options_;
		void updateInput();
};
