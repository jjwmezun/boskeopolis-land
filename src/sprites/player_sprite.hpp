#pragma once

class Block;
class Sprite;

#include "input_component_player.hpp"
#include "player_graphics.hpp"
#include "sprite.hpp"
#include "sprite_graphics.hpp"

class PlayerSprite : public Sprite
{
	public:
		PlayerSprite
		(
			int x,
			int y,
			int jump_start_speed = 1000,
			int jump_top_speed = 6000,
			std::unique_ptr<InputComponent> input
				= std::unique_ptr<InputComponentPlayer>
				(),
			std::unique_ptr<SpriteGraphics>&& gfx = std::make_unique<SpriteGraphics> ( "sprites/autumn.png", 0, 0, false, false, 0, false, -1, -2, 2, 4 ),
			SpriteType type = SpriteType::HERO,
			int start_speed = 160,
			int top_speed = 2000
		);
		~PlayerSprite();

		virtual void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		virtual void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		virtual void duck();
		virtual void unduck( const BlockSystem& blocks );
		virtual bool canJump() const override;

		void deathAction( const Camera& camera, EventSystem& events, const Map& lvmap ) override;
		void actions( const BlockSystem& blocks, EventSystem& events );
		void heroActions( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		std::unique_ptr<InputComponent> input_;

	protected:
		PlayerGraphics player_gfx_;

	private:
		bool door_lock_;

		void forceUnduck();
		void handleCameraMovement( Camera& camera );
		void handleDoorBehavior( EventSystem& events );
		void handleChasmBehavior( const Map& lvmap, EventSystem& events );
		void handleDrowningBehavior( const Health& health );
		void handleWaterEnteringAndExiting();
		void resetBopsOnLanding() const;
		void handleLadderBehavior( EventSystem& events );
		void handleDucking( const BlockSystem& blocks );
		void adjustJumpSpeed();
		void dontDuckWhileSwimming( const BlockSystem& blocks );
		void handleRunning();
		void handleWalking();
		void handleDuckingAndSliding( const BlockSystem& blocks, EventSystem& events );
		void handleJumpingAndFalling( const BlockSystem& blocks, const EventSystem& events );
		void handleLookingUp();
};
