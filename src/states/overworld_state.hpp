#pragma once

#include "game_state.hpp"
#include "ow_camera.hpp"
#include "ow_hero.hpp"
#include "ow_inventory.hpp"
#include "ow_tilemap.hpp"
#include "rapidjson/document.h"
#include "texture_box.hpp"
#include <vector>

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

		OWInventory inventory_;
		OWHero hero_;
		OWCamera camera_;
		CameraState camera_state_;
		int background_animation_timer_;
		int background_animation_frame_;
		ImageGraphics water_background_;
		int width_;
		int height_;
		TextureBox bg_texture_;
		TextureBox fg_texture_;
		OWTileMap tilemap_;

		char scratch_[ 3000 ];
};