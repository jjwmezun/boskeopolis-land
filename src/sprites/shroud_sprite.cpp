#include "camera.h"
#include "collision.h"
#include "shroud_sprite.h"
#include "sprite_graphics.h"

ShroudSprite::ShroudSprite( int x, int y )
:
	Sprite( nullptr, x, y, Unit::BlocksToPixels( 38 ), Unit::BlocksToPixels( 12 ) ),
	top_ ( { x + Unit::BlocksToPixels( 11 ), y, Unit::BlocksToPixels( 20 ), Unit::BlocksToPixels( 6 ) } ),
	bottom_ ( { x, y + Unit::BlocksToPixels( 6 ), Unit::BlocksToPixels( 38 ), Unit::BlocksToPixels( 6 ) } ),
	state_ ( ShroudState::VISIBLE ),
	alpha_ ( 255, 255, 0 ),
	activated_ ( false )
{
};

ShroudSprite::~ShroudSprite() {};

void ShroudSprite::customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
{
	switch ( state_ )
	{
		case ( ShroudState::VISIBLE ):
			if ( activated_ )
			{
				state_ = ShroudState::HIDING;
			}
		break;
		case ( ShroudState::HIDING ):
			if ( !activated_ )
			{
				state_ = ShroudState::RETURNING;
			}
			else if ( alpha_ == 0 )
			{
				state_ = ShroudState::HIDDEN;
			}
			else
			{
				alpha_ -= TRANS_SPEED;
			}
		break;
		case ( ShroudState::RETURNING ):
			if ( activated_ )
			{
				state_ = ShroudState::HIDING;
			}
			else if ( alpha_ == 255 )
			{
				state_ = ShroudState::VISIBLE;
			}
			else
			{
				alpha_ += TRANS_SPEED;
			}
		break;
		case ( ShroudState::HIDDEN ):
			if ( !activated_ )
			{
				state_ = ShroudState::RETURNING;
			}
		break;
	}
	
	activated_ = false;
};

void ShroudSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		activated_ = true;
	}
};

void ShroudSprite::render( Graphics& graphics, Camera& camera, bool priority )
{
	if ( priority && state_ != ShroudState::HIDDEN )
	{
		sdl2::SDLRect top = top_;
		sdl2::SDLRect bottom = bottom_;

		top.x = camera.relativeX( top );
		top.y = camera.relativeY( top );
		bottom.x = camera.relativeX( bottom );
		bottom.y = camera.relativeY( bottom );
		
		graphics.renderRect( top, COLOR, alpha_() );
		graphics.renderRect( bottom, COLOR, alpha_() );
	}
};