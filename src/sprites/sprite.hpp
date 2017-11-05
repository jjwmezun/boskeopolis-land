#pragma once

class Block;
class BlockSystem;
class Camera;
class Collision;
class EventSystem;
class GameState;
class Health;
class Level;
class Map;
class SpriteGraphics;
class SpriteSystem;

#include "direction.hpp"
#include "render.hpp"
#include <memory>
#include "object.hpp"
#include <SDL2/SDL.h>
#include "sprite_component.hpp"
#include "timer_simple.hpp"
#include "unit.hpp"

#include "sprite_movement.hpp"
#include "grounded_sprite_movement.hpp"
#include "fluttering_sprite_movement.hpp"
#include "swimming_sprite_movement.hpp"

class Sprite : public Object
{
	public:
		friend class SpriteMovement;
		friend class GroundedSpriteMovement;
		friend class FlutteringSpriteMovement;
		friend class SwimmingSpriteMovement;

		enum class SpriteType
		{
			NOTYPE,
			HERO,
			ENEMY,
			RIVAL,
			BOPPABLE,
			CLOUD_PLATFORM,
			LIGHTNING,
			HEROS_BULLET,
			DONT_RESPAWN
		};

		enum class CameraMovement
		{
			RESET_INSTANTLY_OFFSCREEN,
			RESET_OFFSCREEN_AND_AWAY,
			PERMANENT,
			PAUSE_OFFSCREEN,
			DESPAWN_OFFSCREEN
		};

		Sprite
		(
			std::unique_ptr<SpriteGraphics> graphics,
			int x = 0,
			int y = 0,
			int width = 16,
			int height = 16,
			std::vector<SpriteType> type = {},
			int start_speed = 160,
			int top_speed = 2000,
			int jump_start_speed = 1000,
			int jump_top_speed = 6000,
			Direction::Horizontal direction_x = Direction::Horizontal::__NULL,
			Direction::Vertical direction_y = Direction::Vertical::__NULL,
			std::unique_ptr<SpriteComponent> component = nullptr,
			SpriteMovement::Type physics_state = SpriteMovement::Type::GROUNDED,
			CameraMovement camera_movement = CameraMovement::RESET_OFFSCREEN_AND_AWAY,
			bool despawn_when_dead = true,
			bool block_interact = true,
			bool sprite_interact = true,
			bool impervious = false,
			double bounce_ = .2,
			bool rotate_on_slopes = false,
			bool ignore_on_camera = false,
			int map_id = -1
		);

		virtual ~Sprite();
		Sprite( const Sprite& ) = delete;
		Sprite& operator=( const Sprite& ) = delete;

		static constexpr double TRACTION_NORMAL = 1.2;
		static constexpr double TRACTION_ICY = 1.025;
		static double traction_;

		static constexpr int RESISTANCE_X_NORMAL = 0;
		static constexpr int RESISTANCE_X_WINDY = 100;
		static int resistance_x_;

		static constexpr int GRAVITY_START_SPEED_NORMAL = 500;
		static constexpr int GRAVITY_TOP_SPEED_NORMAL = 4000;
		static constexpr int GRAVITY_START_SPEED_MOON = 200;
		static constexpr int GRAVITY_TOP_SPEED_MOON = 1000;
		static int gravity_start_speed_;
		static int gravity_top_speed_;
		
		static void moonGravityOn();
		static void moonGravityOff();
		
		const int map_id_;

		int vx_ = 0;
		int vy_ = 0;

		Direction::Simple direction_;
		Direction::Horizontal direction_x_;
		Direction::Vertical direction_y_;

		bool jump_lock_ = true; // Not needed
		bool touching_ladder_ = false; // Not needed
		bool on_ground_ = false;
		bool is_sliding_ = false; // Not needed
		bool in_water_ = false;
		Direction::Horizontal on_slope_ = Direction::Horizontal::__NULL;

		void update( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		virtual void render( Camera& camera, bool priority = false );
		void drawHitBox( const Camera& camera );
		void interact( Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health );

		virtual void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) = 0;
		virtual void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health ) = 0;
		virtual void reset();

		bool onGround() const;
		bool onGroundPrev() const;
		bool onGroundPadding() const;
		bool canJump() const;
		bool isJumping() const;
		bool isJumpingPrev() const;
		bool isDucking() const; // Not needed
		bool onLadder() const; // Not needed
		bool onLadderPrev() const; // Not needed
		bool touchingLadder() const; // Not needed
		bool touchingLadderPrev() const; // Not needed
		bool lookingUp() const; // Not needed

		bool isMoving() const;
		bool isRunning() const; // Not needed
		bool isSlidingPrev() const; // Not needed
		SpriteMovement::Type movementType() const;
		bool hasMovementType( SpriteMovement::Type type ) const;

		bool isDead() const;
		bool fellInBottomlessPit( Map& lvmap ) const;

		bool collidedAny() const;
		bool collidedLeft() const;
		bool collidedRight() const;
		bool collideTopOnly( const Collision& collision, Object& them ) const;

		bool interactsWithBlocks() const;
		bool interactsWithSprites() const;
		bool hasType( SpriteType type ) const;
		bool hasCameraMovement( CameraMovement type ) const;
		CameraMovement cameraMovement() const;
		bool despawnWhenDead() const;

		void moveLeft();
		void moveRight();
		void moveUp();
		void moveDown();
		void stopX();
		void stopY();
		void fullStopX();
		void fullStopY();
		void jump();
		void bounce( int amount = 7 );
		void bounceLeft( int overlap );
		void bounceRight( int overlap );
		void bounceDownward( int overlap );

		const Collision testCollision( const Object& them ) const;
		void collideStopAny( const Collision& collision );
		void collideStopXLeft( int overlap );
		void collideStopXRight( int overlap );
		void collideStopYBottom( int overlap );
		void collideStopYTop( int overlap );

		bool collideBottomOnly( const Collision& collision, const Object& other ) const;

		void slowFall();
		void fastFall();
		void stopDucking(); // Not needed
		void touchLadder(); // Not needed
		void grabLadder(); // Not needed
		void releaseLadder(); // Not needed
		void slideLeft(); // Not needed
		void slideRight(); // Not needed
		void lookUp(); // Not needed
		void stopLookingUp(); // Not needed

		void run(); // Not needed
		void stopRunning(); // Not needed

		void kill();
		void killNoAnimation();

		void boundaries( Camera& camera, Map& lvmap );
		void setPosition( int x, int y );
		void changeMovement( SpriteMovement::Type type );
		const SpriteMovement* getMovement( SpriteMovement::Type type );

		Direction::Horizontal directionX() const;
		Direction::Vertical directionY() const;

		bool deathFinished() const;

		int xPrevSubPixels() const;
		int yPrevSubPixels() const;
		int xPrevPixels()    const;
		int yPrevPixels()    const;
		int prevLeftSubPixels()   const;
		int prevRightSubPixels()  const;
		int prevTopSubPixels()    const;
		int prevBottomSubPixels() const;
		int prevRightPixels()  const;
		int prevBottomPixels() const;
		const sdl2::SDLRect& originalHitBox() const;
		int originalXSubPixels() const;
		int originalYSubPixels() const;

		static const int JUMP_DUCK_TOP_SPEED = 5500;
		static const int LADDER_SPEED = 1000;

		int x_prev_ = -123456789;
		int y_prev_ = -123456789;
		const sdl2::SDLRect original_hit_box_;

		const std::vector<SpriteType> types_;
		const CameraMovement camera_movement_;
		const bool despawn_when_dead_;
		std::unique_ptr<SpriteGraphics> graphics_;
		std::unique_ptr<SpriteComponent> component_;
		const SpriteMovement* movement_;

		const Direction::Horizontal direction_x_orig_;
		const Direction::Vertical direction_y_orig_;

		const bool impervious_;
		const int start_speed_walk_;
		const int top_speed_walk_;
		const int start_speed_run_; // Not needed
		const int top_speed_run_; // Not needed
		int start_speed_;
		int top_speed_;
		int top_speed_upward_;
		int top_speed_downward_;
		const double bounce_;

		int acceleration_x_ = 0;
		int acceleration_y_ = 0;
		int fall_start_speed_ = gravity_start_speed_; // Not needed
		int fall_top_speed_ = gravity_top_speed_; // Not needed
		int jump_start_speed_;
		int jump_top_speed_normal_;
		int jump_top_speed_;
		int bounce_height_ = 0;
		bool on_ground_prev_ = false;
		bool can_jump_ = false;
		bool is_jumping_ = false;
		bool is_jumping_prev_ = false;
		bool jump_start_ = false;
		bool jump_end_ = false;
		bool is_bouncing_ = false;
		bool is_bouncing_prev_ = false;
		bool slide_jump_ = false; // Not needed
		bool is_ducking_ = false; // Not needed
		bool touching_ladder_prev_ = false; // Not needed
		bool on_ladder_ = false; // Not needed
		bool on_ladder_prev_ = false; // Not needed
		bool looking_up_ = false; // Not needed
		bool is_sliding_prev_ = false; // Not needed

		bool collide_top_ = false;
		bool collide_bottom_ = false;
		bool collide_top_prev_ = false;
		bool collide_bottom_prev_ = false;
		bool collide_left_ = false;
		bool collide_right_ = false;
		bool collide_left_prev_ = false;
		bool collide_right_prev_ = false;

		bool block_interact_ = true;
		bool sprite_interact_ = true;
		bool is_running_ = false;
		bool is_moving_ = false;

		void position();
		void positionX();
		void positionY();

		bool is_dead_ = false;
		bool death_finished_ = false;
		bool dead_no_animation_ = false;
		TimerSimple death_timer_ = TimerSimple( 32, false );
		virtual void deathAction( Camera& camera );
		void defaultDeathAction( Camera& camera );
		void resetPosition();
		void invincibilityFlicker( const Health& health );

		TimerSimple on_ground_padding_;

		static const SpriteMovement floating_;
		static const GroundedSpriteMovement grounded_;
		static const FlutteringSpriteMovement fluttering_;
		static const SwimmingSpriteMovement swimming_;

		sdl2::SDLRect justAbove() const;
		bool blocksJustAbove( const BlockSystem& blocks ) const;
		sdl2::SDLRect justLeft() const;
		bool blocksJustLeft( const BlockSystem& blocks ) const;
};