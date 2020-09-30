#include "event_system.hpp"
#include "switch_graphics.hpp"

SwitchGraphics::SwitchGraphics
(
	std::unique_ptr<SpriteGraphics> gfx_off,
	std::unique_ptr<SpriteGraphics> gfx_on
)
:
	SpriteGraphics( "" ),
	current_gfx_ ( gfx_off_.get() ),
	gfx_off_ ( std::move( gfx_off ) ),
	gfx_on_ ( std::move( gfx_on ) )
{};

void SwitchGraphics::update( const EventSystem& events )
{
	current_gfx_ = ( events.isSwitchOn() ) ? gfx_on_.get() : gfx_off_.get();
	current_gfx_->update( events );
};

void SwitchGraphics::render( const sdl2::SDLRect& bound_box, const Camera* camera ) const
{
	current_gfx_->render( bound_box, camera );
};
