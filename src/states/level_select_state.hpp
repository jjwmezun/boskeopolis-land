#pragma once

#include "camera.hpp"
#include "game_state.hpp"
#include "level.hpp"
#include "map_layer_image.hpp"
#include "sprite_graphics.hpp"
#include "text_obj.hpp"

class LevelSelectState : public GameState
{
	public:
		LevelSelectState( int level );
		~LevelSelectState();
		void stateUpdate() override;
		void stateRender() override;
		void init() override;

	private:
		MapLayerImage bg_;
		SpriteGraphics win_icon_gfx_;
		SpriteGraphics diamond_gfx_;
		SpriteGraphics no_win_icon_gfx_;
		SpriteGraphics no_diamond_gfx_;
		SpriteGraphics no_win_icon_gfx_lighter_;
		SpriteGraphics no_diamond_gfx_lighter_;
		SpriteGraphics time_challenge_icon_gfx_;
		SpriteGraphics gem_challenge_icon_gfx_;
		SpriteGraphics time_challenge_icon_gfx_lighter_;
		SpriteGraphics gem_challenge_icon_gfx_lighter_;
		TextObj title_;
		TextObj pts_;
		TextObj percent_;
		Camera camera_;

		std::vector<TextObj> level_names_;
		std::vector<TextObj> gem_scores_;
		std::vector<TextObj> gem_challenges_text_;
		std::vector<TextObj> time_scores_;
		std::vector<TextObj> time_challenges_text_;
		std::vector<sdl2::SDLRect> win_icon_dests_;
		std::vector<sdl2::SDLRect> diamond_icon_dests_;
		std::vector<sdl2::SDLRect> gem_challenge_icon_dests_;
		std::vector<sdl2::SDLRect> time_challenge_icon_dests_;
		std::vector<TextObj*> gem_show_;
		std::vector<TextObj*> time_show_;
		std::vector<SpriteGraphics*> win_icon_show_;
		std::vector<SpriteGraphics*> diamond_icon_show_;
		std::vector<SpriteGraphics*> gem_icon_show_;
		std::vector<SpriteGraphics*> time_icon_show_;
		std::vector<int>  level_ids_;

		const sdl2::SDLRect INVENTORY_BG_DEST;
		const sdl2::SDLRect HEADER_BG_DEST;
		sdl2::SDLRect highlight_dest_;

		SpriteGraphics* all_wins_icon_;
		SpriteGraphics* all_diamonds_icon_;
		SpriteGraphics* all_gem_challenges_icon_;
		SpriteGraphics* all_time_challenges_icon_;

		const int prev_level_;
		int selection_;
		int delay_length_;
		int delay_;
		int first_entry_on_camera_;
		int end_entry_on_camera_;
		bool show_challenges_;
		bool show_challenges_prev_;
		bool changed_selection_;

		void updateEntries();
		void updateCameraEntryLimits();
		void updateInput();
		void updateHighlighter();
		void renderInventory() const;
		void renderHeader() const;
		int calculateFirstEntryOnCamera() const;
		int calculateEndEntryOnCamera() const;
};
