#pragma once

#include "game_state.hpp"
#include "ow_camera.hpp"
#include "ow_hero.hpp"
#include "ow_inventory.hpp"
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

		OWInventory inventory_;
		OWHero hero_;
		OWCamera camera_;
		CameraState camera_state_;
		char scratch_[ 3000 ];
};