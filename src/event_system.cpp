#include "audio.hpp"
#include <cstring>
#include "event_system.hpp"
#include "inventory.hpp"
#include "inventory_level.hpp"
#include "level.hpp"
#include "level_message_state.hpp"
#include "level_state.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "overworld_state.hpp"
#include "wmessage_state.hpp"

static constexpr int WATER_NULL = -1;
static constexpr int SEWER_START_W = 64;
static constexpr int SEWER_START_H = 64;
static constexpr int GROW_SIZE = 32;

EventSystem::SewerGFX::SewerGFX( int x, int y )
:
	dest_ ( x, y, SEWER_START_W, SEWER_START_H ),
	fade_in_ ( false )
{};

EventSystem::BossUI::BossUI()
:
	dest_ ( 8, Unit::WINDOW_HEIGHT_PIXELS - 16, Unit::WINDOW_WIDTH_PIXELS - 16, 8 ),
	src_ ( 0, 0, Unit::WINDOW_WIDTH_PIXELS - 16, 8 ),
	texture_ ( Render::createRenderBox( dest_.w, dest_.h ) )
{};

EventSystem::BossUI::~BossUI()
{
	SDL_DestroyTexture( texture_ );
}

void EventSystem::BossUI::render() const
{
	Render::renderRenderBox( texture_, src_, dest_ );
}

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
	in_front_of_door_ ( Door::NONE ),
	on_conveyor_belt_ ( false ),
	played_death_song_ ( false ),
	pause_state_movement_ ( false ),
	touching_ladder_ ( false ),
	touching_ladder_prev_ ( false ),
	can_climb_down_ ( false ),
	is_sliding_ ( false ),
	is_sliding_prev_ ( false ),
	pause_hero_ ( false ),
	misc_ (),
	level_complete_status_ ( LevelCompleteStatus::STILL_PLAYING )
{
	resetMisc();
};

void EventSystem::init( const Level& level )
{
	switch_ = level.startOn();
};

EventSystem::~EventSystem()
{
	resetMisc();
};

void EventSystem::reset()
{
	change_map_ = 0;
	message_ = false;
	message_lock_ = false;
	key_ = false;
	switch_ = false;
	disable_pause_ = false;
	move_water_ = -1;
	current_water_ = -1;
	in_front_of_door_ = Door::NONE;
	on_conveyor_belt_ = false;
	played_death_song_ = false;
	pause_state_movement_ = false;
	touching_ladder_ = false;
	touching_ladder_prev_ = false;
	can_climb_down_ = false;
	is_sliding_ = false;
	is_sliding_prev_ = false;
	pause_hero_ = false;
	level_complete_status_ = LevelCompleteStatus::STILL_PLAYING;
	resetPalette();
	resetMisc();
};

void EventSystem::resetMisc()
{/*
	if ( misc_.type_ == MiscType::SEWER_GFX && misc_.data_.sewer_gfx_ != nullptr )
	{
		delete misc_.data_.sewer_gfx_;
	}
	else if ( misc_.type_ == MiscType::BOSS_UI && misc_.data_.boss_ui_ != nullptr )
	{
		delete misc_.data_.boss_ui_;
	}
	memset( &misc_, 0, sizeof( EMisc ) ); // Just 0-out data.*/
	memset( &misc_, 0, sizeof( EMisc ) ); // Just 0-out data.
};

void EventSystem::win()
{
	level_complete_status_ = LevelCompleteStatus::WON;
};

void EventSystem::fail()
{
	level_complete_status_ = LevelCompleteStatus::FAILED;
};

void EventSystem::secretGoal()
{
	level_complete_status_ = LevelCompleteStatus::SECRET_GOAL;
};

void EventSystem::quitLevel()
{
	level_complete_status_ = LevelCompleteStatus::QUIT;
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

bool EventSystem::isSwitchOn() const
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

void EventSystem::updateTrainer( LevelState& level_state )
{
	switch_changed_ = false;
	if ( waterShouldStop() )
	{
		move_water_ = WATER_NULL;
	}
	testWarp( level_state );
};

void EventSystem::update( LevelState& level_state )
{
	updateTrainer( level_state );
	testMessage( level_state.level() );
	testLevelCompleteStatus( level_state.inventory() );
	resetOnConveyorBelt();
};

void EventSystem::resetIsSliding()
{
	is_sliding_prev_ = is_sliding_;
	is_sliding_ = false;
};

void EventSystem::resetTouchingLadder()
{
	touching_ladder_prev_ = touching_ladder_;
	touching_ladder_ = false;
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

void EventSystem::testWarp( LevelState& level_state )
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
				doWarp( level_state );
			}
		}
		break;

		case ( 2 ):
		{
			/*
			sprites.hero().graphics_->current_frame_x_ = 128;
			sprites.hero().graphics_->current_frame_y_ = 26;
			if ( testSewerAnimationFinished() )
			{
				level.sewerWarp( sprites, *this );
				change_map_ = 0;
				in_front_of_door_ = 0;
				sprites.hero().graphics_->priority_ = true;
				misc_.data_.sewer_gfx_->fade_in_ = true;
			}*/
		}
		break;
	}

	//doSewerAnimation( sprites );
};

void EventSystem::doWarp( LevelState& level_state )
{
	level_state.level().warp( level_state );
	change_map_ = 0;
	in_front_of_door_ = Door::NONE;
};

void EventSystem::testLevelCompleteStatus( const InventoryLevel& inventory )
{
	switch ( level_complete_status_ )
	{
		case ( LevelCompleteStatus::WON ):
		{
			winEvent( inventory );
		}
		break;
		case ( LevelCompleteStatus::FAILED ):
		{
			failEvent();
		}
		break;
		case ( LevelCompleteStatus::QUIT ):
		{
			quitEvent();
		}
		break;
		case ( LevelCompleteStatus::SECRET_GOAL ):
		{
			secretGoalEvent( inventory );
		}
		break;
	}
};

void EventSystem::failEvent()
{
	Inventory::fail();
	Main::pushState
	(
		std::make_unique<WMessageState>
		(
			WTextObj::MessageData{ Localization::getCurrentLanguage().getFailureMessage(), WTextCharacter::Color::WHITE, WTextCharacter::Color::DARK_GRAY },
			WMessageState::Type::CHANGE,
			Palette( "Stop Red", 2 ),
			std::make_unique<OverworldState> ( Inventory::currentLevel() ),
			"low",
			true,
			false
		),
		true
	);
	playDeathSoundIfNotAlreadyPlaying();
};

void EventSystem::winEvent( const InventoryLevel& inventory )
{
	const bool win_momento = Inventory::victory( Inventory::currentLevel() );
	Inventory::win( inventory );
	const ShowEventType new_event = ( !win_momento ) ? ShowEventType::NORMAL : ShowEventType::NONE;
	Main::pushState
	(
		std::make_unique<WMessageState>
		(
			WTextObj::MessageData{ Localization::getCurrentLanguage().getSuccessMessage(), WTextCharacter::Color::WHITE, WTextCharacter::Color::DARK_GRAY },
			WMessageState::Type::CHANGE,
			Palette( "Go Green", 2 ),
			std::make_unique<OverworldState> ( Inventory::currentLevel(), false, new_event ),
			"success",
			true,
			true
		),
		true
	);
};

void EventSystem::quitEvent()
{
	Inventory::quit();
	Main::changeState( std::make_unique<OverworldState> ( Inventory::currentLevel() ) );
};

void EventSystem::secretGoalEvent( const InventoryLevel& inventory )
{
	const bool win_momento = Inventory::getSecretGoal( Inventory::currentLevel() );
	Inventory::secretGoal( inventory );
	const ShowEventType new_event = ( !win_momento ) ? ShowEventType::SECRET : ShowEventType::NONE;
	Main::pushState
	(
		std::make_unique<WMessageState>
		(
			WTextObj::MessageData{ Localization::getCurrentLanguage().getSecretGoalMessage(), WTextCharacter::Color::WHITE, WTextCharacter::Color::DARK_GRAY },
			WMessageState::Type::CHANGE,
			Palette( "Triste Blue", 2 ),
			std::make_unique<OverworldState> ( Inventory::currentLevel(), false, new_event ),
			"success",
			true,
			true
		),
		true
	);
};

bool EventSystem::waterShouldMove() const
{
	return move_water_ != WATER_NULL;
};

bool EventSystem::waterShouldStop() const
{
	return move_water_ == current_water_;
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

bool EventSystem::onUpperLayer() const
{
	return misc_.data_.flag_ == MiscFlagType::UPPER_LAYER;
};

bool EventSystem::showBossUI() const
{
	return misc_.type_ == MiscType::BOSS_UI;
};

void EventSystem::renderBossUI() const
{
	misc_.data_.boss_ui_->render();
};

void EventSystem::createBossUI()
{
	if ( misc_.type_ != MiscType::BOSS_UI )
	{
		misc_.type_ = MiscType::BOSS_UI;
		misc_.data_.boss_ui_ = new BossUI();
	}
};

void EventSystem::changeBossUI( const TextObj& name, int health )
{
	Render::setRenderTarget( misc_.data_.boss_ui_->texture_ );
		Render::renderRect( misc_.data_.boss_ui_->src_, 1 );
		name.render();
		int left = ( name.words_.size() + 1 ) * 8;
		while ( health > 0 )
		{
			Render::renderRect( { left + 1, 1, 2, 6 }, 5 );
			left += 4;
			--health;
		}
	Render::releaseRenderTarget();
};

int EventSystem::waterMovement() const
{
	return move_water_;
};

void EventSystem::setWaterMovement( int value )
{
	move_water_ = value;
};

bool EventSystem::isWaterMovingUpward() const
{
	return move_water_ > current_water_;
};

bool EventSystem::isWaterMovingDownward() const
{
	return move_water_ < current_water_;
};

void EventSystem::setCurrentWater( int value )
{
	current_water_ = value;
};

bool EventSystem::pauseStateMovement() const
{
	return pause_state_movement_;
};

bool EventSystem::pauseIsDisabled() const
{
	return disable_pause_;
};

void EventSystem::setPauseDisabled()
{
	disable_pause_ = true;
};

void EventSystem::setTouchingLadder()
{
	touching_ladder_ = true;
};

bool EventSystem::testTouchingLadder() const
{
	return touching_ladder_;
};

void EventSystem::setInFrontOfDoor()
{
	in_front_of_door_ = Door::REGULAR;
};

void EventSystem::setInFrontOfSewerDoor()
{
	in_front_of_door_ = Door::SEWER;
};

void EventSystem::resetInFrontOfDoor()
{
	in_front_of_door_ = Door::NONE;
};

bool EventSystem::isInFrontOfRegularDoor() const
{
	return in_front_of_door_ == Door::REGULAR;
};

bool EventSystem::isInFrontOfSewerDoor() const
{
	return in_front_of_door_ == Door::SEWER;
};

void EventSystem::forceSwitch( bool value)
{
	switch_ = value;
};

void EventSystem::forceSwitchOn()
{
	switch_ = true;
};

void EventSystem::forceSwitchOff()
{
	switch_ = false;
};

void EventSystem::setCanClimbDown()
{
	can_climb_down_ = true;
};

void EventSystem::resetCanClimbDown()
{
	can_climb_down_ = false;
};

bool EventSystem::testCanClimbDown() const
{
	return can_climb_down_;
};

void EventSystem::setOnConveyorBelt()
{
	on_conveyor_belt_ = true;
};

void EventSystem::resetOnConveyorBelt()
{
	on_conveyor_belt_ = false;
};

bool EventSystem::testOnConveyorBelt() const
{
	return on_conveyor_belt_;
};

bool EventSystem::testIsSliding() const
{
	return is_sliding_;
};

void EventSystem::setSlidingOn()
{
	is_sliding_ = true;
};

bool EventSystem::testIsSlidingPreviously() const
{
	return is_sliding_prev_;
};

bool EventSystem::testChangeMap() const
{
	return change_map_;
};

bool EventSystem::testPauseHero() const
{
	return pause_hero_;
};

void EventSystem::setPauseHeroOn()
{
	pause_hero_ = true;
};

void EventSystem::setPauseHeroOff()
{
	pause_hero_ = false;
};
