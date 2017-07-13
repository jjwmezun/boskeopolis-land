#pragma once

#include "animated_graphics.h"
#include "event_system.h"
#include "game_state.h"
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
		OverworldState( const EventSystem& events, const Inventory& inventory, int level );
		~OverworldState();

		void update( Game& game, const Input& input, Graphics& graphics ) override;
		void stateRender( Graphics& graphics ) override;
		void init( Game& game, Graphics& graphics ) override;
		
	private:
		static constexpr int SMOOTH_MOVEMENT_PADDING = 4;

		OWHero hero_;
		OWCamera camera_;
		std::vector<OWTile> tiles_;
		std::vector<OWLevel> level_tiles_;
		std::vector<OWPalChange> pal_change_tiles_;
		OWInventory inventory_;
		EventSystem events_;
		int map_width_;
		int map_height_;
		int level_selection_;
		AnimatedGraphics water_gfx_;

		void mapData();
		void interactions( Graphics& graphics );
		void testInteractions( Graphics& graphics );
		Collision testCollision( const sdl2::SDLRect& lhs, const sdl2::SDLRect& rhs );
		void mapEvents();
		void eventByID( int id );
};