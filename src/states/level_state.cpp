#include "audio.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "main.hpp"
#include "level_state.hpp"
#include "message_state.hpp"
#include "overworld_state.hpp"
#include "pause_state.hpp"
#include "level_select_state.hpp"
#include "time_start_state.hpp"

LevelState::LevelState( int level_id )
:
	GameState ( StateID::LEVEL_STATE ),
	inventory_screen_ (),
	level_ ( Level::getLevel( level_id ) ),
	events_ ( level_.startOn() ),
	camera_ ( level_.cameraX(), level_.cameraY(), Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS - 32, 0, 0, Unit::WINDOW_WIDTH_PIXELS, Unit::WINDOW_HEIGHT_PIXELS ),
	sprites_ ( level_.entranceX(), level_.entranceY() ),
	blocks_ ( level_.currentMap() ),
	health_ ()
{
	Inventory::levelStart( level_id );
}

LevelState::~LevelState()
{
};

void LevelState::stateUpdate()
{
	blocks_.blocksFromMap( level_.currentMap(), camera_ );
	blocks_.update( events_ );

	if ( camera_.testPause() )
	{
		camera_.scroll( level_.currentMap() );
	}
	else
	{
		if ( !events_.pause_state_movement_ )
		{
			level_.currentMap().update( events_, sprites_, blocks_, camera_ );
			camera_.update();
			sprites_.update( camera_, level_.currentMap(), events_, blocks_, health_ );
			sprites_.interact( blocks_, level_, events_, camera_, health_ );
			sprites_.interactWithMap( level_.currentMap(), camera_, health_ );
			sprites_.spriteInteraction( camera_, blocks_, level_.currentMap(), health_, events_ );
			health_.update();
		}
		inventory_screen_.update( events_, health_ );
		level_.updateGoal( inventory_screen_, events_, sprites_, blocks_, camera_, health_, *this );
		events_.update( level_, sprites_, camera_, blocks_ );

		if ( events_.paletteChanged() )
		{
			newPalette( events_.getPalette() );
		}

		if ( events_.timerStart() )
		{
			events_.unsetFlag();
			Main::pushState
			(
				std::unique_ptr<GameState>
				(
					new TimeStartState( palette() )
				)
			);
		}
	}

	testPause();
};

void LevelState::stateRender()
{
	level_.currentMap().renderBG( camera_ );
	blocks_.render( level_.currentMap(), camera_, false );
	sprites_.render( camera_, false );
	blocks_.render( level_.currentMap(), camera_, true );
	sprites_.render( camera_, true );
	level_.currentMap().renderFG( camera_ );
	events_.renderSewer( camera_ );
	inventory_screen_.render( events_, sprites_.hero(), camera_, level_.currentMap() );
};

void LevelState::init()
{
	newPalette( level_.currentMap().palette_ );
	sprites_.reset( level_, events_ );
	level_.init( sprites_.hero(), inventory_screen_, events_, health_ );
	camera_.setPosition( level_.cameraX(), level_.cameraY() );
	inventory_screen_.init();
};

void LevelState::testPause()
{
	if ( Input::pressed( Input::Action::MENU ) && !events_.disable_pause_ )
	{
		Main::pushState
		(
			std::unique_ptr<GameState>
			(
				new PauseState( palette(), events_ )
			)
		);
	}
};

void LevelState::backFromPop()
{
	Audio::changeSong( level_.currentMap().music_ );
};
