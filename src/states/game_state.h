#ifndef GAME_STATE_H
#define GAME_STATE_H

class Game;
class Graphics;
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

		void render( Graphics& graphics );

		virtual void update( Game& game, const Input& input, Graphics& graphics ) = 0;
		virtual void stateRender( Graphics& graphics ) = 0;
		virtual void init( Game& game, Graphics& graphics ) = 0;
		void changePalette( Graphics& graphics );
		void newPalette( Graphics& graphics, const Palette& pallete );
		StateID id() const;
		const Palette& palette() const;

	protected:
		const StateID id_;

	private:
		Palette palette_;
};

#endif // GAME_STATE_H
