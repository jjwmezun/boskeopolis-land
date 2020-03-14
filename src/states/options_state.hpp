#pragma once

#include "game_state.hpp"
#include "options_bg.hpp"
#include "option_system.hpp"
#include "texture_box.hpp"

class OptionsState final : public GameState
{
	public:
		OptionsState( bool from_title = false );
		~OptionsState();

		void stateUpdate() override;
		void stateRender() override;
		void init() override;
		void backFromPop() override;

	private:
		enum class Option
		{
			RESOLUTION,
			CONTROLS,
			LANGUAGE
		};

		void updateInput();
		bool testLanguageHasChanged() const;
		void resetOptionsText();
		void generateTitle();

		bool from_title_;
		int language_id_;
		TextureBox title_;
		OptionSystem options_;
		OptionsBG bg_;
};
