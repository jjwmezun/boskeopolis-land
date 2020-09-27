#include "audio.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "main.hpp"
#include "level_state.hpp"
#include "overworld_state.hpp"
#include "pause_state.hpp"
#include "level_select_state.hpp"
#include "renderable.hpp"
#include "time_start_state.hpp"

LevelState::LevelState( int level_id, Difficulty difficulty, int heart_upgrades, bool has_oxygen_upgrade, Camera camera )
:
	GameState ( StateID::LEVEL_STATE ),
	id_ ( 0 ),
	health_ ( difficulty, heart_upgrades, has_oxygen_upgrade ),
	camera_ ( camera ),
	events_ (),
	sprites_ ( *this ),
	blocks_ (),
	level_ ( Level::getLevel( level_id ) ),
	inventory_screen_ ( difficulty, health_.maxHP(), has_oxygen_upgrade )
{
	blocks_.init( level_.currentMap() );
}

LevelState::~LevelState() {};

void LevelState::stateUpdate()
{
	blocks_.blocksFromMap( *this );
	blocks_.update( events_ );

	if ( camera_.testPause() )
	{
		camera_.scroll( level_.currentMap() );
	}
	else
	{
		if ( !events_.pauseStateMovement() )
		{
			level_.currentMap().update( *this );
			camera_.update();
			sprites_.update( *this );
			sprites_.interact( *this );
			sprites_.interactWithMap( *this );
			sprites_.spriteInteraction( *this );
			if ( !sprites_.hero().isDead() )
			{
				health_.update();
			}
		}
		inventory_screen_.update( events_, health_ );
		level_.updateGoal( *this );
		events_.update( *this );

		if ( events_.paletteChanged() )
		{
			newPalette( events_.getPalette() );
			inventory_screen_.forceRerender();
		}

		if ( events_.timerStart() )
		{
			Main::pushState
			(
				std::unique_ptr<GameState>
				(
					new TimeStartState( palette(), inventory_screen_.clock().getTimeString() )
				)
			);
		}
	}

	testPause();
};

void LevelState::updateForTrainer()
{
	blocks_.blocksFromMap( *this );
	blocks_.update( events_ );

	if ( camera_.testPause() )
	{
		camera_.scroll( level_.currentMap() );
	}
	else
	{
		level_.currentMap().update( *this );
		camera_.update();
		sprites_.update( *this );
		sprites_.interact( *this );
		sprites_.interactWithMap( *this );
		sprites_.spriteInteraction( *this );
		health_.update();
		events_.updateTrainer( *this );
	}
};

void LevelState::stateRender()
{
	renderLevel();
	inventory_screen_.render( events_, sprites_.hero(), camera_ );
};

void LevelState::renderLevel() const
{
	level_.currentMap().renderBGColor();
	for ( int i = 0; i < NUMBER_OF_LAYERS; ++i )
	{
		for ( int j = 0; j < layers_[ i ].size(); ++j )
		{
			layers_[ i ][ j ]->render( *this );
		}
	}
	/*
	// 1
	level_.currentMap().renderBG( camera_ ); // 2
	// 3
	// BG2 4
	// 5
	blocks_.render( level_.currentMap(), camera_, false ); // 6
	// 7
	sprites_.render( camera_, false ); // 8
	// 9
	blocks_.render( level_.currentMap(), camera_, true ); // 10
	// 11
	sprites_.render( camera_, true ); // 12
	// 13
	level_.currentMap().renderFG( camera_ ); // 14
	// 15
	// 16 — FG2
	// 17
	sprites_.renderSuperPriority( camera_ ); // 18*/
}

void LevelState::init()
{
	newPalette( level_.currentMap().palette_ );
	Inventory::levelStart( level_.id() );
	events_.init( level_ );
	sprites_.reset( *this );
	level_.init( *this );
	camera_.setPosition( level_.cameraX(), level_.cameraY() );
	inventory_screen_.init( level_.currentMap() );
};

void LevelState::testPause()
{
	if ( Input::pressed( Input::Action::MENU ) && !events_.pauseIsDisabled() )
	{
		Main::pushState
		(
			std::unique_ptr<GameState>
			(
				new PauseState( palette(), events_, inventory_screen_ )
			)
		);
	}
};

void LevelState::backFromPop()
{
	if ( events_.timerStart() )
	{
		events_.unsetFlag();
		inventory_screen_.forceRerender();
	}
	Audio::changeSong( level_.currentMap().music_ );
};

Health& LevelState::health()
{
	return health_;
};

const Health& LevelState::health() const
{
	return health_;
};

Camera& LevelState::camera()
{
	return camera_;
};

const Camera& LevelState::camera() const
{
	return camera_;
};

SpriteSystem& LevelState::sprites()
{
	return sprites_;
};

const SpriteSystem& LevelState::sprites() const
{
	return sprites_;
};

BlockSystem& LevelState::blocks()
{
	return blocks_;
};

const BlockSystem& LevelState::blocks() const
{
	return blocks_;
};

InventoryLevel& LevelState::inventory()
{
	return inventory_screen_;
};

const InventoryLevel& LevelState::inventory() const
{
	return inventory_screen_;
};

Map& LevelState::currentMap()
{
	return level_.currentMap();
};

const Map& LevelState::currentMap() const
{
	return level_.currentMap();
};

Level& LevelState::level()
{
	return level_;
};

const Level& LevelState::level() const
{
	return level_;
};

EventSystem& LevelState::events()
{
	return events_;
};

const EventSystem& LevelState::events() const
{
	return events_;
};

void LevelState::initForTrainer()
{
	sprites_.resetTrainer( *this );
	camera_.setPosition( level_.cameraX(), level_.cameraY() );
	level_.initCurrentMap( *this );
};

bool LevelState::trainerPaletteChanged() const
{
	return events_.paletteChanged();
};

Palette LevelState::getNewPalette()
{
	return events_.getPalette();
};

void LevelState::reRenderInventory()
{
	inventory_screen_.forceRerender();
};

int LevelState::addRenderable( std::unique_ptr<Renderable>&& renderable, int layer )
{
	if ( layer >= 0  && layer < NUMBER_OF_LAYERS )
	{
		renderable->id_ = id_++;
		renderable_map_.insert( std::pair<int, Renderable*> ( renderable->id_, renderable.get() ) );
		layers_[ layer ].push_back( std::move( renderable ) );
		return id_ - 1;
	}
	return -1;
};

void LevelState::removeRenderable( int id )
{
	auto renderable_iterator = renderable_map_.find( id );
	if ( renderable_iterator != renderable_map_.end() )
	{
		for ( int i = 0; i < NUMBER_OF_LAYERS; ++i )
		{
			for ( int j = 0; j < layers_[ i ].size(); ++j )
			{
				if ( renderable_iterator->second == layers_[ i ][ j ].get() )
				{
					layers_[ i ].erase( layers_[ i ].begin() + j );
					renderable_map_.erase( renderable_iterator );
					return;
				}
			}
		}
	}
};

void LevelState::changeRenderableLayer( int id, int layer )
{
	auto renderable_iterator = renderable_map_.find( id );
	if ( renderable_iterator != renderable_map_.end() )
	{
		for ( int i = 0; i < NUMBER_OF_LAYERS; ++i )
		{
			for ( int j = 0; j < layers_[ i ].size(); ++j )
			{
				if ( renderable_iterator->second == layers_[ i ][ j ].get() )
				{
					if ( layer != i )
					{
						layers_[ layer ].emplace_back( layers_[ i ][ j ].release() );
						layers_[ i ].erase( layers_[ i ].begin() + j );
					}
					return;
				}
			}
		}
	}
};