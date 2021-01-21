#include "mezun_math.hpp"
#include "bullet_sprite.hpp"
#include "cowpoker_sprite.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

std::unique_ptr<Sprite> CowpokerSprite::TallCowpokerSprite( int x, int y, int map_id )
{
	return std::unique_ptr<Sprite> ( new CowpokerSprite( x, y, 14, 22, CowpokerType::TALL, std::make_unique<SpriteGraphics> ( "sprites/cowpoker.png", 0, 48, false, false, 0, true, -1, -3, 2, 4 ), map_id ) );
};

std::unique_ptr<Sprite> CowpokerSprite::ShortCowpokerSprite( int x, int y, int map_id )
{
	return std::unique_ptr<Sprite> ( new CowpokerSprite( x, y, 10, 14, CowpokerType::SHORT, std::make_unique<SpriteGraphics> ( "sprites/cowpoker.png", 0, 100, false, false, 0, true, -5, -1, 6, 2 ), map_id ) );
};

CowpokerSprite::CowpokerSprite( int x, int y, int w, int h, CowpokerType type, std::unique_ptr<SpriteGraphics>&& gfx, int map_id )
:
	Sprite( std::move( gfx ), x, y, w, h, { SpriteType::ENEMY, SpriteType::BOPPABLE, SpriteType::DEATH_COUNT }, 500, 1000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true, true, true, .2, map_id ),
	type_ ( type ),
	is_shooting_ ( false ),
	is_shooting_counter_ ( 0 ),
	throw_timer_ ()
{};

CowpokerSprite::~CowpokerSprite() {};

void CowpokerSprite::customUpdate( LevelState& level_state )
{
	handleGraphics();
	handleShooting( level_state.sprites() );
};

void CowpokerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		direction_x_ = ( them.xSubPixels() > rightSubPixels() )
			? Direction::Horizontal::RIGHT
			: Direction::Horizontal::LEFT;
	}
};

void CowpokerSprite::handleGraphics()
{
	flipGraphicsOnRight();
	graphics_->current_frame_x_ = ( is_shooting_ ) ? 17 : 0;
	if ( is_shooting_ )
	{
		if ( is_shooting_counter_ >= 8 )
		{
			is_shooting_ = false;
			is_shooting_counter_ = 0;
		}
		else
		{
			++is_shooting_counter_;
		}
	}
}

void CowpokerSprite::handleShooting( SpriteSystem& sprites )
{
	if ( throw_timer_.update() )
	{
		int bullet_y = yPixels();
		if ( type_ == CowpokerType::SHORT )
		{
			bullet_y += 3;
		}
		sprites.spawn( std::make_unique<BulletSprite> ( centerXPixels(), bullet_y, Direction::horizontalToSimple( direction_x_ ), false ) );
		is_shooting_ = true;
	}
};
