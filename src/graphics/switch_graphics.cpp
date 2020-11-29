#include <cassert>
#include "event_system.hpp"
#include "switch_graphics.hpp"

SwitchGraphics::SwitchGraphics
(
	std::vector<std::unique_ptr<SpriteGraphics>>&& gfx
)
:
	SpriteGraphics( "" ),
	current_gfx_ ( nullptr ),
	gfx_ ( std::move( gfx ) )
{
	assert( gfx_.size() > 0 );
	current_gfx_ = gfx_[ 0 ].get();
};

void SwitchGraphics::update( const EventSystem& events )
{
	const int switch_value = events.getSwitchValue();
	if ( switch_value < gfx_.size() )
	{
		current_gfx_ = gfx_[ switch_value ].get();
	}
	current_gfx_->update( events );
};

void SwitchGraphics::render( const sdl2::SDLRect& bound_box, const Camera* camera ) const
{
	current_gfx_->render( bound_box, camera );
};
