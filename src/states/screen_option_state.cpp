#include "audio.hpp"
#include "input.hpp"
#include "main.hpp"
#include "screen_option_state.hpp"
#include "render.hpp"
#include <iostream>

static constexpr int FULL_OR_WINDOW_OPTION_WIDTH = 12;
static constexpr int FULL_OR_WINDOW_OPTION_WIDTH_PIXELS = Unit::MiniBlocksToPixels( FULL_OR_WINDOW_OPTION_WIDTH );
static constexpr int START_Y = 64;

ScreenOptionState::ScreenOptionState()
:
	GameState( StateID::OPTIONS_STATE, { "Mountain Red", 2 }, false ),
	bg_ (),
	title_ ( "Screen Resolution", 0, 16, Text::FontColor::WHITE, Text::FontAlign::CENTER, Text::FontColor::BLACK ),
	fullscreen_option_ ( "Fullscreen", START_Y, FULL_OR_WINDOW_OPTION_WIDTH, ( Unit::WINDOW_WIDTH_PIXELS / 2 ) - FULL_OR_WINDOW_OPTION_WIDTH_PIXELS - 8 ),
	window_option_ ( "Windowed", START_Y, FULL_OR_WINDOW_OPTION_WIDTH, ( Unit::WINDOW_WIDTH_PIXELS / 2 ) + 8 ),
	selection_ ( Render::getMaxMagnification() ),
	other_options_ (),
	max_options_ ( Render::getMaxMagnification() + 1 )
{
	int y = START_Y + 32;
	for ( int i = 1; i < max_options_; ++i )
	{
		const int width = Unit::WINDOW_WIDTH_PIXELS * i;
		const int height = Unit::WINDOW_HEIGHT_PIXELS * i;
		const std::string text = std::to_string( width ) + "x" + std::to_string( height );
		other_options_.emplace_back( text, y, FULL_OR_WINDOW_OPTION_WIDTH * 2 + 2 );
		y += 32;
	}
	setNullifiedSizeOptions();
	setNullifiedFullOrWindowOption();
};

ScreenOptionState::~ScreenOptionState() {};

void ScreenOptionState::stateUpdate()
{
	bg_.update();
	selection_.update();
	updateOptions();
	updateInput();
};

void ScreenOptionState::stateRender()
{
	bg_.render();
	fullscreen_option_.render();
	window_option_.render();
	for ( const auto& other_option : other_options_ )
	{
		other_option.render();
	}
	title_.render();
};

void ScreenOptionState::init() {};

void ScreenOptionState::updateOptions()
{
	for ( int select_i = 0; select_i < max_options_; ++select_i )
	{
		if ( select_i >= 1 )
		{
			const int option_i = select_i - 1;
			if ( select_i == selection_.selection() )
			{
				other_options_[ option_i ].setToCurrent();
			}
			else if ( select_i == selection_.previousSelection() )
			{
				other_options_[ option_i ].setToPrevious();
			}
			other_options_[ option_i ].update();
		}
		else
		{
			if ( select_i == selection_.selection() )
			{
				if ( fullscreen_option_.isNullified() )
				{
					window_option_.setToCurrent();
				}
				else
				{
					fullscreen_option_.setToCurrent();
				}
			}
			else if ( select_i == selection_.previousSelection() )
			{
				if ( fullscreen_option_.isNullified() )
				{
					window_option_.setToPrevious();
				}
				else
				{
					fullscreen_option_.setToPrevious();
				}
			}
		}

		fullscreen_option_.update();
		window_option_.update();
	}
};

void ScreenOptionState::updateInput()
{
	if ( Input::pressed( Input::Action::CANCEL ) )
	{
		Main::popState();
	}
	else if ( Input::pressed( Input::Action::CONFIRM ) )
	{
		if ( selection_.selection() > 0 )
		{
			const int chosen_magnification = selection_.selection();
			Render::setMagnification( chosen_magnification );
			setNullifiedSizeOptions();
			selection_.setSelect( -1 );
			Audio::playSound( Audio::SoundType::CONFIRM );
		}
		else if ( selection_.selection() == 0 )
		{
			if ( fullscreen_option_.isNullified() )
			{
				Render::setWindowed();
				window_option_.setToNullified();
			}
			else
			{
				Render::setFullscreen();
				fullscreen_option_.setToNullified();
			}
			setNullifiedFullOrWindowOption();
			selection_.setSelect( -1 );
			Audio::playSound( Audio::SoundType::CONFIRM );
		}
	}
};

void ScreenOptionState::setNullifiedFullOrWindowOption()
{
	if ( Render::isFullscreen() )
	{
		fullscreen_option_.setToNullified();
		window_option_.setToNormal();
	}
	else
	{
		window_option_.setToNullified();
		fullscreen_option_.setToNormal();
	}
};

void ScreenOptionState::setNullifiedSizeOptions()
{
	for ( int i = 0; i < other_options_.size(); ++i )
	{
		if ( i == Render::getMagnification() - 1 )
		{
			other_options_[ i ].setToNullified();
		}
		else if ( i == selection_.previousSelection() )
		{
			other_options_[ i ].setToPrevious();
		}
		else
		{
			other_options_[ i ].setToNormal();
		}
	}
	selection_.setNotAllowed( Render::getMagnification() );
};
