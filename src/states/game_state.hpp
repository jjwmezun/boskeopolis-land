#pragma once

#include "camera.hpp"
#include "palette.hpp"

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
			const Palette& palette = { 0, 1 }
		);

		virtual ~GameState();

		void render();

		virtual void update() = 0;
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