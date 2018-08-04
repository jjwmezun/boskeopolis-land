#include "options_bg.hpp"
#include "render.hpp"

OptionsBG::OptionsBG()
:
	bg_ ( "bg/leaves.png", 400, 300, 0, 0, 0, 0, 1, MapLayerImage::REPEAT_INFINITE, MapLayerImage::REPEAT_INFINITE, -250, -250 )
{};

void OptionsBG::update()
{
	bg_.move( 400, Render::getScreen() );
};

void OptionsBG::render()
{
	bg_.render( Render::getScreen() );
};
