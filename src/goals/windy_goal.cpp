#include "windy_goal.hpp"
#include "sprite.hpp"

WindyGoal::WindyGoal( int wind_strength, Direction::Horizontal direction )
:
	Goal( "O, fun: a lazy wind gimmick." ),
	wind_strength_ ( windStrength( direction, wind_strength ) )
{};

WindyGoal::~WindyGoal() {};

void WindyGoal::customInit( Sprite& hero, Level& level, InventoryLevel& inventory_screen, EventSystem& events, Health& health )
{
	Sprite::resistance_x_ = wind_strength_;
};

int WindyGoal::windStrength( Direction::Horizontal direction, unsigned int wind_strength )
{
	switch( direction )
	{
		case ( Direction::Horizontal::RIGHT ):
			return wind_strength; // Positive
		break;
			
		case ( Direction::Horizontal::LEFT ):
			return -wind_strength; // Negative
		break;
			
		case ( Direction::Horizontal::__NULL ):
			return wind_strength; // Either
		break;
	}
};