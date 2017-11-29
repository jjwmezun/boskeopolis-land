#include "audio.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "falling_gem_sprite.hpp"
#include "inventory.hpp"
#include "animated_graphics.hpp"

FallingGemSprite::FallingGemSprite( int x, int y )
:
	Sprite( std::make_unique<AnimatedGraphics> ( "tilesets/universal.png", std::vector<std::pair<int, int>>( { std::make_pair( 3, 64 ), std::make_pair( 19, 64 ), std::make_pair( 35, 64 ), std::make_pair( 51, 64 ), std::make_pair( 67, 64 ), std::make_pair( 83, 64 ) } ) ), x + 3, y, 10, 16, {}, 0, 0, 0, 0, Direction::Horizontal::RIGHT, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT, false, false ),
	active_ ( false )
{};

FallingGemSprite::~FallingGemSprite() {};

void FallingGemSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	if ( active_ )
	{
		graphics_->visible_ = true;
		switch ( direction_x_ )
		{
			case ( Direction::Horizontal::LEFT ):
				hit_box_.x = original_hit_box_.x - 8000;
			break;

			case ( Direction::Horizontal::RIGHT ):
				hit_box_.x = original_hit_box_.x + 8000; 
			break;
		}

		if ( camera.offscreenBottom( hit_box_ ) )
		{
			respawn( camera );
		}
	}
	else
	{
		graphics_->visible_ = false;
		resetPosition();
		direction_x_ = Direction::Horizontal::RIGHT;
	}
};

void FallingGemSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		active_ = testActive( them );

		if ( active_ && their_collision.collideAny() )
		{
			Inventory::addFunds( 100 );
			Audio::playSound( Audio::SoundType::GEM );
		}
	}
};

void FallingGemSprite::respawn( const Camera& camera )
{
	hit_box_.y = Unit::PixelsToSubPixels( camera.y() ) - hit_box_.h;
	vy_ = 0;
	acceleration_y_ = 0;
	direction_x_ = Direction::switchHorizontal( direction_x_ );
};

bool FallingGemSprite::testActive( const Sprite& hero )
{
	return
		hero.bottomSubPixels() < Unit::BlocksToSubPixels( 73 ) + 4000 &&
		hero.hit_box_.x > Unit::BlocksToSubPixels( 8 ) &&
		hero.rightSubPixels() < Unit::BlocksToSubPixels( 36 );
};