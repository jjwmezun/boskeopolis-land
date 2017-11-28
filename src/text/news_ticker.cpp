#include "mezun_helpers.hpp"
#include "news_ticker.hpp"

const std::vector<std::string> strings_ =
{
	"Kitties Want Answers In Possible Kitty Kibble Shortage",
	"Thank you for reading this message. This message loves you.",
	"Clean up on aisle Delfino.",
	"Experts declare, SimCity 3000 Marquee + platformer = bad game - 9/10.",
	"Experts declare, changing code through simple search & replace ne'er messes with in-Main:: text.",
	"Experts declare, those who died are justified by wearing the badge, they're your chosen whites. Details @ 11.",
	"Experts declare, \"Playing Railroad\" taking way too long to finish.",
	"...& that's the news you can choose.",
	"NEWSFLASH: Crazy woman running round grabbing floating gems everywhere she goes. Details @ 11.",
	"¡AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH!",
	"WARNING: Playing games for too long can be dangerous. Take 15-minute breaks 'tween each session or we'll come to smash your hand with a hammer-& yes, Jacob, we'll find you @ 2456 Acorn Avenue. We know where you live.",
	"NEWSFLASH: Despite all o' my rage, I am still just a rat in a cage. Details @ 11.",
	"Thou shalt not waste thine time reading these dumb newsflashes 'stead o' playing the game - ¡Look out for that Bad Apple! ¡You fool!",
	"If you don't know 'bout the tavern, then it is time.",
	"Achievement Unlocked: You read a pointless message."
};

TextObj NewsTicker::make( int y )
{
	return TextObj
	(
		newMessage(),
		Unit::WINDOW_WIDTH_PIXELS,
		y,
		Text::FontColor::DARK_GRAY,
		Text::FontAlign::LEFT,
		Text::FontColor::__NULL,
		false,
		Text::DEFAULT_LINE_LENGTH,
		-1,
		1,
		std::unique_ptr<TextComponent>
		(
			new NewsTicker()
		)
	);
};

NewsTicker::NewsTicker() : marquee_ ( Direction::Horizontal::LEFT, 1 ) {};
NewsTicker::~NewsTicker() {};

void NewsTicker::update( TextObj& text )
{
	// Check just before position changes from marquee.
	if ( text.right() == 0 )
	{
		changeMessage( text );
	}

	marquee_.update( text );
};

std::string NewsTicker::newMessage()
{
	return mezun::randomListItem( strings_ );
};

void NewsTicker::changeMessage( TextObj& text )
{
	text.words_ = newMessage();
	text.x_ = -text.width(); // Readjust position so still left o' screen with new string size.
};