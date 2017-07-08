#include "event_system.h"
#include "game.h"
#include "inventory_level.h"
#include "level_message_state.h"
#include "level_select_state.h"
#include "message_state.h"
#include "mezun_helpers.h"

EventSystem::EventSystem()
:
	won_ ( false ),
	failed_ ( false ),
	quit_level_ ( false ),
	key_ ( false ),
	message_ ( false ),
	message_lock_ ( false ),
	change_map_ ( false ),
	switch_ ( false ),
	switch_changed_ ( false ),
	new_palette_ ( mezun::emptyString(), 0 ),
	palette_changed_ ( false ),
	move_water_ ( -1 )
{};

void EventSystem::reset()
{
	won_ = false;
	failed_ = false;
	quit_level_ = false;
	change_map_ = false;
	message_ = false;
	message_lock_ = false;
	key_ = false;
	switch_ = false;
	move_water_ = -1;

	resetPalette();
};

void EventSystem::win()
{
	won_ = true;
};

void EventSystem::fail()
{
	failed_ = true;
};

void EventSystem::quitLevel()
{
	quit_level_ = true;
};

void EventSystem::changeMap()
{
	change_map_ = true;
};

void EventSystem::getKey()
{
	key_ = true;
};

bool EventSystem::hasKey() const
{
	return key_;
};

void EventSystem::showMessage()
{
	message_ = true;
};

void EventSystem::changePalette( const Palette& new_palette )
{
	new_palette_ = new_palette;
	palette_changed_ = true;
};

Palette EventSystem::getPalette()
{
	const auto temp = new_palette_;
	resetPalette();
	return temp;
};

bool EventSystem::paletteChanged() const
{
	return palette_changed_;
};

void EventSystem::resetPalette()
{
	new_palette_ = { mezun::emptyString(), 0 };
	palette_changed_ = false;
};

bool EventSystem::switchOn() const
{
	return switch_;
};

bool EventSystem::switchChanged() const
{
	return switch_changed_;
};

void EventSystem::flipSwitch()
{
	switch_ = !switch_;
	switch_changed_ = true;
};

void EventSystem::update( Level& level, InventoryLevel& inventory, Game& game, SpriteSystem& sprites, Camera& camera )
{
	switch_changed_ = false;

	if ( waterShouldStop() )
	{
		move_water_ = WATER_NULL;
	}
	
	testMessage( level, game );
	testWarp( level, inventory, sprites, camera );
	testWinLoseOrQuit( level, inventory, game );
};

void EventSystem::testMessage( Level& level, Game& game )
{
	if ( message_lock_ && !message_ )
	{
		message_lock_ = false;
	}

	if ( message_ && !message_lock_ )
	{
		game.pushState( std::unique_ptr<GameState> ( new LevelMessageState( level.currentMap().palette(), level.message() ) ) );
		message_lock_ = true;
	}
	
	message_ = false;
};

void EventSystem::testWarp( Level& level, InventoryLevel& inventory, SpriteSystem& sprites, Camera& camera )
{
	if ( change_map_ )
	{
		level.warp( sprites, camera, inventory, *this );
		change_map_ = false;
	}
};

void EventSystem::testWinLoseOrQuit( Level& level, InventoryLevel& inventory, Game& game )
{
	if ( failed_ )
	{
		failEvent( level, inventory, game );
	}
	else if ( won_ )
	{
		winEvent( level, inventory, game );
	}
	else if ( quit_level_ )
	{
		quitEvent( level, inventory, game );
	}
};

void EventSystem::failEvent( Level& level, InventoryLevel& inventory, Game& game )
{
	inventory.failed();

	game.pushState
	(
		std::unique_ptr<GameState>
		(
			new MessageState
			(
				"¡Failure!",
				false,
				std::unique_ptr<GameState>
				(
					LevelSelectState::continueLevelSelect
					(
						*this,
						inventory,
						level.id()
					)
				),
				false,
				{ "Mountain Red", 2 },
				Text::FontShade::DARK_GRAY
			)
		)
	);
};

void EventSystem::winEvent( Level& level, InventoryLevel& inventory, Game& game )
{	
	inventory.won( level.id() );
	
	game.pushState
	(
		std::unique_ptr<GameState>
		(
			new MessageState
			(
				"¡Success!",
				false,
				std::unique_ptr<GameState>
				(
					LevelSelectState::continueLevelSelect
					(
						*this,
						inventory,
						level.id()
					)
				),
				false,
				{ "Classic Green", 2 },
				Text::FontShade::DARK_GRAY
			)
		)
	);
};

void EventSystem::quitEvent( Level& level, InventoryLevel& inventory, Game& game )
{	
	inventory.quit( level.id() );
	game.changeState( std::unique_ptr<GameState> ( LevelSelectState::continueLevelSelect( *this, inventory, level.id() ) ) );
};

bool EventSystem::waterShouldMove() const
{
	return move_water_ != WATER_NULL;
};

bool EventSystem::waterShouldStop() const
{
	return move_water_ == current_water_;
};