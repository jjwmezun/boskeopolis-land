#pragma once

#include "counter_flip.hpp"
#include "game_state.hpp"
#include "texture_box.hpp"

class LevelTileMenuState : public GameState
{
	public:
		LevelTileMenuState( const Palette& palette, int level );
		~LevelTileMenuState();
        LevelTileMenuState( const LevelTileMenuState& ) = delete;
        LevelTileMenuState& operator=( const LevelTileMenuState& ) = delete;
        LevelTileMenuState( LevelTileMenuState&& ) = delete;
        LevelTileMenuState& operator=( LevelTileMenuState&& ) = delete;
		void stateUpdate() override;
		void stateRender() override;
		void init() override;

	private:
        enum class Option
        {
            PLAY,
            HARD_MODE,
            CANCEL
        };
        static constexpr int NUMBER_OF_OPTIONS = ( int )( Option::CANCEL ) + 1;

        std::u32string getOptionName( Option type );

        int level_;
        CounterFlip<NUMBER_OF_OPTIONS - 1> selection_;
        TextureBox bg_;
        TextureBox highlighted_text_[ NUMBER_OF_OPTIONS ];
};
