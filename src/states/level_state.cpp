#include "input.hpp"
#include "main.hpp"
#include "level_state.hpp"
#include "message_state.hpp"
#include "mezun_exceptions.hpp"
#include "overworld_state.hpp"
#include "pause_state.hpp"
#include "level_select_state.hpp"

LevelState::LevelState( const Inventory& inventory, int lvname )
try :
	GameState ( StateID::LEVEL_STATE ),
	inventory_ ( inventory ),
	events_ (),
	level_ ( Level::getLevel( lvname ) ),
	camera_ ( { level_.cameraX(), level_.cameraY() } ),
	sprites_ ( level_.entranceX(), level_.entranceY() ),
	blocks_ ( level_.currentMap() )
{
	inventory_.registerBeenToLevel( lvname );
}
catch ( const mezun::CantLoadTileset& e )
{
	Main::pushState
	(
		std::move( MessageState::error
		(
			e.what(),
			false,
			std::make_unique<OverworldState> ( inventory_.inventory(), level_.id() ),
			false
		) )
	);
};

LevelState::~LevelState() {};

void LevelState::update()
{
	try
	{
		
		blocks_.blocksFromMap( level_.currentMap(), camera_ );
		blocks_.update( events_ );
		level_.update( events_, sprites_, inventory_, blocks_, camera_ );
		camera_.update();
		sprites_.update( camera_, level_.currentMap(), events_, blocks_ );
		sprites_.interact( blocks_, level_, events_, inventory_, camera_ );
		sprites_.interactWithMap( level_.currentMap(), camera_ );
		sprites_.spriteInteraction( camera_, blocks_, level_.currentMap() );
		inventory_.update( events_, sprites_.hero() );
		events_.update( level_, inventory_, sprites_, camera_, blocks_ );

		if ( events_.paletteChanged() )
		{
			newPalette( events_.getPalette() );
		}

		testPause();
	}
	catch ( const mezun::InvalidBlockType& e )
	{
		Main::pushState
		(
			std::move( MessageState::error
			(
				e.what(),
				false,
				std::make_unique<OverworldState> ( inventory_.inventory(), level_.id() ),
				false
			) )
		);
	}
};

void LevelState::stateRender()
{
	level_.currentMap().renderBG( camera_ );
	blocks_.render( camera_, false );
	sprites_.render( camera_, false );
	blocks_.render( camera_, true );
	sprites_.render( camera_, true );
	level_.currentMap().renderFG( camera_ );
	inventory_.render( level_.id(), events_ );
};

void LevelState::init()
{
	newPalette( level_.currentMap().palette() );

	try
	{
		sprites_.reset( level_, inventory_ );
	}
	catch ( const mezun::InvalidSprite& e )
	{
		Main::pushState
		(
			std::move( MessageState::error
			(
				e.what(),
				false,
				std::make_unique<OverworldState> ( inventory_.inventory(), level_.id() ),
				false
			) )
		);
	}

	inventory_.init();
	level_.init( sprites_.hero(), inventory_, events_ );
	camera_.setPosition( level_.cameraX(), level_.cameraY() );
};

void LevelState::testPause()
{
	if ( Input::pressed( Input::Action::MENU ) )
	{
		Main::pushState
		(
			std::unique_ptr<GameState>
			(
				new PauseState( palette(), events_, inventory_.inventory().victory( level_.id() ) )
			)
		);
	}
};