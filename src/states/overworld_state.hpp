#pragma once

#include "animated_graphics.hpp"
#include "game_state.hpp"
#include "ow_camera.hpp"
#include "ow_hero.hpp"
#include "ow_inventory.hpp"
#include "ow_level.hpp"
#include "ow_pal_change.hpp"
#include "ow_tile.hpp"
#include <vector>

class Collision;

class OverworldState : public GameState
{
	public:
		OverworldState();
		~OverworldState();

		void update() override;
		void stateRender() override;
		void init() override;
		
	private:
		static constexpr int SMOOTH_MOVEMENT_PADDING = 4;

		OWCamera camera_;
		OWHero hero_;
		int map_width_;
		int map_height_;
		OWInventory inventory_screen_;
		std::vector<OWTile> tiles_;
		std::vector<OWLevel> level_tiles_;
		std::vector<OWPalChange> pal_change_tiles_;
		int level_selection_;
		AnimatedGraphics water_gfx_;
		AnimatedGraphics lv_gfx_;
		bool go_to_list_;
		bool camera_mode_;
		bool camera_trans_;

		void mapData();
		void interactions();
		Collision testCollision( const sdl2::SDLRect& lhs, const sdl2::SDLRect& rhs );
		void mapEvents();
		void eventByID( int id );
		void menu();
		static Palette lvPal( int id );
		void renderCameraArrows();
};