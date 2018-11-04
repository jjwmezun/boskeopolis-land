#include "camera.hpp"
#include "input.hpp"
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
			false,
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
		800, 3000, 1000, 6000, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, true, true, false, 0, 2, 2
	),
	death_spins_ ( 0 )
{};

MazePlayerSprite::~MazePlayerSprite() {};

void MazePlayerSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	inputMoveAllDirections();
	invincibilityFlicker( health );
	camera.adjustCart( *this, lvmap );
};

void MazePlayerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{};

void MazePlayerSprite::deathAction( const Camera& camera, EventSystem& events, const Map& lvmap )
{
	block_interact_ = false;

	if ( camera.offscreen( hit_box_ ) )
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

		if ( ( int )graphics_->rotation_ % 360 == 0 )
		{
			++death_spins_;
		}
	}
};
