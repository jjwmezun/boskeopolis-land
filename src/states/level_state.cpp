#include "game.h"
#include "level_state.h"
#include "message_state.h"
#include "mezun_exceptions.h"
#include "overworld_state.h"
#include "pause_state.h"
#include "level_select_state.h"

LevelState::LevelState( EventSystem events, InventoryLevel inventory, int lvname, Game& game )
try :
	GameState ( StateID::LEVEL_STATE ),
	inventory_ ( inventory ),
	events_ ( events ),
	level_ ( Level::getLevel( lvname ) ),
	camera_ ( { level_.cameraX(), level_.cameraY() } ),
	sprites_ ( level_.entranceX(), level_.entranceY() ),
	blocks_ ( level_.currentMap() )
{
	inventory_.registerBeenToLevel( lvname );
}
catch ( const mezun::CantLoadTileset& e )
{
	game.pushState
	(
		std::move( MessageState::error
		(
			e.what(),
			false,
			std::make_unique<OverworldState> ( events_, inventory_.inventory(), level_.id() ),
			false
		) )
	);
};

LevelState::~LevelState() {};

void LevelState::update( Game& game, const Input& input, Graphics& graphics )
{
	try
	{
		
		blocks_.blocksFromMap( level_.currentMap(), camera_ );
		blocks_.update( events_ );
		camera_.update();
		level_.update( events_, sprites_, inventory_, input );
		sprites_.update( input, camera_, level_.currentMap(), game, events_, blocks_ );
		sprites_.interact( blocks_, level_, events_, inventory_, camera_ );
		sprites_.interactWithMap( level_.currentMap(), camera_ );
		sprites_.spriteInteraction( camera_, blocks_, level_.currentMap() );
		inventory_.update( events_, sprites_.hero() );
		events_.update( level_, inventory_, game, sprites_, camera_ );

		if ( events_.paletteChanged() )
		{
			newPalette( graphics, events_.getPalette() );
		}

		testPause( game, input );
		
	}
	catch ( const mezun::InvalidBlockType& e )
	{
		game.pushState
		(
			std::move( MessageState::error
			(
				e.what(),
				false,
				std::make_unique<OverworldState> ( events_, inventory_.inventory(), level_.id() ),
				false
			) )
		);
	}
};

void LevelState::stateRender( Graphics& graphics )
{
	level_.currentMap().renderBG( graphics, camera_ );
	blocks_.render( graphics, camera_, false );
	sprites_.render( graphics, camera_, false );
	blocks_.render( graphics, camera_, true );
	sprites_.render( graphics, camera_, true );
	level_.currentMap().renderFG( graphics, camera_ );
	inventory_.render( graphics, level_.id(), events_ );
};

void LevelState::init( Game& game, Graphics& graphics )
{
	newPalette( graphics, level_.currentMap().palette() );

	try
	{
		sprites_.reset( level_, inventory_ );
	}
	catch ( const mezun::InvalidSprite& e )
	{
		game.pushState
		(
			std::move( MessageState::error
			(
				e.what(),
				false,
				std::make_unique<OverworldState> ( events_, inventory_.inventory(), level_.id() ),
				false
			) )
		);
	}

	inventory_.init();
	level_.init( sprites_.hero(), inventory_, events_, game );
	events_.reset();
	camera_.setPosition( level_.cameraX(), level_.cameraY() );
};

void LevelState::testPause( Game& game, const Input& input )
{
	if ( input.pressed( Input::Action::MENU ) )
	{
		game.pushState
		(
			std::unique_ptr<GameState>
			(
				new PauseState( palette(), events_, inventory_.inventory().victory( level_.id() ) )
			)
		);
	}
};