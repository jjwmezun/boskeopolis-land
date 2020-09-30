#include "camera.hpp"
#include "input.hpp"
#include "level_state.hpp"
#include "maze_player_sprite.hpp"
#include "animated_graphics.hpp"

static constexpr int DEATH_SPIN_SPEED = 20;

MazePlayerSprite::MazePlayerSprite( int x, int y )
:
	Sprite
	(
		std::unique_ptr<SpriteGraphics>
		( new AnimatedGraphics(
			"sprites/autumn_maze.png",
			{ std::make_pair( 0, 0 ), std::make_pair( 15, 0 ) },
			false,
			false,
			0,
			-2,
			-1,
			3,
			2
		)),
		x,
		y,
		12,
		14,
		{ SpriteType::HERO },
		800, 3000, 1000, 6000, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, true, true, 0
	),
	death_spins_ ( 0 )
{};

MazePlayerSprite::~MazePlayerSprite() {};

void MazePlayerSprite::customUpdate( LevelState& level_state )
{
	inputMoveAllDirections();
	invincibilityFlicker( level_state.health() );
	level_state.camera().adjustCart( *this, level_state.currentMap() );
};

void MazePlayerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{};

void MazePlayerSprite::deathAction( LevelState& level_state )
{
	block_interact_ = false;

	if ( level_state.camera().offscreen( hit_box_ ) )
	{
		death_finished_ = true;
	}

	if ( death_spins_ > 3 )
	{
		changeMovement( SpriteMovement::Type::GROUNDED );
	}
	else
	{
		fullStopX();
		fullStopY();

		graphics_->rotation_ += DEATH_SPIN_SPEED;

		if ( ( int )( graphics_->rotation_ ) % 360 == 0 )
		{
			++death_spins_;
		}
	}
};
