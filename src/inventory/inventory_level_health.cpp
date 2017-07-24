#include <iostream>
#include "health.hpp"
#include "inventory.hpp"
#include "inventory_level_health.hpp"

InventoryLevelHealth::InventoryLevelHealth( int y )
:
	dest_ ( { X, y, 8, 8 } ),
	gfx_ ( "tilesets/universal.png", 32, 8 ),
	empty_gfx_ ( "tilesets/universal.png", 32, 0 ),
	hearts_shown_ ( 0 )
{};

void InventoryLevelHealth::update( const Health& health )
{
	if ( hearts_shown_ > health.hp() )
	{
		--hearts_shown_;
	}

	if ( hearts_shown_ < health.hp() )
	{
		++hearts_shown_;
	}

	if ( hearts_shown_ < 0 )
	{
		hearts_shown_ = 0;
	}
};

void InventoryLevelHealth::render()
{
	for ( int i = 0; i < Health::maxHP(); ++i )
	{
		if ( i < hearts_shown_ )
		{
			gfx_.render( dest_, nullptr );
		}
		else
		{
			empty_gfx_.render( dest_, nullptr );
		}

		dest_.x += 8;
	}

	dest_.x = X;
};