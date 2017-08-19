#pragma once

#include "game_state.hpp"
#include "level.hpp"
#include "sprite_graphics.hpp"
#include "text.hpp"

class LevelSelectState : public GameState
{

	public:
		LevelSelectState( int level );
		~LevelSelectState();

		void update();
		void stateRender();
		void init();


	private:
		static constexpr int DEFAULT_LEVEL = 0;
		static constexpr int FUNDS_X = 32;
		static constexpr int BG_COLOR = 1;
		static constexpr int INVENTORY_Y = Unit::WINDOW_HEIGHT_PIXELS - 24;
		sdl::rect INVENTORY_BG_DEST = { 0, INVENTORY_Y, Unit::WINDOW_WIDTH_PIXELS, 24 };
		static constexpr int START_Y = 40;
		sdl::rect HEADER_BG_DEST = { 0, 0, Unit::WINDOW_WIDTH_PIXELS, START_Y };
		sdl::rect GEM_ICON_DEST = { FUNDS_X - 16, INVENTORY_Y + 8, 8, 8 };

		Text title_ = Text( "Select a Level, yo.", 0, 16, Text::FontShade::BLACK, Text::FontAlign::CENTER );
		SpriteGraphics win_icon_gfx_ = SpriteGraphics( "tilesets/universal.png", 48, 8 );
		SpriteGraphics diamond_gfx_ = SpriteGraphics( "tilesets/universal.png", 48, 0 );
		SpriteGraphics gem_icon_gfx_ = SpriteGraphics( "charset.png", 32, 16 );

		int delay_length_ = 16;
		int delay_ = 0;
		bool show_challenges_ = false;

		sdl::rect highlight_dest_ = { 16, START_Y, Unit::WINDOW_WIDTH_PIXELS - 32, 8 };

		std::vector<Text> level_names_;
		std::vector<int>  level_ids_;
		std::vector<Text> gem_scores_;
		std::vector<Text> time_scores_;
		std::vector<Text> gem_challenges_text_;
		std::vector<Text> time_challenges_text_;
		std::vector<bool> gem_challenges_;
		std::vector<bool> time_challenges_;
		std::vector<bool> levels_complete_;

		std::vector<sdl::rect> win_icon_dests_;
		std::vector<sdl::rect> diamond_icon_dests_;
		std::vector<sdl::rect> gem_challenge_icon_dests_;
		std::vector<sdl::rect> time_challenge_icon_dests_;

		int prev_level_;
		Counter selection_;
		Camera camera_;
};