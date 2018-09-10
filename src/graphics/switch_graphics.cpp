#include "event_system.hpp"
#include "switch_graphics.hpp"

SwitchGraphics::SwitchGraphics
(
	std::unique_ptr<SpriteGraphics> gfx_off,
	std::unique_ptr<SpriteGraphics> gfx_on
)
:
	SpriteGraphics( "" ),
	gfx_off_ ( std::move( gfx_off ) ),
	gfx_on_ ( std::move( gfx_on ) ),
	current_gfx_ ( gfx_off_.get() )
{};

void SwitchGraphics::update( const EventSystem& events )
{
	switch ( events.switchOn() )
	{
		case ( true ):
			current_gfx_ = gfx_on_.get();
		break;

		case ( false ):
			current_gfx_ = gfx_off_.get();
		break;
	}
	current_gfx_->update( events );
};

void SwitchGraphics::render( const sdl2::SDLRect& bound_box, const Camera* camera, bool priority, SDL_Texture* texture ) const
{
	current_gfx_->render( bound_box, camera, priority );
};
