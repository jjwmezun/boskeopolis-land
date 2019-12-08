#include <cassert>
#include "option_system.hpp"
#include "render.hpp"

static constexpr int SPACE_BETWEEN_OPTIONS_MINIBLOCKS = 1;
static constexpr int SPACE_BETWEEN_OPTIONS_PIXELS = Unit::MiniBlocksToPixels( SPACE_BETWEEN_OPTIONS_MINIBLOCKS );

static int calculateBoxWidth( const std::vector<std::u32string>& options )
{
	int max_width = 0;
	for ( const std::u32string& name : options )
	{
		if ( name.size() > max_width )
		{
			max_width = name.size();
		}
	}
	if ( max_width > Unit::MINIBLOCKS_PER_SCREEN - 4 )
	{
		max_width = Unit::MINIBLOCKS_PER_SCREEN - 4;
	}
	return Unit::MiniBlocksToPixels( max_width + 2 );
}

OptionSystem::OptionSystem( const std::vector<std::u32string>& options, int y, int start_selection )
:
	options_ (),
	selection_ ( options.size() - 1, -1, start_selection )
{
	for ( const std::u32string& name : options )
	{
		options_.emplace_back( name, y, calculateBoxWidth( options ) );
		y += OptionBox::BOX_HEIGHT + SPACE_BETWEEN_OPTIONS_PIXELS;
	}
};

void OptionSystem::init()
{
	for ( auto& option : options_ )
	{
		option.init();
	}
};

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