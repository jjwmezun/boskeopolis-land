#include "localization.hpp"
#include "localization_language.hpp"
#include "mezun_math.hpp"
#include "news_ticker.hpp"
#include "render.hpp"
#include "wtext_obj.hpp"

static constexpr int START_POSITION = Unit::WINDOW_WIDTH_PIXELS + 64;
static constexpr int LEFT_BORDER = 6;

NewsTicker::NewsTicker( int y )
:
	x_ ( START_POSITION ),
	y_ ( y ),
	w_ ( 0 ),
	texture_width_ ( Unit::MiniBlocksToPixels( Localization::getCurrentLanguage().getMaxNewsTickerMessageWidth() ) ),
	texture_ ( nullptr ),
	selected_message_ ( Localization::getCurrentLanguage().getRandomNewsTickerMessage() ),
	src_ ( 0, 0, 0, WTextCharacter::SIZE_PIXELS ),
	dest_ ( START_POSITION, y, 0, WTextCharacter::SIZE_PIXELS )
{
	texture_ = Render::createRenderBox( texture_width_, dest_.h );
	recalculateWidth();
};

NewsTicker::~NewsTicker()
{
	if ( texture_ != nullptr )
	{
		SDL_DestroyTexture( texture_ );
	}
};

void NewsTicker::update()
{
	--x_;
	if ( testTickerIsCutOffOnLeftSide() )
	{
		// Cut off left side ’stead o’ moving farther left
		// so it doesn’t get drawn past left border.
		++src_.x;
		// Make sure texture widths don’t go beyond right border
		// or end o’ texture box to prevent drawing past
		// right border or stretching the texture.
		src_.w = dest_.w = std::min( Unit::WINDOW_WIDTH_PIXELS - ( LEFT_BORDER * 2 ), texture_width_ - src_.x );
	}
	else if ( testTickerHasNotReachedScreenYet() )
	{
		// Simple way to just not show ticker yet.
		// Better not to do any calculation & risk
		// getting negative #, which may still show up
		// in a weird way.
		src_.w = dest_.w = 0;
	}
	else
	{
		// Move drawing point o’ ticker leftward.
		dest_.x = x_;
		// Make sure we don’t draw past right border.
		src_.w = dest_.w = Unit::WINDOW_WIDTH_PIXELS - LEFT_BORDER - x_;
	}

	if ( right() < 0 )
	{
		generateNewNewsItem();
	}
};

void NewsTicker::render() const
{
	Render::renderObject( texture_, src_, dest_ );
};

void NewsTicker::forceRedraw()
{
	Render::setRenderTarget( texture_ );
	Render::clearScreenTransparency();
	WTextObj text( selected_message_, 0, 0, WTextCharacter::Color::BLACK, Unit::MiniBlocksToPixels( Localization::getCurrentLanguage().getMaxNewsTickerMessageWidth() ) );
	text.render();
	Render::releaseRenderTarget();
};

void NewsTicker::recalculateWidth()
{
	w_ = Unit::MiniBlocksToPixels( selected_message_.size() );
};

void NewsTicker::generateNewNewsItem()
{
	x_ = START_POSITION;
	src_.x = 0;
	dest_.x = START_POSITION;
	selected_message_ = Localization::getCurrentLanguage().getRandomNewsTickerMessage();
	recalculateWidth();
	forceRedraw();
};

int NewsTicker::right() const
{
	return x_ + w_;
};

bool NewsTicker::testTickerIsCutOffOnLeftSide() const
{
	return x_ < LEFT_BORDER;
};

bool NewsTicker::testTickerHasNotReachedScreenYet() const
{
	return x_ > Unit::WINDOW_WIDTH_PIXELS - LEFT_BORDER;
};