#include "localization.hpp"
#include "option_selection_grid.hpp"
#include "option_selection_vertical.hpp"
#include "option_system.hpp"
#include "render.hpp"

static constexpr int SPACE_BETWEEN_OPTIONS_PIXELS = WTextCharacter::SIZE_PIXELS;
static constexpr int GRID_WIDTH = Unit::WINDOW_WIDTH_PIXELS - ( SPACE_BETWEEN_OPTIONS_PIXELS * 2 );

static constexpr int calculateNumberOfGridColumns( int box_width )
{
    return ( int )( std::ceil( ( double )( GRID_WIDTH ) / ( double )( box_width + SPACE_BETWEEN_OPTIONS_PIXELS ) ) );
};

OptionSystem OptionSystem::generateGridOptionSystem( const std::vector<std::u32string>& option_strings )
{
	std::vector<OptionBox> options = {};
    const int box_width = OptionBox::calculateMaxBoxWidthWithPadding( option_strings );
    const int number_of_cols = calculateNumberOfGridColumns( box_width );
    const int number_of_rows = ( int )( std::ceil( ( double )( option_strings.size() ) / ( double )( number_of_cols ) ) );
    const int number_of_col_spaces = number_of_cols - 1;
    const int total_box_width = box_width * number_of_cols;
    const int total_space_width = GRID_WIDTH - total_box_width;
    const int space_width = ( int )( std::floor( ( double )( total_space_width ) / ( double )( number_of_col_spaces ) ) );

    int i = 0;
    int y_position = 56;
    for ( int y = 0; y < number_of_rows; ++y )
    {
        int x_position = SPACE_BETWEEN_OPTIONS_PIXELS;
        for ( int x = 0; x < number_of_cols; ++x )
        {
            if ( i == option_strings.size() )
            {
                break;
            }
            options.emplace_back( option_strings[ i ], y_position, box_width, x_position );
            x_position += ( box_width + space_width );
            ++i;
        }
        y_position += ( OptionBox::BOX_HEIGHT + SPACE_BETWEEN_OPTIONS_PIXELS );
    }
	return OptionSystem( std::move( options ), std::make_unique<OptionSelectionGrid>( number_of_rows, number_of_cols, option_strings.size() - 1 ) );
};

OptionSystem OptionSystem::generateVerticalOptionSystem( const std::vector<std::u32string>& option_strings, int y, int start_selection )
{
	std::vector<OptionBox> options = {};
	const int box_width = OptionBox::calculateMaxBoxWidthWithPadding( option_strings );
	for ( const std::u32string& name : option_strings )
	{
		options.emplace_back( name, y, box_width );
		y += OptionBox::BOX_HEIGHT + SPACE_BETWEEN_OPTIONS_PIXELS;
	}
	return OptionSystem( std::move( options ), std::make_unique<OptionSelectionVertical>( option_strings.size() - 1, -1, start_selection ) );
};

OptionSystem::OptionSystem( std::vector<OptionBox>&& options, std::unique_ptr<OptionSelection>&& selection )
:
	options_ ( std::move( options ) ),
	selection_ ( std::move( selection ) )
{};

void OptionSystem::init()
{
	for ( auto& option : options_ )
	{
		option.init();
	}
};

void OptionSystem::initForLanguageSelect()
{	
	const int remember_language = Localization::getCurrentLanguageIndex();
	for ( int i = 0; i < options_.size(); ++i )
	{
		Localization::setLanguage( i );
		options_[ i ].init();
	}
	Localization::setLanguage( remember_language );
};

int OptionSystem::selection() const { return selection_->selection(); };

void OptionSystem::update()
{
	updateOptions();
	selection_->update();
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
		if ( i == selection_->selection() )
		{
			options_[ i ].setToCurrent();
		}
		else if ( i == selection_->previousSelection() )
		{
			options_[ i ].setToPrevious();
		}
		options_[ i ].update();
	}
};

void OptionSystem::setSelectedPressedDown()
{
	setPressedDown( selection_->selection() );
};

void OptionSystem::setPressedDown( int option )
{
	setVisuallyPressedDown( option );
	selection_->setNotAllowed( option );
};

void OptionSystem::setAllNotPressedDown()
{
	for ( int i = 0; i < options_.size(); ++i )
	{
		options_[ i ].setToNormal();
	}
	selection_->setAllAllowed();
};

void OptionSystem::setVisuallyPressedDown( int option )
{
	for ( int i = 0; i < options_.size(); ++i )
	{
		if ( i == option )
		{
			options_[ i ].setToNullified();
		}
		else
		{
			options_[ i ].setToNormal();
		}
	}
};

void OptionSystem::setSelectedVisuallyPressedDown()
{
	setVisuallyPressedDown( selection_->selection() );
};