#include "game_state.hpp"
#include "mezun_math.hpp"
#include "render.hpp"

GameState::GameState
(
	StateID id,
	const Palette& palette,
	bool render_below
)
:
	id_ ( id ),
	palette_ ( palette ),
	frame_counter_ ( 0 ),
	render_below_ ( render_below )
{};

GameState::~GameState() {};

void GameState::update()
{
	stateUpdate();
	++frame_counter_;
};

void GameState::render()
{
	stateRender();
};

void GameState::init()
{
	// Do nothing if not overrided by children.
};

void GameState::backFromPop()
{
	// Do nothing if not overrided by children.
};

void GameState::changePalette()
{
	Render::newPalette( palette_ );
};

void GameState::newPalette( const Palette& palette )
{
	if ( palette_ != palette )
	{
		palette_ = palette;
		changePalette();
	}
};

void GameState::newPalette( const char* name )
{
	std::string type = std::string( name );
	if ( palette_.type() != type )
	{
		palette_ = { type, palette_.bgN() };
		changePalette();
	}
};

GameState::StateID GameState::id() const
{
	return id_;
};

const Palette& GameState::palette() const
{
	return palette_;
};

bool GameState::nextFrame( int interval, int duration ) const
{
	return mezun::nextFrame( frame_counter_, interval, duration );
};

int GameState::frame() const
{
	return frame_counter_;
};

bool GameState::testRenderBelow() const
{
	return render_below_;
};
