#include "block_component_very_steep.hpp"
#include "block.hpp"
#include "collision.hpp"
#include "sprite.hpp"

BlockComponentVerySteep::BlockComponentVerySteep( Direction::Horizontal direction, int height, int relative, int resistance, int extra_push )
:
	direction_ ( direction ),
	height_ ( height ),
	relative_ ( relative ),
	resistance_ ( resistance ),
	extra_push_ ( extra_push )
{};

BlockComponentVerySteep::~BlockComponentVerySteep() {};

void BlockComponentVerySteep::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	if ( collision.collideAny() )
	{
		const bool condition = ( direction_ == Direction::Horizontal::LEFT )
		?
		(
			sprite.centerXSubPixels() > block.leftSubPixels() + ( ( 16000 / height_ ) * ( relative_ - 1 ) ) &&
			sprite.centerXSubPixels() < block.rightSubPixels()
		)
		:
		(
			sprite.centerXSubPixels() > block.leftSubPixels() &&
			sprite.centerXSubPixels() < block.rightSubPixels() - ( ( 16000 / height_ ) * ( relative_ - 1 ) )
		);

		if ( condition )
		{
			sprite.on_slope_ = direction_;
			sprite.collideStopYBottom( 0 );
			if ( sprite.vy_ >= 0 )
			{
				sprite.hit_box_.y =
					block.bottomSubPixels() -
					getSlopeTopFromX( sprite, block ) -
					sprite.heightSubPixels();
				adjustSpritesSpeed( sprite );
			}
		}
	}
};

int BlockComponentVerySteep::getSlopeTopFromX( const Sprite& sprite, const Block& block ) const
{
	const int L = ( direction_ == Direction::Horizontal::LEFT )
		? sprite.centerXSubPixels() - ( block.leftSubPixels() + ( ( 16000 / height_ ) * ( relative_ - 1)) )
		: ( block.rightSubPixels() - sprite.centerXSubPixels() - ( ( 16000 / height_ ) * ( relative_ - 1)) );
	const int V = std::min( std::max( ( L * height_ ) + 2000, 0 ), 16000 );
	return V;
}


void BlockComponentVerySteep::adjustSpritesSpeed( Sprite& sprite ) const
{
	switch ( direction_ )
	{
		case ( Direction::Horizontal::LEFT ):
		{
			if ( sprite.vx_ >= 0 )
			{
				sprite.vx_ /= getResistance();
			}
			else if ( sprite.vx_ <= 0 )
			{
				sprite.vx_ *= getResistance();
			}

			sprite.vx_ -= extra_push_;
		}
		break;

		case ( Direction::Horizontal::RIGHT ):
		{
			if ( sprite.vx_ <= 0 )
			{
				sprite.vx_ /= getResistance();
			}
			else if ( sprite.vx_ >= 0 )
			{
				sprite.vx_ *= getResistance();
			}

			sprite.vx_ += extra_push_;
		}
		break;
	}
}

double BlockComponentVerySteep::getResistance() const
{
	//
	// 'Cause this # will be multiplied or divided by, 1 is the
	// normalizing factor ( no resistance ), with the percentage
	// added to that factor.
	//
	return 1.0 + ( ( double )( resistance_ ) / 100.0 );
}
