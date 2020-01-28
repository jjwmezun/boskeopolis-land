#pragma once

#include "counter_flip.hpp"
#include "event_system.hpp"
#include "game_state.hpp"
#include "inventory_level.hpp"

class PauseState : public GameState
{
	public:
		PauseState( const Palette& palette, EventSystem& events, InventoryLevel& inventory_box );
		~PauseState();
		PauseState( const PauseState& ) = delete;
		PauseState( PauseState&& ) = delete;
		PauseState& operator=( const PauseState& ) = delete;
		PauseState& operator=( PauseState&& ) = delete;
		void stateUpdate() override;
		void stateRender() override;
		void init() override;
		void backFromPop() override;

	private:
		enum class PauseOption
		{
			CONTINUE,
			OPTIONS,
			QUIT
		};
		static constexpr int NUMBER_OF_OPTIONS = ( int )( PauseOption::QUIT ) + 1;
		std::u32string getOptionName( PauseOption type );
		void generateTextures();

		int language_id_;
		CounterFlip<NUMBER_OF_OPTIONS - 1> selection_;
		EventSystem& events_;
		InventoryLevel& inventory_box_;
		TextureBox bg_;
		TextureBox highlighted_text_[ PauseState::NUMBER_OF_OPTIONS ];
};