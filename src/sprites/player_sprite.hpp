#pragma once

class Block;
class EventSystem;
class InventoryLevel;
class Sprite;

#include "input_component_player.hpp"
#include "input_component_player_record.hpp"
#include "player_graphics.hpp"
#include "sprite.hpp"
#include "sprite_graphics.hpp"

class PlayerSprite : public Sprite
{
	public:
		static constexpr int DEFAULT_JUMP_START_SPEED = 1000;
		static constexpr int DEFAULT_JUMP_TOP_SPEED = 6000;

		PlayerSprite
		(
			int x,
			int y,
			int jump_start_speed = 1000,
			int jump_top_speed = 6000,
			std::unique_ptr<InputComponent> input
				= nullptr,
			std::unique_ptr<SpriteGraphics>&& gfx = nullptr,
			SpriteType type = SpriteType::HERO,
			int start_speed = 160,
			int top_speed = 2000,
			DuckData duck_data = { false, 9, 11, -6, 8 },
			UnDuckData unduck_data = { 10, 1, -2, 3 }
		);
		~PlayerSprite();

		virtual void customUpdate( LevelState& level_state ) override;
		virtual void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		virtual void tryUnduck( const BlockSystem& blocks );
		virtual bool canJump() const override;

		void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap ) override;
		void actions( const BlockSystem& blocks, EventSystem& events );
		void heroActions( LevelState& level_state );
		bool isLookingUp() const;

	protected:
		void playerInteract( Collision& my_collision, Sprite& them, Health& health, EventSystem& events, InventoryLevel& inventory );
		void handleCameraMovement( Camera& camera );
		void handleDoorBehavior( EventSystem& events );
		void handleChasmBehavior( const Map& lvmap, EventSystem& events );
		void handleDrowningBehavior( const Health& health );
		void handleWaterEnteringAndExiting( const Map& lvmap );
		void resetBopsOnLanding( InventoryLevel& inventory ) const;
		void handleLadderBehavior( EventSystem& events );
		void handleDucking( const BlockSystem& blocks );
		void adjustJumpSpeed();
		void dontDuckWhileSwimming( const BlockSystem& blocks );
		void handleRunning();
		void handleWalking();
		void handleDuckingAndSliding( const BlockSystem& blocks, EventSystem& events );
		void handleJumpingAndFalling( const BlockSystem& blocks, EventSystem& events );
		void handleLookingUp();
		void slideLeft( EventSystem& events );
		void slideRight( EventSystem& events );

		bool door_lock_;
		bool is_looking_up_;
		std::unique_ptr<InputComponent> input_;
		const DuckData duck_data_;
		const UnDuckData unduck_data_;
		PlayerGraphics player_gfx_;
};
