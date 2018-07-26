#include "audio.hpp"
#include <cstring>
#include "event_system.hpp"
#include "inventory.hpp"
#include "level.hpp"
#include "level_message_state.hpp"
#include "main.hpp"
#include "message_state.hpp"
#include "mezun_helpers.hpp"
#include "overworld_state.hpp"

static constexpr int WATER_NULL = -1;
static constexpr int SEWER_START_W = 64;
static constexpr int SEWER_START_H = 64;
static constexpr int GROW_SIZE = 32;

EventSystem::SewerGFX::SewerGFX( int x, int y )
:
	dest_ ( x, y, SEWER_START_W, SEWER_START_H ),
	fade_in_ ( false )
{};

EventSystem::MiscData::MiscData()
:
	rand_treasure_ ( { 0, 9 } )
{};

EventSystem::EMisc::EMisc()
:
	type_ ( MiscType::__NULL ),
	data_ ()
{};

EventSystem::EventSystem()
:
	won_ ( false ),
	failed_ ( false ),
	quit_level_ ( false ),
	key_ ( false ),
	message_ ( false ),
	message_lock_ ( false ),
	change_map_ ( 0 ),
	switch_ ( false ),
	switch_changed_ ( false ),
	palette_changed_ ( false ),
	new_palette_ ( mezun::emptyString(), 0 ),
	disable_pause_ ( false ),
	move_water_ ( WATER_NULL ),
	current_water_ ( WATER_NULL ),
	in_front_of_door_ ( 0 ),
	on_conveyor_belt_ ( false ),
	played_death_song_ ( false ),
	pause_state_movement_ ( false ),
	misc_ ()
{
	resetMisc();
};

void EventSystem::reset()
{
	won_ = false;
	failed_ = false;
	quit_level_ = false;
	change_map_ = 0;
	message_ = false;
	message_lock_ = false;
	key_ = false;
	switch_ = false;
	disable_pause_ = false;
	move_water_ = -1;
	current_water_ = -1;
	in_front_of_door_ = 0;
	on_conveyor_belt_ = false;
	played_death_song_ = false;
	pause_state_movement_ = false;
	resetPalette();
	resetMisc();
};

void EventSystem::resetMisc()
{
	if ( misc_.type_ == MiscType::SEWER_GFX && misc_.data_.sewer_gfx_ != nullptr )
	{
		delete misc_.data_.sewer_gfx_;
	}
	memset( &misc_, 0, sizeof( EMisc ) );
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
	change_map_ = 1;
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
		Main::pushState( std::unique_ptr<GameState> ( new LevelMessageState( level.currentMap().palette_, level.message() ) ) );
		message_lock_ = true;
	}

	message_ = false;
};

void EventSystem::testWarp( Level& level, SpriteSystem& sprites, Camera& camera, BlockSystem& blocks )
{
	switch ( change_map_ )
	{
		case ( 1 ):
		{
			if ( Main::transitionState() == Main::TransitionState::__NULL )
			{
				Main::startFadeOut();
			}
			else if ( Main::transitionState() == Main::TransitionState::FADE_IN )
			{
				doWarp( level, sprites, camera, blocks );
			}
		}
		break;

		case ( 2 ):
		{
			sprites.hero().graphics_->current_frame_x_ = 128;
			sprites.hero().graphics_->current_frame_y_ = 26;
			if ( testSewerAnimationFinished() )
			{
				level.sewerWarp( sprites, *this );
				change_map_ = 0;
				in_front_of_door_ = 0;
				sprites.hero().graphics_->priority_ = true;
				misc_.data_.sewer_gfx_->fade_in_ = true;
			}
		}
		break;
	}

	doSewerAnimation( sprites );
};

void EventSystem::doWarp( Level& level, SpriteSystem& sprites, Camera& camera, BlockSystem& blocks )
{
	level.warp( sprites, camera, *this, blocks );
	change_map_ = 0;
	in_front_of_door_ = 0;
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
		std::make_unique<MessageState> ( "Failure...", MessageState::Type::CHANGE, Palette( "Stop Red", 2 ), std::make_unique<OverworldState> (), Text::FontColor::WHITE, Text::FontColor::DARK_GRAY, "low", true, false ),
		true
	);
	playDeathSoundIfNotAlreadyPlaying();
};

void EventSystem::winEvent( Level& level )
{
	Inventory::win();
	Main::pushState
	(
		std::make_unique<MessageState> ( "¡Success!", MessageState::Type::CHANGE, Palette( "Go Green", 2 ), std::make_unique<OverworldState> (), Text::FontColor::WHITE, Text::FontColor::DARK_GRAY, "success", true, true ),
		true
	);
};

void EventSystem::quitEvent( Level& level )
{
	Inventory::quit();
	Main::changeState( std::make_unique<OverworldState> () );
};

bool EventSystem::waterShouldMove() const
{
	return move_water_ != WATER_NULL;
};

bool EventSystem::waterShouldStop() const
{
	return move_water_ == current_water_;
};

void EventSystem::lightSwitchTurnOn()
{
	switch_ = true;
};

bool EventSystem::testLightSwitch()
{
	bool temp = switch_;
	switch_ = false;
	return temp;
};

void EventSystem::playDeathSoundIfNotAlreadyPlaying()
{
	if ( !played_death_song_ )
	{
		Audio::playSound( Audio::SoundType::DEATH );
		played_death_song_ = true;
	}
};

void EventSystem::changeMapSewer()
{
	change_map_ = 2;
	turnOnSewerAnimation();
};

void EventSystem::setFlag( MiscFlagType type )
{
	misc_.type_ = MiscType::FLAG;
	misc_.data_.flag_ = type;
};

void EventSystem::unsetFlag()
{
	assertFlag();
	misc_.data_.flag_ = MiscFlagType::__NULL;
};

void EventSystem::assertFlag() const
{
	assert( misc_.type_ == MiscType::FLAG );
};

bool EventSystem::testIsFlag() const
{
	return misc_.type_ == MiscType::FLAG;
};

bool EventSystem::testFlag( MiscFlagType type ) const
{
	return testIsFlag() &&
		misc_.data_.flag_ == type;
};

void EventSystem::startTimer()
{
	setFlag( MiscFlagType::TIMER_START );
};

bool EventSystem::timerStart() const
{
	return testFlag( MiscFlagType::TIMER_START );
};

bool EventSystem::trainDoorPartlyOpen() const
{
	return
		testIsFlag() &&
		(
			misc_.data_.flag_ == MiscFlagType::LOCKED_GATE_OPENING ||
			misc_.data_.flag_ == MiscFlagType::LOCKED_GATE_OPEN
		);
};

void EventSystem::setOpening()
{
	setFlag( MiscFlagType::LOCKED_GATE_OPENING );
}

void EventSystem::setOpen()
{
	setFlag( MiscFlagType::LOCKED_GATE_OPEN );
};

int EventSystem::addRandomTreasure()
{
	misc_.type_ = MiscType::RAND_TREASURE;
	++misc_.data_.rand_treasure_.count_;
	return misc_.data_.rand_treasure_.count_;
};

void EventSystem::setRandomTreasureIfNotAlreadySet()
{
	assert( misc_.type_ == MiscType::RAND_TREASURE );
	if ( misc_.data_.rand_treasure_.keycane_chest_ == 0 )
	{
		misc_.data_.rand_treasure_.keycane_chest_ = mezun::randInt( misc_.data_.rand_treasure_.count_, 1 );
	}
}

bool EventSystem::testRandomTreasure( int id )
{
	setRandomTreasureIfNotAlreadySet();
	return misc_.data_.rand_treasure_.keycane_chest_ == id;
};

bool EventSystem::testSewerAnimationFinished() const
{
	assert( misc_.type_ == MiscType::SEWER_GFX );
	return misc_.data_.sewer_gfx_->dest_.w >= Unit::WINDOW_WIDTH_PIXELS*2;
}

bool EventSystem::ensureSewerPointerIsSet( int x, int y )
{
	if ( misc_.data_.sewer_gfx_ == nullptr )
	{
		misc_.data_.sewer_gfx_ = new SewerGFX( x, y );
		return true;
	}
	return false;
}

void EventSystem::turnOnSewerAnimation()
{
	misc_.type_ = MiscType::SEWER_GFX;
	//ensureSewerPointerIsSet();
	pause_state_movement_ = true;
};

void EventSystem::doSewerAnimation( SpriteSystem& sprites )
{
	if ( misc_.type_ == MiscType::SEWER_GFX )
	{
		if ( !misc_.data_.sewer_gfx_->fade_in_ )
		{
			misc_.data_.sewer_gfx_->dest_.h += GROW_SIZE;
			misc_.data_.sewer_gfx_->dest_.w += GROW_SIZE;
			misc_.data_.sewer_gfx_->dest_.x -= GROW_SIZE/2;
			misc_.data_.sewer_gfx_->dest_.y -= GROW_SIZE/2;
		}
		else
		{
			misc_.data_.sewer_gfx_->dest_.h -= GROW_SIZE;
			misc_.data_.sewer_gfx_->dest_.w -= GROW_SIZE;
			misc_.data_.sewer_gfx_->dest_.x += GROW_SIZE/2;
			misc_.data_.sewer_gfx_->dest_.y += GROW_SIZE/2;

			if ( misc_.data_.sewer_gfx_->dest_.w <= 0 )
			{
				pause_state_movement_ = false;
				misc_.type_ = MiscType::__NULL;
				misc_.data_.sewer_gfx_->fade_in_ = false;
				if ( misc_.data_.sewer_gfx_ != nullptr )
				{
					misc_.data_.sewer_gfx_->dest_.w = SEWER_START_W;
					misc_.data_.sewer_gfx_->dest_.h = SEWER_START_H;
				}
				sprites.hero().graphics_->priority_ = false;
			}
		}
	}
};

void EventSystem::renderSewer( const Camera& camera )
{
	if ( misc_.type_ == MiscType::SEWER_GFX )
	{
		auto dest = misc_.data_.sewer_gfx_->dest_;
		Render::renderObject( "sprites/sewer-hole.png", { 0, 0, 64, 64 }, dest, SDL_FLIP_NONE, 0, 255, &camera );
	}
}

void EventSystem::setSewerPosition( int x, int y )
{
	x = Unit::SubPixelsToPixels( x );
	y = Unit::SubPixelsToPixels( y );
	if ( !ensureSewerPointerIsSet( x, y ) )
	{
		misc_.data_.sewer_gfx_->dest_.x = x;
		misc_.data_.sewer_gfx_->dest_.y = y;
	}
};
