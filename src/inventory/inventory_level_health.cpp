#include "health.hpp"
#include "inventory_level_health.hpp"
#include "render.hpp"

static constexpr int X = 90;
static constexpr int Y = 10;

static constexpr int calculateGraphicsWidth( int number_of_hearts )
{
	return 9 * number_of_hearts - 1;
};

InventoryLevelHealth::InventoryLevelHealth( int y )
:
	hearts_shown_ ( 0 ),
	image_ ( "bg/level-select-characters.png" ),
	empty_heart_src_ ( 0, 168, calculateGraphicsWidth( Health::maxHP() ), 8 ),
	empty_heart_dest_ ( X, Y, calculateGraphicsWidth( Health::maxHP() ), 8 ),
	full_heart_src_ ( 0, 176, calculateGraphicsWidth( Health::maxHP() ), 8 ),
	full_heart_dest_ ( X, Y, calculateGraphicsWidth( Health::maxHP() ), 8 )
{};

bool InventoryLevelHealth::update( const Health& health )
{
	const int hearts_shown_momento = hearts_shown_;
	if ( hearts_shown_ > health.hp() && hearts_shown_ > 0 )
	{
		--hearts_shown_;
	}
	else if ( hearts_shown_ < health.hp() )
	{
		++hearts_shown_;
	}
	return hearts_shown_momento != hearts_shown_;
};

void InventoryLevelHealth::render()
{
	Render::renderRect( empty_heart_dest_, 1 );
	Render::renderObject( image_, empty_heart_src_, empty_heart_dest_ );
	full_heart_dest_.w = full_heart_src_.w = calculateGraphicsWidth( hearts_shown_ );
	Render::renderObject( image_, full_heart_src_, full_heart_dest_ );
};