#include "audio.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "level.hpp"
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
	prev_level_ ( -1 ),
	level_ ( -1 ),
	current_gem_score_texture_ ( &gem_score_textures_[ 0 ] ),
	current_time_score_texture_ ( &time_score_textures_[ 0 ] ),
	level_name_textures_ (),
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
	crown_icon_ ( "bg/level-select-characters.png", { 0, 192, 8, 8 }, { LEFT_EDGE + 8, ROW_1, 8, 8 } ),
	secret_goal_icon_ ( "bg/level-select-characters.png", { 0, 200, 8, 8 }, { LEFT_EDGE + 8, ROW_2, 8, 8 } )
{};

OWInventory::~OWInventory()
{
	for ( int i = 0; i < ( int )( WTextCharacter::Color::__NULL ); ++i )
	{
		level_name_textures_[ i ].destroy();
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
		level_name_textures_[ i ].init();
		gem_score_textures_[ i ].init();
		time_score_textures_[ i ].init();
	}
	gem_score_target_texture_.init();
	time_score_target_texture_.init();
};

void OWInventory::update( int level )
{
	level_ = level;
	Inventory::updateForOverworld();
	updateShowChallengeScoresInput();
	updateFlashColor();
	if ( testOnDifferentLevel() )
	{
		if ( testStandingOnLevel() )
		{
			regenerateLevelGraphics();
		}
		else if ( testStandingOnShop() )
		{
			regenerateShopGraphics();
		}
		prev_level_ = level_;
	}
};

void OWInventory::render()
{
	bg_frame_.render();
	if ( testStandingOnLevel() )
	{
		renderLevelInfo();
	}
	else if ( testStandingOnShop() )
	{
		level_name_textures_[ 0 ].render();
	}
	renderPts();
};

void OWInventory::renderLevelInfo()
{
	win_icon_.render();
	diamond_icon_.render();
	crown_icon_.render();
	gem_score_icon_.render();
	time_score_icon_.render();
	current_gem_score_texture_->render();
	current_time_score_texture_->render();
	secret_goal_icon_.render();
	level_name_textures_[ getFlashColor() ].render();
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
	if ( testStandingOnLevel() )
	{
		if ( Inventory::levelComplete( level_ ) )
		{
			win_icon_.src_.x = getFlashColor() * 8;
			diamond_icon_.src_.x = getFlashColor() * 8;
			crown_icon_.src_.x = getFlashColor() * 8;
			secret_goal_icon_.src_.x = getFlashColor() * 8;
		}

		if ( !show_challenges_ )
		{
			if ( Inventory::gemChallengeBeaten( level_ ) )
			{
				gem_score_icon_.src_.x = getFlashColor() * 8;
			}
			current_gem_score_texture_ = &gem_score_textures_[ getFlashColor() ];
		}

		if ( !show_challenges_ )
		{
			if ( Inventory::timeChallengeBeaten( level_ ) )
			{
				time_score_icon_.src_.x = getFlashColor() * 8;
			}
			current_time_score_texture_ = &time_score_textures_[ getFlashColor() ];
		}
	}
};

bool OWInventory::testOnDifferentLevel() const
{
	return prev_level_ != level_;
};

void OWInventory::regenerateLevelGraphics()
{
	if ( Inventory::victory( level_ ) )
	{
		win_icon_.src_.x = 16;
		win_icon_.src_.y = 40;
	}
	else
	{
		win_icon_.src_.x = 0;
		win_icon_.src_.y = 16;
	}
	if ( Inventory::haveDiamond( level_ ) )
	{
		diamond_icon_.src_.x = 16;
		diamond_icon_.src_.y = 32;
	}
	else
	{
		diamond_icon_.src_.x = 8;
		diamond_icon_.src_.y = 24;
	}
	crown_icon_.src_.x = ( Inventory::hasCrown( level_ ) ) ? 24 : 0;

	if ( Level::hasSecretGoal( level_ ) )
	{
		secret_goal_icon_.src_.w = secret_goal_icon_.src_.h = 8;
		secret_goal_icon_.src_.x = ( Inventory::getSecretGoal( level_ ) ) ? 24 : 0;
	}
	else
	{
		secret_goal_icon_.src_.w = secret_goal_icon_.src_.h = 0;
	}

	WTextObj gem_score = { Inventory::gemScore( level_ ).c_str(), SCORE_X, ROW_1 };
	WTextObj time_score = { Inventory::timeScore( level_ ), SCORE_X + 8, ROW_2 };
	WTextObj level_name = generateLevelName();

	// Generate texture for each color o’ flashing text.
	for ( int i = 0; i < ( int )( WTextCharacter::Color::__NULL ); ++i )
	{
		regenerateLevelNameGraphics( level_name, i );

		gem_score.changeColor( Inventory::gemChallengeBeaten( level_ ) ? ( WTextCharacter::Color )( i ) : WTextCharacter::Color::BLACK );
		gem_score_textures_[ i ].init();
		gem_score_textures_[ i ].startDrawing();
		Render::clearScreenTransparency();
		gem_score.render();
		gem_score_textures_[ i ].endDrawing();

		time_score.changeColor( Inventory::timeChallengeBeaten( level_ ) ? ( WTextCharacter::Color )( i ) : WTextCharacter::Color::BLACK );
		time_score_textures_[ i ].init();
		time_score_textures_[ i ].startDrawing();
		Render::clearScreenTransparency();
		time_score.render();
		time_score_textures_[ i ].endDrawing();
	}
	const WTextObj gem_score_target = { Level::gemChallengeText( level_ ), SCORE_X, ROW_1, WTextCharacter::Color::DARK_GRAY };
	const WTextObj time_score_target = { Level::timeChallengeText( level_ ), SCORE_X + 8, ROW_2, WTextCharacter::Color::DARK_GRAY };
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

WTextObj OWInventory::generateName( std::u32string text ) const
{
	return { text, LEVEL_NAME_X, ROW_1, WTextCharacter::Color::BLACK, 312, WTextObj::Align::LEFT, WTextCharacter::Color::__NULL, 8 };
};

WTextObj OWInventory::generateLevelName() const
{
	return generateName( Level::getLevelNames()[ level_ ] );
};

WTextObj OWInventory::generateShopName() const
{
	return generateName( Localization::getCurrentLanguage().getOverworldShopTitle() );
};

void OWInventory::regenerateLevelNameGraphics( WTextObj& level_name, int i )
{
	level_name.changeColor( Inventory::levelComplete( level_ ) ? ( WTextCharacter::Color )( i ) : WTextCharacter::Color::BLACK );
	level_name_textures_[ i ].startDrawing();
	Render::clearScreenTransparency();
	level_name.render();
	level_name_textures_[ i ].endDrawing();
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
	else if ( level_ < 0 )
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

bool OWInventory::testStandingOnLevel() const
{
	return level_ > -1;
};

bool OWInventory::testStandingOnShop() const
{
	return level_ < -1;
};

void OWInventory::forceLevelNameRedraw()
{
	if ( testStandingOnShop() )
	{
		regenerateShopGraphics();
	}
	else if ( testStandingOnLevel() )
	{
		WTextObj level_name = generateLevelName();
		for ( int i = 0; i < ( int )( WTextCharacter::Color::__NULL ); ++i )
		{
			regenerateLevelNameGraphics( level_name, i );
		}
	}
};

void OWInventory::regenerateShopGraphics()
{
	WTextObj shop_name = generateShopName();
	level_name_textures_[ 0 ].startDrawing();
	Render::clearScreenTransparency();
	shop_name.render();
	level_name_textures_[ 0 ].endDrawing();
};