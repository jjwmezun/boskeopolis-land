#pragma once

#include "game_state.hpp"
#include "options_bg.hpp"
#include "option_system.hpp"
#include "texture_box.hpp"

class LanguageOptionState final : public GameState
{
	public:
		LanguageOptionState();
		~LanguageOptionState();

		void stateUpdate() override;
		void stateRender() override;
		void init() override;

	private:
		OptionSystem options_;
		TextureBox title_;
		OptionsBG bg_;

		void updateInput();
		void setButtonsPressState();
		void initTitle();
		void resetTitle();
		void saveLanguageSettings() const;
};
