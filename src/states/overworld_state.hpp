#pragma once

#include "game_state.hpp"
#include "ow_camera.hpp"
#include "ow_camera_arrows.hpp"
#include "ow_event.hpp"
#include "ow_hero.hpp"
#include "ow_inventory.hpp"
#include "ow_level_tile_graphics.hpp"
#include "ow_object.hpp"
#include "ow_state.hpp"
#include "ow_tilemap.hpp"
#include "rapidjson/document.h"
#include "show_event_type.hpp"
#include "texture_box.hpp"
#include <unordered_map>

class Collision;

class OverworldState : public GameState
{
	public:
		static constexpr int NUMBER_OF_LAYERS = 2;
		OverworldState( int previous_level, bool new_game = false, ShowEventType show_event = ShowEventType::NONE );
		~OverworldState();

		void stateUpdate() override;
		void stateRender() override;
		void init() override;
		void backFromPop() override;
		void addLevel( int i, int level_id, sdl2::SDLRect dest );

	private:
		void testForMenuAction();
		void updateBackgroundAnimation();
		void updateBackgroundPosition();
		void loadMap( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data );
		void generateSprites();
		void generateMapTextures();
		void generateBGMapTexture( int n );
		void generateFGMapTexture( int n );
		void generateMap();
		bool testLanguageHasChanged() const;
		void updateSolids( int tile, int i );

		OWState state_;
		int background_animation_timer_;
		int background_animation_frame_;
		int current_level_;
		int previous_level_;
		int language_id_;
		int current_palette_;
		OWObject* object_on_;
		std::unordered_map<int, OWObject> objects_;
		OWTileMap tilemap_;
		TextureBox bg_textures_[ NUMBER_OF_LAYERS ];
		TextureBox fg_textures_[ NUMBER_OF_LAYERS ];
		ImageGraphics water_background_;
		OWCamera camera_;
		OWEvent event_;
		OWCameraArrows camera_arrows_;
		OWHero hero_;
		OWLevelTileGraphics level_tile_graphics_;
		OWInventory inventory_;
};