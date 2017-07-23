#ifndef GAME_STATE_H
#define GAME_STATE_H

class Input;

#include "camera.h"
#include "palette.h"

class GameState
{
	public:
		enum StateID
		{
			__NULL,
			TITLE_STATE,
			LEVEL_SELECT_STATE,
			OVERWORLD_STATE,
			OVERWORLD_MENU_STATE,
			LEVEL_STATE,
			MESSAGE_STATE,
			PAUSE_STATE
		};

		GameState
		(
			StateID id = __NULL,
			const Palette& palette = { "Grayscale", 1 }
		);

		virtual ~GameState();

		void render();

		virtual void update( const Input& input ) = 0;
		virtual void stateRender() = 0;
		virtual void init() = 0;
		void changePalette();
		void newPalette( const Palette& pallete );
		StateID id() const;
		const Palette& palette() const;

	protected:
		const StateID id_;

	private:
		Palette palette_;
};

#endif // GAME_STATE_H
