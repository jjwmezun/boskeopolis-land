#include "localization.hpp"
#include "localization_language.hpp"
#include "mezun_math.hpp"
#include "news_ticker.hpp"
#include "render.hpp"
#include "wtext_obj.hpp"

static constexpr int START_POSITION = Unit::WINDOW_WIDTH_PIXELS + 64;

NewsTicker::NewsTicker( int y )
:
	y_ ( y ),
	w_ ( 0 ),
	texture_ ( Unit::MiniBlocksToPixels( Localization::getCurrentLanguage().getMaxNewsTickerMessageWidth() ), 8, START_POSITION, y ),
	selected_message_ ( Localization::getCurrentLanguage().getRandomNewsTickerMessage() )
{
	texture_.init();
	recalculateWidth();
};

NewsTicker::~NewsTicker()
{
	texture_.destroy();
};

void NewsTicker::update()
{
	texture_.moveLeft();
	if ( texture_.getX() + w_ < 0 )
	{
		texture_.setX( START_POSITION );
		selected_message_ = Localization::getCurrentLanguage().getRandomNewsTickerMessage();
		recalculateWidth();
		forceRedraw();
	}
};

void NewsTicker::render() const
{
	texture_.render();
};

void NewsTicker::forceRedraw()
{
	texture_.startDrawing();
	Render::clearScreenTransparency();
	WTextObj text( selected_message_, 0, 0 );
	text.render();
	texture_.endDrawing();
};

void NewsTicker::recalculateWidth()
{
	w_ = Unit::MiniBlocksToPixels( selected_message_.size() );
};