#pragma once

#include "counter_flip.hpp"
#include "frame.hpp"
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
#include "scrolling_background.hpp"
#include "show_event_type.hpp"
#include "texture_box.hpp"
#include "timer_repeat_t2.hpp"
#include <unordered_map>
#include <vector>

class Collision;

class OverworldState final : public GameState
{
	public:
		static constexpr int NUMBER_OF_LAYERS = 2;
		OverworldState( OWTile previous_level, ShowEventType show_event = ShowEventType::NONE, int map = 0 );
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
		void setLevelSprite( int level_id, int i, const sdl2::SDLRect& dest );
		void dowarp();

		static constexpr int MAX_ANIMATION_FRAMES = 2;

		struct OverworldWarp
		{
			int destx;
			int desty;
			int destmap;
			int palette;
		};

		OWState state_;
		int width_blocks_;
		int height_blocks_;
		int background_animation_timer_;
		int background_animation_frame_;
		int language_id_;
		int current_palette_;
		int current_map_;
		CounterFlip<MAX_ANIMATION_FRAMES - 1> current_animation_frame_;
		TimerRepeatT2<32> animation_timer_;
		OWObject* object_on_;
		OWTile current_space_;
		OWTile previous_space_;
		std::vector<int> sprites_tiles_;
		std::vector<OverworldWarp> warps_;
		std::unordered_map<int, OWObject> objects_;
		OWTileMap tilemap_;
		std::vector<int> bg_tiles_[ NUMBER_OF_LAYERS ];
		std::vector<int> fg_tiles_[ NUMBER_OF_LAYERS ];
		TextureBox screen_;
		TextureBox bg_textures_[ NUMBER_OF_LAYERS ][ MAX_ANIMATION_FRAMES ];
		TextureBox fg_textures_[ NUMBER_OF_LAYERS ][ MAX_ANIMATION_FRAMES ];
		Frame main_frame_;
		ImageGraphics water_backgrounds_[ 2 ];
		ScrollingBackground background_;
		OWCamera camera_;
		OWEvent event_;
		OWCameraArrows camera_arrows_;
		OWHero hero_;
		OWLevelTileGraphics level_tile_graphics_;
		OWInventory inventory_;
};