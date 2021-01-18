#pragma once

#include "counter_flip.hpp"
#include "game_state.hpp"
#include "texture_box.hpp"

class LevelTileMenuState final : public GameState
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
        enum class OptionType
        {
            PLAY,
            HARD_MODE,
            CANCEL
        };
        static constexpr int MAX_OPTIONS = 4;

        std::u32string getOptionName( OptionType type );

        int level_;
        int selection_;
        std::vector<OptionType> options_;
        TextureBox bg_;
        TextureBox highlighted_text_[ MAX_OPTIONS ];
};
