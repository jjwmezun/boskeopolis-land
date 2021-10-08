#include "audio.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "level.hpp"
#include "level_list.hpp"
#include "localization.hpp"
#include "localization_language.hpp"
#include "mezun_helpers.hpp"
#include "ow_inventory.hpp"
#include "render.hpp"

static constexpr sdl2::SDLRect AREA = { 0, Unit::WINDOW_HEIGHT_PIXELS - OWInventory::HEIGHT, Unit::WINDOW_WIDTH_PIXELS, OWInventory::HEIGHT };
static constexpr int ROW_1 = AREA.y + 8;
static constexpr int ROW_2 = AREA.y + 16;
static constexpr int ROW_3 = AREA.y + 24;
static constexpr int LEFT_EDGE = AREA.x + 8;
static constexpr int RIGHT_EDGE = AREA.w - 8;
static constexpr int LEVEL_NAME_X = LEFT_EDGE + 8;
static constexpr int FLASH_SPEED = 8;
static constexpr int HAVE_X = 48;
static constexpr int DONT_HAVE_X = 56;
static constexpr int SCORE_X = Unit::MiniBlocksToPixels( Unit::WINDOW_WIDTH_MINIBLOCKS - 6 );


OWInventory::OWInventory()
:
	show_challenges_ ( false ),
	show_challenges_lock_ ( true ),
	sound_lock_ ( false ),
	color_animation_ ( 0 ),
	color_animation_timer_ ( 0 ),
	previous_space_ (),
	space_ (),
	current_gem_score_texture_ ( &gem_score_textures_[ 0 ] ),
	current_time_score_texture_ ( &time_score_textures_[ 0 ] ),
	name_textures_ (),
	gem_score_textures_ (),
	time_score_textures_ (),
	gem_score_target_texture_ (),
	time_score_target_texture_ (),
	bg_frame_ ( "bg/overworld-level-name-frame.png", { 0, 0, Unit::WINDOW_WIDTH_PIXELS, 40 }, AREA ),
	negative_pts_icon_ ( "bg/level-select-characters.png", { 8, 0, 8, 8 }, { LEFT_EDGE, ROW_3, 8, 8 } ),
	win_icon_ ( "bg/level-select-characters.png", { 0, 16, 8, 8 }, { LEFT_EDGE, ROW_1, 8, 8 } ),
	diamond_icon_ ( "bg/level-select-characters.png", { 0, 24, 8, 8 }, { LEFT_EDGE, ROW_2, 8, 8 } ),
	gem_score_icon_ ( "bg/level-select-characters.png", { 0, 0, 8, 8 }, { SCORE_X - 8, ROW_1, 8, 8 } ),
	time_score_icon_ ( "bg/level-select-characters.png", { 0, 8, 8, 8 }, { SCORE_X, ROW_2, 8, 8 } ),
	suits_icon_ ( "bg/level-select-characters.png", { 0, 232, 8, 8 }, { LEFT_EDGE + 8, ROW_1, 8, 8 } ),
	secret_goal_icon_ ( "bg/level-select-characters.png", { 0, 200, 8, 8 }, { LEFT_EDGE + 8, ROW_2, 8, 8 } ),
	level_icon_ ( "bg/level-select-characters.png", { 0, 48, 8, 8 }, { LEVEL_NAME_X + 8, ROW_1, 8, 8 } )
{};

OWInventory::~OWInventory()
{
	for ( int i = 0; i < ( int )( WTextCharacter::Color::__NULL ); ++i )
	{
		name_textures_[ i ].destroy();
		gem_score_textures_[ i ].destroy();
		time_score_textures_[ i ].destroy();
	}
	gem_score_target_texture_.destroy();
	time_score_target_texture_.destroy();
};

void OWInventory::init()
{
	for ( int i = 0; i < ( int )( WTextCharacter::Color::__NULL ); ++i )
	{
		name_textures_[ i ].init();
		gem_score_textures_[ i ].init();
		time_score_textures_[ i ].init();
	}
	gem_score_target_texture_.init();
	time_score_target_texture_.init();
};

void OWInventory::update( OWTile space )
{
	space_ = space;
	Inventory::updateForOverworld();
	updateShowChallengeScoresInput();
	updateFlashColor();
	if ( testOnDifferentLevel() )
	{
		if ( space_.isLevel() )
		{
			regenerateLevelGraphics();
		}
		else if ( space_.isShop() )
		{
			regenerateShopGraphics();
		}
		previous_space_ = space_;
	}
};

void OWInventory::render()
{
	bg_frame_.render();
	if ( space_.isLevel() )
	{
		renderLevelInfo();
	}
	else if ( space_.isShop() )
	{
		name_textures_[ 0 ].render();
	}
	renderPts();
};

void OWInventory::renderLevelInfo()
{
	win_icon_.render();
	if ( LevelList::hasCard( space_.getLevelNumber() ) )
	{
		diamond_icon_.render();
	}
	if ( LevelList::hasSuits( space_.getLevelNumber() ) )
	{
		suits_icon_.render();
	}
	if ( LevelList::hasGemScore( space_.getLevelNumber() ) )
	{
		gem_score_icon_.render();
		current_gem_score_texture_->render();
	}
	if ( LevelList::hasTimeScore( space_.getLevelNumber() ) )
	{
	time_score_icon_.render();
	current_time_score_texture_->render();
	}
	secret_goal_icon_.render();
	name_textures_[ getFlashColor() ].render();
}

void OWInventory::renderPts()
{
	Text::FontColor color = Text::FontColor::BLACK;
	if ( testMoneyInTheRed() )
	{
		color = Text::FontColor::DARK_GRAY;
		negative_pts_icon_.render();
	}
	Text::renderNumber( Inventory::totalFundsShown(), AREA.x + 24, ROW_3, 9, color );
}

void OWInventory::setShowChallengesOn()
{
	gem_score_icon_.src_.x = time_score_icon_.src_.x = 8;
	show_challenges_ = true;
	current_gem_score_texture_ = &gem_score_target_texture_;
	current_time_score_texture_ = &time_score_target_texture_;
};

void OWInventory::setShowChallengesOff()
{
	gem_score_icon_.src_.x = time_score_icon_.src_.x = 0;
	show_challenges_ = false;
	updateTextFlashColor();
};

void OWInventory::updateTextFlashColor()
{
	if ( space_.isLevel() )
	{
		if ( Inventory::levelComplete( space_.getLevelNumber() ) )
		{
			win_icon_.src_.x = getFlashColor() * 8;
			diamond_icon_.src_.x = getFlashColor() * 8;
			suits_icon_.src_.x = getFlashColor() * 8;
			secret_goal_icon_.src_.x = getFlashColor() * 8;
		}

		if ( !show_challenges_ )
		{
			if ( Inventory::gemChallengeBeaten( space_.getLevelNumber() ) )
			{
				gem_score_icon_.src_.x = getFlashColor() * 8;
			}
			current_gem_score_texture_ = &gem_score_textures_[ getFlashColor() ];
		}

		if ( !show_challenges_ )
		{
			if ( Inventory::timeChallengeBeaten( space_.getLevelNumber() ) )
			{
				time_score_icon_.src_.x = getFlashColor() * 8;
			}
			current_time_score_texture_ = &time_score_textures_[ getFlashColor() ];
		}
	}
};

bool OWInventory::testOnDifferentLevel() const
{
	return previous_space_ != space_;
};

void OWInventory::regenerateLevelGraphics()
{
	if ( Inventory::victory( space_.getLevelNumber() ) )
	{
		win_icon_.src_.x = 16;
		win_icon_.src_.y = 40;
	}
	else
	{
		win_icon_.src_.x = 0;
		win_icon_.src_.y = 16;
	}
	if ( Inventory::haveDiamond( space_.getLevelNumber() ) )
	{
		diamond_icon_.src_.x = 16;
		diamond_icon_.src_.y = 32;
	}
	else
	{
		diamond_icon_.src_.x = 8;
		diamond_icon_.src_.y = 24;
	}
	suits_icon_.src_.x = ( Inventory::hasSuits( space_.getLevelNumber() ) ) ? 16 : 0;

	if ( LevelList::hasSecretGoal( space_.getLevelNumber() ) )
	{
		secret_goal_icon_.src_.w = secret_goal_icon_.src_.h = 8;
		secret_goal_icon_.src_.x = ( Inventory::getSecretGoal( space_.getLevelNumber() ) ) ? 24 : 0;
	}
	else
	{
		secret_goal_icon_.src_.w = secret_goal_icon_.src_.h = 0;
	}

	WTextObj gem_score = { Inventory::gemScore( space_.getLevelNumber() ).c_str(), SCORE_X, ROW_1 };
	WTextObj time_score = { Inventory::timeScore( space_.getLevelNumber() ), SCORE_X + 8, ROW_2 };

	// Generate texture for each color o’ flashing text.
	regenerateLevelNameGraphics();
	for ( int i = 0; i < ( int )( WTextCharacter::Color::__NULL ); ++i )
	{
		gem_score.changeColor( Inventory::gemChallengeBeaten( space_.getLevelNumber() ) ? ( WTextCharacter::Color )( i ) : WTextCharacter::Color::BLACK );
		gem_score_textures_[ i ].init();
		gem_score_textures_[ i ].startDrawing();
		Render::clearScreenTransparency();
		gem_score.render();
		gem_score_textures_[ i ].endDrawing();

		time_score.changeColor( Inventory::timeChallengeBeaten( space_.getLevelNumber() ) ? ( WTextCharacter::Color )( i ) : WTextCharacter::Color::BLACK );
		time_score_textures_[ i ].init();
		time_score_textures_[ i ].startDrawing();
		Render::clearScreenTransparency();
		time_score.render();
		time_score_textures_[ i ].endDrawing();
	}
	const WTextObj gem_score_target = { LevelList::gemChallengeText( space_.getLevelNumber() ), SCORE_X, ROW_1, WTextCharacter::Color::DARK_GRAY };
	const WTextObj time_score_target = { LevelList::timeChallengeText( space_.getLevelNumber() ), SCORE_X + 8, ROW_2, WTextCharacter::Color::DARK_GRAY };
	gem_score_target_texture_.startDrawing();
		Render::clearScreenTransparency();
	gem_score_target.render();
	gem_score_target_texture_.endDrawing();
	time_score_target_texture_.startDrawing();
		Render::clearScreenTransparency();
	time_score_target.render();
	time_score_target_texture_.endDrawing();
	updateTextFlashColor();
};

WTextObj OWInventory::generateName( std::u32string name, int offset ) const
{
	return
	{
		name,
		LEVEL_NAME_X + offset,
		ROW_1,
		WTextCharacter::Color::BLACK,
		312 - offset,
		WTextObj::Align::LEFT,
		WTextCharacter::Color::__NULL,
		8
	};
};

void OWInventory::regenerateLevelNameGraphics()
{
	const std::u32string level_name_header_text = LevelList::getLevelHeader( space_.getLevelNumber() );
	WTextObj level_name_header { level_name_header_text, LEVEL_NAME_X + 16, ROW_1 };
	WTextObj level_name = generateName( Localization::getCurrentLanguage().getLevelName( LevelList::getCodeNameFromID( space_.getLevelNumber() ) ), WTextCharacter::SIZE_PIXELS * ( level_name_header_text.length() + 1 ) );
	for ( int i = 0; i < ( int )( WTextCharacter::Color::__NULL ); ++i )
	{
		const WTextCharacter::Color color = Inventory::levelComplete( space_.getLevelNumber() ) ? ( WTextCharacter::Color )( i ) : WTextCharacter::Color::BLACK;
		level_name_header.changeColor( color );
		level_name.changeColor( color );

		name_textures_[ i ].startDrawing();
		Render::clearScreenTransparency();
		level_name_header.render();
		level_name.render();
		name_textures_[ i ].endDrawing();
	}
};

void OWInventory::updateFlashColor()
{
	++color_animation_timer_;
	if ( color_animation_timer_ == FLASH_SPEED )
	{
		color_animation_timer_ = 0;
		++color_animation_;
		if ( color_animation_ == NUMBER_OF_FLASH_FRAMES )
		{
			color_animation_ = 0;
		}
		updateTextFlashColor();
	}
};

void OWInventory::updateShowChallengeScoresInput()
{
	if ( !Input::held( Input::Action::CANCEL ) )
	{
		show_challenges_lock_ = false;
	}
	else if ( !space_.isLevel() )
	{
		show_challenges_lock_ = true;
	}

	if ( !show_challenges_lock_ && Input::held( Input::Action::CANCEL ) )
	{
		setShowChallengesOn();
		if ( !sound_lock_ )
		{
			Audio::playSound( Audio::SoundType::SELECT );
			sound_lock_ = true;
		}
	}
	else
	{
		setShowChallengesOff();
		sound_lock_ = false;
	}
};

bool OWInventory::testMoneyInTheRed() const
{
	return Inventory::totalFundsShown() < 0;
};

void OWInventory::forceLevelNameRedraw()
{
	if ( space_.isShop() )
	{
		regenerateShopGraphics();
	}
	else if ( space_.isLevel() )
	{
		regenerateLevelNameGraphics();
	}
};

void OWInventory::regenerateShopGraphics()
{
	WTextObj shop_name = generateName( Localization::getCurrentLanguage().getOverworldShopTitle(), 0 );
	name_textures_[ 0 ].startDrawing();
	Render::clearScreenTransparency();
	shop_name.render();
	name_textures_[ 0 ].endDrawing();
};