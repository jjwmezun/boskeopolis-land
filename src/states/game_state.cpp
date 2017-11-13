#include "game_state.hpp"
#include "render.hpp"

GameState::GameState
(
	StateID id,
	const Palette& palette
)
:
	id_ ( id ),
	palette_ ( palette )
{};

GameState::~GameState() {};

void GameState::render()
{
	stateRender();
};

void GameState::init()
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

GameState::StateID GameState::id() const
{
	return id_;
};

const Palette& GameState::palette() const
{
	return palette_;
};