#include "camera.hpp"
#include "clock.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "inventory_level.hpp"
#include "map.hpp"
#include "mezun_helpers.hpp"
#include "render.hpp"
#include "sprite.hpp"
#include "wtext_obj.hpp"

static constexpr int MISC_X = 185;
static constexpr int FLASHING_TIMER_SHADES_NUM = 12;
static constexpr WTextCharacter::Color FLASHING_TIMER_SHADES[ FLASHING_TIMER_SHADES_NUM ] =
{
	WTextCharacter::Color::BLACK,
	WTextCharacter::Color::DARK_GRAY,
	WTextCharacter::Color::DARK_MID_GRAY,
	WTextCharacter::Color::LIGHT_MID_GRAY,
	WTextCharacter::Color::LIGHT_GRAY,
	WTextCharacter::Color::WHITE,
	WTextCharacter::Color::WHITE,
	WTextCharacter::Color::LIGHT_GRAY,
	WTextCharacter::Color::LIGHT_MID_GRAY,
	WTextCharacter::Color::DARK_MID_GRAY,
	WTextCharacter::Color::DARK_GRAY,
	WTextCharacter::Color::BLACK
};
static constexpr int FLASHING_TIMER_SPEED = 8;

InventoryLevel::InventoryLevel()
:
	health_gfx_ ( Y ),
	oxygen_meter_ ( TOP_ROW_Y ),
	ticker_ ( BOTTOM_ROW_Y ),
	flashing_timer_ ( 0 ),
	flashing_time_shade_ ( 0 ),
	main_texture_ ( Unit::WINDOW_WIDTH_PIXELS, HEIGHT, 0, Y ),
	kill_count_ ( -1 ),
	showing_key_ ( false ),
	show_on_off_ ( false ),
	on_off_state_ ( false ),
	mcguffins_to_render_ ( -1 )
{};

InventoryLevel::~InventoryLevel()
{
	main_texture_.destroy();
};

void InventoryLevel::update( EventSystem& events, const Health& health )
{
	oxygen_meter_.update( health );
	if ( Inventory::updateLevelFunds() )
	{
		updatePtsGraphics();
	}
	if ( Inventory::clock().update() )
	{
		updateTimerGraphics();
	}
	if ( health_gfx_.update( health ) )
	{
		updateHealthGraphics();
	}
	ticker_.update();
	if ( Inventory::clock().lowOnTime() )
	{
		++flashing_timer_;
		if ( flashing_timer_ >= Inventory::clock().timeRemaining() )
		{
			flashing_timer_ = 0;
			++flashing_time_shade_;
			if ( flashing_time_shade_ == FLASHING_TIMER_SHADES_NUM )
			{
				flashing_time_shade_ = 0;
			}
			updateTimerGraphics();
		}
	}
	if ( !showing_key_ && events.hasKey() )
	{
		updateKeyGraphics();
		showing_key_ = true;
	}
	if ( show_on_off_ && on_off_state_ != events.switch_ )
	{
		on_off_state_ = events.switch_;
		updateSwitchGraphics();
	}
	if ( mcguffins_to_render_ != -1 && mcguffins_to_render_ != Inventory::McGuffins() )
	{
		mcguffins_to_render_ = Inventory::McGuffins();
		updateMcGuffinGraphics();
	}
};

void InventoryLevel::init( const Map& lvmap )
{
	show_on_off_ = lvmap.show_on_off_;
	main_texture_.init();
	forceRerender();
};

void InventoryLevel::render( const EventSystem& events, const Sprite& hero, const Camera& camera )
{
	main_texture_.render();

	// OXYGEN
	oxygen_meter_.render();

	// TICKER
	if ( events.showBossUI() )
	{
		events.renderBossUI();
	}
	else
	{
		ticker_.render();
	}

	// BOPS
	if ( Inventory::bopsMultiplier() )
	{
		WTextObj text( mezun::intToChar32String( Inventory::howManyBops() ), camera.relativeX( hero.centerXPixels() - 4 ), camera.relativeY( hero.yPixels() - 12 ), WTextCharacter::Color::DARK_GRAY );
		text.render();
	}
	else if ( Inventory::multipleGhostKills() )
	{
		WTextObj text( mezun::intToChar32String( Inventory::howManyGhostKills() ), camera.relativeX( hero.centerXPixels() - 4 ), camera.relativeY( hero.yPixels() - 12 ), WTextCharacter::Color::DARK_GRAY );
		text.render();
	}
};

void InventoryLevel::setShowMcGuffins()
{
	if ( mcguffins_to_render_ == -1 )
	{
		mcguffins_to_render_ = 0;
	}
};

void InventoryLevel::changeKillCounter( int count )
{
	kill_count_ = count;
	updateKillCountGraphics();
};

void InventoryLevel::updateHealthGraphics()
{
	main_texture_.startDrawing();
	health_gfx_.render();
	main_texture_.endDrawing();
};

void InventoryLevel::forceRerender()
{
	main_texture_.startDrawing();
	Render::clearScreenTransparency();
	Render::renderObject( "bg/level-inventory-frame.png", { 0, 0, Unit::WINDOW_WIDTH_PIXELS, HEIGHT }, { 0, 0, Unit::WINDOW_WIDTH_PIXELS, HEIGHT } );
	if ( Inventory::victory() )
	{
		Render::renderObject( "bg/level-select-characters.png", { 16, 40, 8, 8 }, { 10, TOP_ROW_Y_RELATIVE, 8, 8 } );
	}
	if ( Inventory::haveDiamond() )
	{
		Render::renderObject( "bg/level-select-characters.png", { 16, 32, 8, 8 }, { 18, TOP_ROW_Y_RELATIVE, 8, 8 } );
	}
	health_gfx_.render();
	renderPtsGraphics();
	renderTimerGraphics();
	if ( kill_count_ != -1 )
	{
		renderKillCountGraphics();
	}
	if ( showing_key_ )
	{
		renderKeyGraphics();
	}
	if ( show_on_off_ )
	{
		renderSwitchGraphics();
	}
	if ( mcguffins_to_render_ != -1 )
	{
		renderMcGuffinGraphics();
	}
	main_texture_.endDrawing();
	ticker_.forceRedraw();
};

void InventoryLevel::updatePtsGraphics()
{
	main_texture_.startDrawing();
	Render::renderRect( { 42, TOP_ROW_Y_RELATIVE, 8 * 5, 8 }, 1 );
	renderPtsGraphics();
	main_texture_.endDrawing();
};

void InventoryLevel::renderPtsGraphics()
{
	WTextObj text{ Inventory::fundsString(), 42, TOP_ROW_Y_RELATIVE };
	text.render();
};

void InventoryLevel::updateTimerGraphics()
{
	main_texture_.startDrawing();
	Render::renderRect( { CLOCK_X, TOP_ROW_Y_RELATIVE, 8 * 4, 8 }, 1 );
	renderTimerGraphics();
	main_texture_.endDrawing();
};

void InventoryLevel::renderTimerGraphics()
{
	WTextObj text{ Inventory::clock().getTimeString(), CLOCK_X, TOP_ROW_Y_RELATIVE, FLASHING_TIMER_SHADES[ flashing_time_shade_ ] };
	text.render();
};

void InventoryLevel::updateKillCountGraphics()
{
	main_texture_.startDrawing();
	Render::renderRect( { MISC_X, TOP_ROW_Y_RELATIVE, 8 * 4, 8 }, 1 );
	renderKillCountGraphics();
	main_texture_.endDrawing();
};

void InventoryLevel::renderKillCountGraphics()
{
	Render::renderObject( "bg/level-select-characters.png", { 0, 184, 8, 8 }, { MISC_X, TOP_ROW_Y_RELATIVE, 8, 8 } );
	WTextObj text{ U"x" + mezun::intToChar32StringWithPadding( kill_count_, 2 ), MISC_X + 8, TOP_ROW_Y_RELATIVE };
	text.render();
};

void InventoryLevel::updateKeyGraphics()
{
	main_texture_.startDrawing();
	Render::renderRect( { MISC_X, TOP_ROW_Y_RELATIVE, 8, 8 }, 1 );
	renderKeyGraphics();
	main_texture_.endDrawing();
};

void InventoryLevel::renderKeyGraphics()
{
	Render::renderObject( "bg/level-select-characters.png", { 8, 184, 8, 8 }, { MISC_X, TOP_ROW_Y_RELATIVE, 8, 8 } );
};

void InventoryLevel::updateSwitchGraphics()
{
	main_texture_.startDrawing();
	Render::renderRect( { MISC_X, TOP_ROW_Y_RELATIVE, 8 * 3, 8 }, 1 );
	renderSwitchGraphics();
	main_texture_.endDrawing();
};

void InventoryLevel::renderSwitchGraphics()
{
	const std::u32string string = ( on_off_state_ ) ? U"ON" : U"OFF";
	WTextObj text( string, MISC_X, TOP_ROW_Y_RELATIVE );
	text.render();
};

void InventoryLevel::updateMcGuffinGraphics()
{
	main_texture_.startDrawing();
	Render::renderRect( { MISC_X, TOP_ROW_Y_RELATIVE, 8 * 3, 8 }, 1 );
	renderMcGuffinGraphics();
	main_texture_.endDrawing();
};

void InventoryLevel::renderMcGuffinGraphics()
{
	Render::renderObject( "bg/level-select-characters.png", { 16, 184, 8, 8 }, { MISC_X, TOP_ROW_Y_RELATIVE, 8, 8 } );
	WTextObj text{ U"x" + mezun::intToChar32String( mcguffins_to_render_ ), MISC_X + 8, TOP_ROW_Y_RELATIVE };
	text.render();
};