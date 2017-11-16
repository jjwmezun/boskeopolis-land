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
			PAUSE_STATE,
			TIME_START_STATE
		};

		GameState
		(
			StateID id = __NULL,
			const Palette& palette = { "Grayscale", 1 }
		);
		virtual ~GameState();

		void update();
		void render();
		virtual void stateUpdate() = 0;
		virtual void stateRender() = 0;
		virtual void init();

		void changePalette();
		void newPalette( const Palette& pallete );
		void newPalette( const char* name );
		StateID id() const;
		const Palette& palette() const;
		bool nextFrame( int interval = 8, int duration = 1 ) const;
		int frame() const;

	protected:
		const StateID id_;

	private:
		Palette palette_;
		int frame_counter_;
};