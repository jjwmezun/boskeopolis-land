#include "game_state.h"
#include "graphics.h"

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

void GameState::render( Graphics& graphics )
{
	stateRender( graphics );
};

void GameState::changePalette( Graphics& graphics )
{
	graphics.newPalette( palette_ );
};

void GameState::newPalette( Graphics& graphics, const Palette& palette )
{
	if ( palette_ != palette )
	{
		palette_ = palette;
		changePalette( graphics );
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