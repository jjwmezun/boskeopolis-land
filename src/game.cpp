#include <cassert>
#include "game.h"
#include "message_state.h"
#include "mezun_helpers.h"
#include "title_state.h"
#include <SDL2/SDL.h>
#include <iostream>

int Game::frames_ = 0;
std::string Game::resource_path_ = "";
std::string Game::path_divider_ = "/";
bool Game::SAVING_ALLOWED = true;


Game::Game( std::vector<std::string>& args )
:
	input_ ( Input() )
{
	initSDL();

	if ( args.size() >= 4 )
	{
		if ( mezun::areStringsEqual( args.at( 3 ), "no-save" ) )
		{
			SAVING_ALLOWED = false;
		}
	}

	setResourcePath();
	graphics_ = std::make_unique<Graphics> ( *this, args );
	firstState();
};

void Game::destruct()
{
	graphics_->quit();
	SDL_Quit();
};

bool Game::running() const
{
	return running_;
};

void Game::execute()
{
	SDL_Event event;

	while ( running_ )
	{
		while ( SDL_PollEvent( &event ) != 0 )
		{
			if ( event.type == SDL_QUIT )
			{
				quit();
			}
			if ( event.type == SDL_KEYDOWN )
			{
				input_.keyPress( SDL_GetKeyFromScancode( event.key.keysym.scancode ) );
				input_.keyHold( SDL_GetKeyFromScancode( event.key.keysym.scancode ) );
			}
			if ( event.type == SDL_KEYUP )
			{
				input_.keyRelease( SDL_GetKeyFromScancode( event.key.keysym.scancode ) );
			}
		}

		if ( SDL_GetTicks() - ticks_ >= fpsMilliseconds() )
		{
			if ( input_.pressed( Input::Action::ESCAPE ) )
			{
				running_ = false;
			}

			if ( state_is_new_ )
			{
				changeStateSafe();
			}
			else if ( state_pop_ )
			{
				popStateSafe();
			}

			if ( state_change_ )
			{
				input_.reset();
				state_change_ = false;
			}

			states_.back()->update( *this, input_, *graphics_ );

			++frames_;
			ticks_ = SDL_GetTicks();

			input_.update();
			render();
		}
	}
};

void Game::render()
{
	graphics_->clearScreen();
	graphics_->colorCanvas();
	for ( auto& st : states_ )
	{
		st->render( *graphics_ );
	}
	graphics_->screenBorders();
	graphics_->presentScreen();
};

void Game::quit()
{
	running_ = false;
};

void Game::initSDL()
{
	if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
	{
		SDL_Log( "SDL_Initialization failed: %s", SDL_GetError() );
		quit();
	}
};

void Game::changeState( std::unique_ptr<GameState> state )
{
	new_state_.reset( state.release() );
	state_change_ = true;
	state_is_new_ = true;
};

void Game::changeStateSafe()
{
	assert( new_state_ );

	states_.clear();
	pushState( move( new_state_ ) );
	state_is_new_ = false;

	// This is a mo' important state change from the others, so we should clear out surfaces.
	graphics_->clearSurfaces();
};

void Game::pushState  ( std::unique_ptr<GameState> state )
{
	states_.push_back( move(state) );
	states_.back()->changePalette( *graphics_ );
	states_.back()->init( *this, *graphics_ );
	state_change_ = true;
};

void Game::popState()
{
	state_change_ = true;
	state_pop_    = true;
};

void Game::popStateSafe()
{
	assert ( !states_.empty() );

	states_.pop_back();

	assert ( !states_.empty() );

	states_.back()->changePalette( *graphics_ );

	state_pop_ = false;
};

void Game::tempMessage( std::string message, Palette&& palette, Text::FontShade text_color )
{
	pushState( std::unique_ptr<GameState> ( new MessageState( message, true, nullptr, false, palette, text_color ) ) );
};

int Game::fpsMilliseconds() const
{
	return floor( 1000 / Unit::FPS );
};

void Game::firstState()
{
	//Ensure state list is clear:
	states_.clear();

	pushState( std::unique_ptr<GameState> ( new TitleState() ) );
};

bool Game::nextFrame( int interval, int duration )
{
	for ( int i = 0; i < duration; ++i )
	{
		if ( frames_ % interval == i )
		{
			return true;
		}
	}

	return false;
};

int Game::frame()
{
	return frames_;
};

void Game::setResourcePath()
{
	#ifdef _WIN32
		path_divider_ = "\\";
	#endif

	const std::string BASE_DIR = "boskeopolis-land";
	const std::string RES_SUB_DIR = pathDivider() + "resources/";
	char* base_path = SDL_GetBasePath();
	std::string path = base_path;
	SDL_free( base_path );
	size_t pos = path.find( BASE_DIR );
	path.replace( pos + BASE_DIR.length(), path.length() - pos - BASE_DIR.length(), RES_SUB_DIR );
	resource_path_ = path;
	resource_path_ = "/home/jjwmezun/Documents/cpp-projects/boskeopolis-land/resources/";
};

std::string Game::resourcePath()
{
	return resource_path_;
};

std::string Game::pathDivider()
{
	return path_divider_;
};

bool Game::savingAllowed()
{
	return SAVING_ALLOWED;
};

std::string Game::savePath()
{
	return resourcePath() + "saves" + pathDivider() + "save.bin";
};