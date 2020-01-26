#pragma once

#include "game_state.hpp"
#include "options_bg.hpp"
#include "option_system.hpp"
#include "texture_box.hpp"

class OptionsState : public GameState
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

		OptionsBG bg_;
		TextureBox title_;
		OptionSystem options_;
		bool from_title_;
		void updateInput();
};
