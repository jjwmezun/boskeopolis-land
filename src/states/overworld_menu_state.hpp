#pragma once

#include <array>
#include "game_state.hpp"
#include "mezun_sdl2.hpp"
#include "text_obj.hpp"

class OverworldMenuState : public GameState
{
	public:
		OverworldMenuState( bool& go_to_list, bool& camera_mode, const Palette& pal );
		~OverworldMenuState();
		void stateUpdate() override;
		void stateRender() override;

	private:
		enum class Option
		{
			CONTINUE,
			LIST,
			CAMERA,
			QUIT
		};
		static constexpr int NUM_O_OPTIONS = ( int )( Option::QUIT ) + 1;

		const sdl2::SDLRect bg_;
		std::array<TextObj, NUM_O_OPTIONS> option_text_;
		bool& go_to_list_;
		bool& camera_mode_;
		int option_selection_;
};