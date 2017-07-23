#pragma once

class Block;
class BlockSystem;
class Camera;
class Collision;
class EventSystem;
class Game;
class GameState;
class Input;
class InventoryLevel;
class Level;
class Map;
class SpriteGraphics;
class SpriteSystem;

#include "direction.h"
#include "render.h"
#include "grounded_sprite_movement.h"
#include "memory.h"
#include "object.h"
#include <SDL2/SDL.h>
#include "sprite_component.h"
#include "sprite_movement.h"
#include "sprite_status.h"
#include "unit.h"

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
			LIGHTNING
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
			int max_hp = 3,
			int hp = 3,
			bool rotate_on_slopes = false
		);

		virtual ~Sprite();
		Sprite( const Sprite& ) = delete;
		Sprite& operator=( const Sprite& ) = delete;

		const SpriteStatus& status() const;

		static constexpr double TRACTION_NORMAL = 1.2;
		static constexpr double TRACTION_ICY = 1.025;
		static double traction_;

		static const int RESISTANCE_X_NORMAL = 0;
		static const int RESISTANCE_X_WINDY = 100;
		static int resistance_x_;

		int vx_ = 0;
		int vy_ = 0;

		Direction::Simple direction_;
		Direction::Horizontal direction_x_;
		Direction::Vertical direction_y_;

		bool jump_lock_ = true;
		bool touching_ladder_ = false;
		bool on_ground_ = false;
		bool is_sliding_ = false;
		bool in_water_ = false;
		bool submerged_in_water_ = false;
		Direction::Horizontal on_slope_ = Direction::Horizontal::__NULL;

		void update( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		virtual void render( Camera& camera, bool priority = false );
		void interact( Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );

		virtual void customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks ) = 0;
		virtual void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap ) = 0;
		virtual void reset();

		bool onGround() const;
		bool onGroundPrev() const;
		bool onGroundPadding() const;
		bool canJump() const;
		bool isJumping() const;
		bool isJumpingPrev() const;
		bool isDucking() const;
		bool onLadder() const;
		bool onLadderPrev() const;
		bool touchingLadder() const;
		bool touchingLadderPrev() const;
		bool lookingUp() const;

		bool isMoving() const;
		bool isRunning() const;
		bool isSlidingPrev() const;
		SpriteMovement::Type movementType() const;
		bool hasMovementType( SpriteMovement::Type type ) const;

		bool isDead() const;
		int hp() const;
		bool fellInBottomlessPit( Map& lvmap ) const;

		bool collidedLeft() const;
		bool collidedRight() const;
		bool collideTopOnly( Collision& collision, Object& them ) const;

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
		virtual void swim();

		const Collision testCollision( const Object& them ) const;
		void collideStopAny( Collision& collision );
		void collideStopXLeft( int overlap );
		void collideStopXRight( int overlap );
		void collideStopYBottom( int overlap );
		void collideStopYTop( int overlap );

		bool collideBottomOnly( const Collision& collision, const Object& other ) const;

		void slowFall();
		void fastFall();
		void stopDucking();
		void touchLadder();
		void grabLadder();
		void releaseLadder();
		void slideLeft();
		void slideRight();
		void lookUp();
		void stopLookingUp();

		void run();
		void stopRunning();

		void kill();
		void killNoAnimation();
		void hurt( int amount = 1 );
		void heal( int amount = 1 );
		void fullHeal();

		void boundaries( Camera& camera, Map& lvmap );
		void setPosition( int x, int y );
		void changeMovement( SpriteMovement::Type type );
		static std::unique_ptr<SpriteMovement> getMovement( SpriteMovement::Type type );

		Direction::Horizontal directionX() const;
		Direction::Vertical directionY() const;

		bool deathFinished() const;


	protected:
		static const int GRAVITY_START_SPEED = 500;
		static const int GRAVITY_TOP_SPEED = 4000;
		static const int JUMP_DUCK_TOP_SPEED = 5500;
		static const int LADDER_SPEED = 1000;

		bool rotate_on_slopes_;
		const std::vector<SpriteType> types_;
		const CameraMovement camera_movement_;
		const bool despawn_when_dead_;
		std::unique_ptr<SpriteGraphics> graphics_;
		std::unique_ptr<SpriteComponent> component_;
		std::unique_ptr<SpriteMovement> movement_;
		SpriteStatus status_;

		const Direction::Horizontal direction_x_orig_;
		const Direction::Vertical direction_y_orig_;

		const bool impervious_;
		const int start_speed_walk_;
		const int top_speed_walk_;
		const int start_speed_run_;
		const int top_speed_run_;
		int start_speed_;
		int top_speed_;
		int top_speed_upward_;
		int top_speed_downward_;
		const double bounce_;

		int acceleration_x_ = 0;
		int acceleration_y_ = 0;
		int vx_at_change_ = 0;

		int gravity_start_speed_ = GRAVITY_START_SPEED;
		int gravity_top_speed_ = GRAVITY_TOP_SPEED;
		int jump_start_speed_;
		int jump_top_speed_normal_;
		int jump_top_speed_;
		bool on_ground_prev_ = false;
		bool can_jump_ = false;
		bool is_jumping_ = false;
		bool is_jumping_prev_ = false;
		bool jump_start_ = false;
		bool jump_end_ = false;
		bool is_bouncing_ = false;
		bool is_bouncing_prev_ = false;
		bool slide_jump_ = false;
		bool is_ducking_ = false;
		bool touching_ladder_prev_ = false;
		bool on_ladder_ = false;
		bool on_ladder_prev_ = false;
		bool looking_up_ = false;
		bool in_water_prev_ = false;
		bool is_sliding_prev_ = false;

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

		bool death_finished_ = false;
		bool dead_no_animation_ = false;
		TimerSimple death_timer_ = TimerSimple( 32, false );
		virtual void deathAction( Camera& camera );
		void defaultDeathAction( Camera& camera );
		void resetPosition();

		TimerSimple on_ground_padding_;
};