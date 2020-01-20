#pragma once

#include "game_state.hpp"
#include "ow_camera.hpp"
#include "ow_hero.hpp"
#include "ow_inventory.hpp"
#include "ow_object.hpp"
#include "ow_tilemap.hpp"
#include "rapidjson/document.h"
#include "texture_box.hpp"
#include <unordered_map>

class Collision;

class OverworldState : public GameState
{
	public:
		OverworldState();
		~OverworldState();

		void stateUpdate() override;
		void stateRender() override;
		void init() override;
		void backFromPop() override;

	private:
		enum class CameraState
		{
			MOVE_PLAYER,
			MOVE_CAMERA,
			CAMERA_MOVES_AUTOMATICALLY_TO_PLAYER
		};

		void testForMenuAction();
		void renderCameraArrows();
		void updateBackgroundAnimation();
		void updateBackgroundPosition();
		void loadMap( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data );

		CameraState camera_state_;
		int background_animation_timer_;
		int background_animation_frame_;
		OWTileMap tilemap_;
		TextureBox bg_texture_;
		TextureBox fg_texture_;
		ImageGraphics water_background_;
		OWCamera camera_;
		OWHero hero_;
		OWInventory inventory_;
		std::unordered_map<int, OWObject> objects_;
		int current_level_;
		char scratch_[ 3000 ];
};