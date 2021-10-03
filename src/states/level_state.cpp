#include "audio.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "main.hpp"
#include "level_list.hpp"
#include "level_state.hpp"
#include "overworld_state.hpp"
#include "pause_state.hpp"
#include "level_select_state.hpp"
#include "renderable.hpp"
#include "time_start_state.hpp"

LevelState::LevelState( int level_id, Difficulty difficulty, int heart_upgrades, bool has_oxygen_upgrade, int rand_seed, Camera camera )
:
	GameState ( StateID::LEVEL_STATE ),
	id_ ( 0 ),
	health_ ( difficulty, heart_upgrades, has_oxygen_upgrade ),
	camera_ ( camera ),
	events_ ( rand_seed ),
	sprites_ ( *this ),
	blocks_ (),
	level_ ( LevelList::getLevel( level_id ) ),
	inventory_screen_ ( difficulty, health_.maxHP(), has_oxygen_upgrade, level_.currentMap().palette_transition_.set, level_.currentMap().ui_bg_color_ ),
	palette_transition_ ( level_.currentMap().palette_transition_ )
{
	blocks_.init( level_.currentMap() );
}

LevelState::~LevelState() {};

void LevelState::stateUpdate()
{
	if ( palette_transition_.set && !Render::hasTransPalette() )
	{
		Render::turnOnTransitionPalette( palette_transition_.from, palette_transition_.to );
	}

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
					new TimeStartState( palette(), inventory_screen_.clock().getTimeString(), inventory_screen_.getGraphics().timeX() )
				)
			);
		}
	}

	testPause();

	if ( Render::hasTransPalette() )
	{
		const double relative = ( double )( std::min( palette_transition_.max, std::max( 0, Unit::SubPixelsToBlocks( sprites_.hero().hit_box_.x ) - palette_transition_.start ) ) );
		const double percent = relative / ( double )( palette_transition_.max );
		palette_transition_alpha_ = ( Uint8 )( std::max( 0.0, std::min( 255.0, percent * 255.0 ) ) );
		Render::setPaletteTransAlpha( palette_transition_alpha_ );
	}
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

void LevelState::renderLevel()
{
	level_.currentMap().renderBGColor();
	for ( int i = 0; i < ( int )( Unit::Layer::FG_2 ); ++i )
	{
		for ( int j = 0; j < layers_[ i ].size(); ++j )
		{
			layers_[ i ][ j ]->render( *this );
		}
	}
	inventory_screen_.renderBops( sprites_.hero(), camera_ );
	for ( int i = ( int )( Unit::Layer::FG_2 ); i < NUMBER_OF_LAYERS; ++i )
	{
		for ( int j = 0; j < layers_[ i ].size(); ++j )
		{
			layers_[ i ][ j ]->render( *this );
		}
	}
}

void LevelState::init()
{
	newPalette( level_.currentMap().palette_ );
	Inventory::levelStart( level_.id() );
	events_.init( level_ );
	sprites_.reset( *this );
	level_.init( *this );
	camera_.init( level_.cameraX(), level_.cameraY(), level_.currentMap().camera_type_ );
	inventory_screen_.init( level_ );
};

void LevelState::testPause()
{
	if ( Input::pressed( Input::Action::MENU ) && !events_.pauseIsDisabled() )
	{
		Main::pushState
		(
			std::unique_ptr<GameState>
			(
				new PauseState( palette(), events_, inventory_screen_, palette_transition_, palette_transition_alpha_ )
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

int LevelState::addRenderable( std::unique_ptr<Renderable>&& renderable, Unit::Layer layer )
{
	int layer_number = ( int )( layer );
	int id = id_;
	renderable->id_ = id;
	renderable_map_.insert( std::pair<int, Renderable*> ( id, renderable.get() ) );
	layers_[ layer_number ].push_back( std::move( renderable ) );
	++id_;
	return id;
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
					renderable_map_.erase( renderable_iterator );
					while ( j < layers_[ i ].size() - 1 )
					{
						layers_[ i ][ j ].reset( layers_[ i ][ j + 1 ].release() );
						++j;
					}
					layers_[ i ].pop_back();
					return;
				}
			}
		}
	}
};

void LevelState::changeRenderableLayer( int id, Unit::Layer layer )
{
	int layer_number = ( int )( layer );
	auto renderable_iterator = renderable_map_.find( id );
	if ( renderable_iterator != renderable_map_.end() )
	{
		for ( int i = 0; i < NUMBER_OF_LAYERS; ++i )
		{
			for ( int j = 0; j < layers_[ i ].size(); ++j )
			{
				if ( renderable_iterator->second == layers_[ i ][ j ].get() )
				{
					if ( layer_number != i )
					{
						layers_[ layer_number ].emplace_back( layers_[ i ][ j ].release() );
						layers_[ i ].erase( layers_[ i ].begin() + j );
					}
					return;
				}
			}
		}
	}
};

void LevelState::changePaletteTransition( const PaletteTransition& palette_transition )
{
	palette_transition_ = palette_transition;
};