#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H

#include "event_system.h"
#include "game_state.h"
#include "text.h"

class PauseState : public GameState
{

	public:
		PauseState( const Palette& palette, EventSystem& events, bool beaten );
		~PauseState();

		void update( Game& game, const Input& input );
		void stateRender();
		void init( Game& game );


	private:
		enum class PauseOption
		{
			PO_CONTINUE,
			PO_QUIT
		};
		static constexpr int NUM_O_PAUSE_OPTIONS = ( int )PauseOption::PO_QUIT + 1;

		static constexpr int PAUSE_BOX_WIDTH = 24;
		static constexpr int PAUSE_BOX_HEIGHT = 7;

		static constexpr sdl2::SDLRect surface_box_ =
		{
			Unit::MiniBlocksToPixels( floor( Unit::WINDOW_WIDTH_MINIBLOCKS / 2 ) - floor( PAUSE_BOX_WIDTH / 2 ) ),
			Unit::MiniBlocksToPixels( floor( Unit::WINDOW_HEIGHT_MINIBLOCKS / 2 ) - floor( PAUSE_BOX_HEIGHT / 2 ) ),
			Unit::MiniBlocksToPixels( PAUSE_BOX_WIDTH ),
			Unit::MiniBlocksToPixels( PAUSE_BOX_HEIGHT )
		};

		const std::vector<Text> option_text_;
		PauseOption option_selection_;
		EventSystem& events_;

		std::vector<Text> optionText( bool beaten ) const;
};

#endif // PAUSE_STATE_H