#include "option_system.hpp"
#include "render.hpp"

static constexpr int SPACE_BETWEEN_OPTIONS_MINIBLOCKS = 1;
static constexpr int SPACE_BETWEEN_OPTIONS_PIXELS = Unit::MiniBlocksToPixels( SPACE_BETWEEN_OPTIONS_MINIBLOCKS );

OptionSystem::OptionSystem( std::vector<std::string> options, int option_width, int y )
:
	options_ (),
	selection_ ( options.size() - 1 )
{
	for ( auto& name : options )
	{
		options_.emplace_back( name, y, option_width );
		y += OptionBox::BOX_HEIGHT + SPACE_BETWEEN_OPTIONS_PIXELS;
	}
};
OptionSystem::~OptionSystem() {};

int OptionSystem::selection() const { return selection_.selection(); };

void OptionSystem::update()
{
	updateOptions();
	selection_.update();
};

void OptionSystem::render() const
{
	for ( const auto& option : options_ )
	{
		option.render();
	}
};

void OptionSystem::updateOptions()
{
	for ( int i = 0; i < options_.size(); ++i )
	{
		if ( i == selection_.selection() )
		{
			options_[ i ].setToCurrent();
		}
		else if ( i == selection_.previousSelection() )
		{
			options_[ i ].setToPrevious();
		}
		options_[ i ].update();
	}
};
