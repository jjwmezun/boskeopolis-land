#include "level_graphics_system.hpp"

LevelGraphicsSystem::LevelGraphicsSystem()
:
	graphics_ (),
	graphics_lookup_ (),
	latest_index_ ( 0 )
{};

void LevelGraphicsSystem::render( const Camera* camera ) const
{
	for ( const auto& g : graphics_ )
	{
		g.render( camera );
	}
};