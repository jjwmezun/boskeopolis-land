#pragma once

#include "event_system.hpp"
#include "game_state.hpp"
#include "text.hpp"
#include <vector>

class PauseState : public GameState
{
	public:
		PauseState( const Palette& palette, EventSystem& events );
		~PauseState();
		void stateUpdate() override;
		void stateRender() override;

	private:
		enum class PauseOption
		{
			PO_CONTINUE,
			PO_QUIT
		};
		static constexpr int NUM_O_PAUSE_OPTIONS = ( int )( PauseOption::PO_QUIT ) + 1;

		const sdl2::SDLRect surface_box_;
		Text option_text_ [ PauseState::NUM_O_PAUSE_OPTIONS ];
		EventSystem& events_;
		PauseOption option_selection_;

		std::string quitName( bool beaten ) const;
};