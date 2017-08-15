#include "event_system.hpp"
#include "inventory.hpp"
#include "level.hpp"
#include "level_message_state.hpp"
#include "main.hpp"
#include "message_state.hpp"
#include "mezun_helpers.hpp"
#include "overworld_state.hpp"

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
	palette_changed_ ( false ),
	new_palette_ ( mezun::emptyString(), 0 ),
	move_water_ ( -1 ),
	current_water_ ( - 1 ),
	in_front_of_door_ ( false ),
	misc_ ( 0 )
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
	current_water_ = -1;
	in_front_of_door_ = false;

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

void EventSystem::update( Level& level, SpriteSystem& sprites, Camera& camera, BlockSystem& blocks )
{
	switch_changed_ = false;

	if ( waterShouldStop() )
	{
		move_water_ = WATER_NULL;
	}
	
	testMessage( level );
	testWarp( level, sprites, camera, blocks );
	testWinLoseOrQuit( level );
};

void EventSystem::testMessage( Level& level )
{
	if ( message_lock_ && !message_ )
	{
		message_lock_ = false;
	}

	if ( message_ && !message_lock_ )
	{
		Main::pushState( std::unique_ptr<GameState> ( new LevelMessageState( level.currentMap().palette(), level.message() ) ) );
		message_lock_ = true;
	}
	
	message_ = false;
};

void EventSystem::testWarp( Level& level, SpriteSystem& sprites, Camera& camera, BlockSystem& blocks )
{
	if ( change_map_ )
	{
		level.warp( sprites, camera, *this, blocks );
		change_map_ = false;
		in_front_of_door_ = false;
	}
};

void EventSystem::testWinLoseOrQuit( Level& level )
{
	if ( failed_ )
	{
		failEvent( level );
	}
	else if ( won_ )
	{
		winEvent( level );
	}
	else if ( quit_level_ )
	{
		quitEvent( level );
	}
};

void EventSystem::failEvent( Level& level )
{
	Inventory::fail();

	Main::pushState
	(
		std::unique_ptr<GameState>
		(
			new MessageState
			(
				"¡Failure!",
				false,
				std::unique_ptr<GameState> ( new OverworldState() ),
				false,
				{ "Mountain Red", 2 },
				Text::FontShade::DARK_GRAY
			)
		)
	);
};

void EventSystem::winEvent( Level& level )
{	
	Inventory::win();
	
	Main::pushState
	(
		std::unique_ptr<GameState>
		(
			new MessageState
			(
				"¡Success!",
				false,
				std::unique_ptr<GameState> ( new OverworldState() ),
				false,
				{ "Classic Green", 2 },
				Text::FontShade::DARK_GRAY
			)
		)
	);
};

void EventSystem::quitEvent( Level& level )
{	
	Inventory::quit();
	Main::changeState( std::unique_ptr<GameState> ( new OverworldState() ) );
};

bool EventSystem::waterShouldMove() const
{
	return move_water_ != WATER_NULL;
};

bool EventSystem::waterShouldStop() const
{
	return move_water_ == current_water_;
};