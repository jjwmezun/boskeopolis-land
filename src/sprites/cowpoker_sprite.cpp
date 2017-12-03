#include "mezun_math.hpp"
#include "cowpoker_sprite.hpp"
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
	Sprite( std::move( gfx ), x, y, w, h, { SpriteType::ENEMY, SpriteType::BOPPABLE, SpriteType::DONT_RESPAWN }, 500, 1000, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY, true, true, true, false, .2, false, false, map_id ),
	type_ ( type ),
	throw_time_ ( 0 ),
	throw_counter_ ( 0 ),
	is_shooting_ ( false ),
	is_shooting_counter_ ( 0 )
{};

CowpokerSprite::~CowpokerSprite() {};

void CowpokerSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	switch ( direction_x_ )
	{
		case ( Direction::Horizontal::RIGHT ):
			graphics_->flip_x_ = true;
		break;

		default:
			graphics_->flip_x_ = false;
		break;
	}
	
	if ( is_shooting_ )
	{
		graphics_->current_frame_x_ = 17;
	}
	else
	{
		graphics_->current_frame_x_ = 0;
	}
	
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

	handleThrowing( sprites );
};

void CowpokerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( them.xSubPixels() > rightSubPixels() )
		{
			direction_x_ = Direction::Horizontal::RIGHT;
		}
		else
		{
			direction_x_ = Direction::Horizontal::LEFT;
		}
	}
};

int CowpokerSprite::throwTime() const
{
	return mezun::randInt( 80, 10 );
};

void CowpokerSprite::handleThrowing( SpriteSystem& sprites )
{
	if ( throw_counter_ >= throw_time_ )
	{
		int bullet_y = yPixels();
		if ( type_ == CowpokerType::SHORT )
		{
			bullet_y += 3;
		}
		
		throw_counter_ = 0;
		throw_time_ = throwTime();
		sprites.spawnEnemyBullet( centerXPixels(), bullet_y, Direction::horizontalToSimple( direction_x_ ) );
		is_shooting_ = true;
	}
	++throw_counter_;
};