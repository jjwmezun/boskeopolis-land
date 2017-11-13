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
		void update() override;
		void stateRender() override;
		void init() override;

	private:
		std::vector<sdl2::SDLRect> win_icon_dests_;
		std::vector<sdl2::SDLRect> diamond_icon_dests_;
		std::vector<sdl2::SDLRect> gem_challenge_icon_dests_;
		std::vector<sdl2::SDLRect> time_challenge_icon_dests_;
		std::vector<Text> level_names_;
		std::vector<Text> gem_scores_;
		std::vector<Text> time_scores_;
		std::vector<Text> gem_challenges_text_;
		std::vector<Text> time_challenges_text_;
		std::vector<int>  level_ids_;
		std::vector<bool> gem_challenges_;
		std::vector<bool> time_challenges_;
		std::vector<bool> levels_complete_;

		SpriteGraphics win_icon_gfx_;
		SpriteGraphics diamond_gfx_;
		SpriteGraphics gem_icon_gfx_;

		Text title_;
		Camera camera_;
		Counter selection_;

		const sdl2::SDLRect INVENTORY_BG_DEST;
		const sdl2::SDLRect HEADER_BG_DEST;
		const sdl2::SDLRect GEM_ICON_DEST;
		sdl2::SDLRect highlight_dest_;

		const int prev_level_;
		int delay_length_;
		int delay_;
		bool show_challenges_;
};