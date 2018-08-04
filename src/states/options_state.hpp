#pragma once

#include "game_state.hpp"
#include "options_bg.hpp"
#include "option_system.hpp"
#include "text_obj.hpp"

class OptionsState : public GameState
{
	public:
		OptionsState();
		~OptionsState();

		void stateUpdate() override;
		void stateRender() override;
		void init() override;

	private:
		enum class Option
		{
			RESOLUTION,
			CONTROLS
		};

		OptionsBG bg_;
		TextObj title_;
		OptionSystem options_;
		void updateInput();
};
