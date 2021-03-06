#pragma once

class Block;
class BlockSystem;
class Camera;
class Collision;
class EventSystem;
class LevelState;
class Health;
class Level;
class Map;
class SpriteGraphics;
class SpriteSystem;

#include "direction.hpp"
#include "render.hpp"
#include <memory>
#include "object.hpp"
#include "renderable.hpp"
#include <SDL2/SDL.h>
#include "sprite_component.hpp"
#include "timer_simple_t.hpp"
#include "unit.hpp"

#include "sprite_movement.hpp"
#include "grounded_sprite_movement.hpp"
#include "fluttering_sprite_movement.hpp"
#include "swimming_sprite_movement.hpp"
#include "angled_sprite_movement.hpp"
#include "stuck_sprite_movement.hpp"

class Sprite : public Object
{
	public:
		struct DuckData
		{
			bool can_duck_while_jumping;
			int y_change;
			int h_change;
			int gfx_y_change;
			int gfx_h_change;
		};
		struct UnDuckData
		{
			int y_change;
			int h_change;
			int gfx_y_change;
			int gfx_h_change;
		};
		enum class SpriteType
		{
			NOTYPE,
			HERO,
			ENEMY,
			RIVAL,
			BOPPABLE,
			CLOUD_PLATFORM,
			CLOUD_MONSTER,
			LIGHTNING,
			HEROS_BULLET,
			DEATH_COUNT,
			ICICLE,
			TREASURE_CHEST,
			PHASE_THROUGH,
			TRUCK_PLATFORM,
			CRAB,
			CAMERA_MOVE,
			MOVEABLE_BLOCK,
			FALLING_BOUGH,
			RAFT,
			WATER_SPOUT,
			BARREL,
			SPARK,
			MANSION_GHOST,
			SWAMP_POLE,
			PI_BLOCK,
			ZOMBIE,
			SOLID,
			GHOST_PLAYER,
			BULL,
			BABY_MOSES
		};

		enum class CameraMovement
		{
			RESET_INSTANTLY_OFFSCREEN,
			RESET_OFFSCREEN_AND_AWAY,
			PERMANENT,
			PAUSE_OFFSCREEN,
			DESPAWN_OFFSCREEN
		};

		union Misc
		{
			int id;
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
			double bounce_ = .2,
			int map_id = -1,
			Unit::Layer layer = Unit::Layer::SPRITES_1
		);

		virtual ~Sprite();
		Sprite( const Sprite& ) = delete;
		Sprite& operator=( const Sprite& ) = delete;

		static constexpr double TRACTION_NORMAL = 1.2;
		static constexpr double TRACTION_ICY = 1.025;
		static constexpr int RESISTANCE_X_NORMAL = 0;
		static constexpr int RESISTANCE_X_WINDY = 100;

		static constexpr int GRAVITY_START_SPEED_NORMAL = 500;
		static constexpr int GRAVITY_TOP_SPEED_NORMAL = 4000;

		static void setGravity( int gravity );
		void setGravityModifier( double gravity );

		void update( LevelState& level_state );
		virtual void render( const Camera& camera ) const;
		void drawHitBox( const Camera& camera ) const;
		void renderWithHitbox( Camera& camera );
		void interact( Sprite& them, LevelState& level_state );

		virtual void customUpdate( LevelState& level_state ) = 0;
		virtual void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) = 0;
		virtual void reset();

		bool onGround() const;
		bool onGroundPrev() const;
		bool onGroundPadding() const;
		virtual bool canJump() const;
		bool isJumping() const;
		bool isJumpingPrev() const;
		bool isDucking() const;
		bool lookingUp() const;

		bool isMoving() const;
		bool isRunning() const;
		SpriteMovement::Type movementType() const;
		bool hasMovementType( SpriteMovement::Type type ) const;

		bool isDead() const;
		bool fellInBottomlessPit( const Map& lvmap ) const;

		bool collidedAny() const;
		bool collidedLeft() const;
		bool collidedRight() const;
		bool collideTopOnly( const Collision& collision, const Object& them ) const;

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

		const Collision testCollision( const sdl2::SDLRect& hitbox ) const;
		const Collision testCollision( const Object& them ) const;
		const Collision testBlockCollision( const sdl2::SDLRect& hitbox ) const;
		const Collision testBlockCollision( const Object& them ) const;
		void collideStopAny( const Collision& collision );
		void collideStopXLeft( int overlap );
		void collideStopXRight( int overlap );
		void collideStopYBottom( int overlap );
		void collideStopYTop( int overlap );
		bool collideBottomOnly( const Collision& collision, const Object& other ) const;
		bool collideBottomOnly( const Collision& collision, const sdl2::SDLRect& other ) const;

		void slowFall();
		void fastFall();
		void duck( const DuckData& duck_data );
		void unduck( const UnDuckData& unduck_data );
		void stopDucking();
		void touchLadder();
		void grabLadder();
		void releaseLadder();
		void lookUp();
		void stopLookingUp();

		void run();
		void stopRunning();

		void kill();
		void killNoAnimation();

		void boundaries( Camera& camera, Map& lvmap );
		void setPosition( int x, int y );
		void changeMovement( SpriteMovement::Type type );
		static const SpriteMovement* getMovement( SpriteMovement::Type type );

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
		static int gravity_start_speed_;
		static int gravity_top_speed_;
		static double traction_;
		static int resistance_x_;

		void position();
		void positionX();
		void positionY();
		virtual void deathAction( LevelState& level_state );
		void defaultDeathAction( LevelState& level_state );
		void resetPosition();
		void invincibilityFlicker( const Health& health );
		void changeRenderableLayer( LevelState& level_state, Unit::Layer layer );

		sdl2::SDLRect justAbove() const;
		bool blocksJustAbove( const BlockSystem& blocks ) const;
		sdl2::SDLRect justBelow() const;
		bool blocksJustBelow( const BlockSystem& blocks ) const;
		sdl2::SDLRect justLeft() const;
		bool blocksJustLeft( const BlockSystem& blocks ) const;
		sdl2::SDLRect justRight() const;
		bool blocksJustRight( const BlockSystem& blocks ) const;
		bool inBox( const sdl2::SDLRect& box ) const;

		void turnOnEdge( const BlockSystem& blocks );
		void turnOnCollide();
		void moveInDirectionX();
		void moveInDirectionY();
		void moveInDirection();
		void inputMoveAllDirections();
		void containCameraX( const Camera& camera );
		void containCameraY( const Camera& camera );
		void flipGraphicsOnRight();
		void moveToward( const Sprite& them );

		bool isLeftOf( const Object& them ) const;
		bool isRightOf( const Object& them ) const;
		bool isUpsideDown() const;

		static const SpriteMovement floating_;
		static const GroundedSpriteMovement grounded_;
		static const FlutteringSpriteMovement fluttering_;
		static const SwimmingSpriteMovement swimming_;
		static const AngledSpriteMovement angled_;
		static const StuckSpriteMovement stuck_movement_;

		bool despawn_when_dead_;
		bool jump_lock_;
		bool on_ground_;
		bool in_water_;
		bool on_ground_prev_ = false;
		bool is_jumping_ = false;
		bool is_jumping_prev_ = false;
		bool jump_start_ = false;
		bool jump_end_ = false;
		bool is_bouncing_ = false;
		bool is_bouncing_prev_ = false;
		bool slide_jump_ = false;
		bool is_ducking_ = false;
		bool on_ladder_ = false;
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
		bool sprite_interact_from_this_to_others_only_ = false;
		bool is_running_ = false;
		bool is_moving_ = false;
		bool is_dead_ = false;
		bool death_finished_ = false;
		bool dead_no_animation_ = false;
		int map_id_;
		int start_speed_walk_;
		int top_speed_walk_;
		int start_speed_run_;
		int top_speed_run_;
		int vx_;
		int vy_;
		int x_prev_;
		int y_prev_;
		int base_jump_start_speed_;
		int base_jump_top_speed_;
		int start_speed_;
		int top_speed_;
		int top_speed_upward_;
		int top_speed_downward_;
		int acceleration_x_;
		int acceleration_y_;
		int fall_start_speed_;
		int fall_top_speed_;
		int jump_start_speed_;
		int jump_top_speed_normal_;
		int jump_top_speed_;
		int bounce_height_;
		Unit::Layer layer_;
		int renderable_id_;
		int system_id_;
		Misc misc_;
		Direction::Horizontal direction_x_orig_;
		Direction::Vertical direction_y_orig_;
		Direction::Simple direction_;
		Direction::Horizontal direction_x_;
		Direction::Vertical direction_y_;
		Direction::Horizontal on_slope_;
		CameraMovement camera_movement_;
		const SpriteMovement* movement_;
		std::unique_ptr<SpriteGraphics> graphics_;
		std::vector<SpriteType> types_;
		double bounce_;
		double gravity_modifier_;
		sdl2::SDLRect original_hit_box_;
		TimerSimpleT<32, false> death_timer_;
		TimerSimpleT<4, false> on_ground_padding_;
};
