#include "camera.hpp"
#include "clock.hpp"
#include "event_system.hpp"
#include "inventory.hpp"
#include "inventory_level.hpp"
#include "inventory_level_graphics.hpp"
#include "level.hpp"
#include "map.hpp"
#include "mezun_helpers.hpp"
#include "render.hpp"
#include "sprite.hpp"
#include "wtext_obj.hpp"

static constexpr int FLASHING_TIMER_SHADES_NUM = 12;

static constexpr int PTS_X = 10;
static constexpr int PTS_COUNTER_X = PTS_X + 8;
static constexpr int PTS_COUNTER_WIDTH = 8 * 5;
static constexpr int HP_X = PTS_COUNTER_X + PTS_COUNTER_WIDTH + 8;
static constexpr int CLOCK_FROM_HP = 0;
static constexpr int TIME_FROM_HP = CLOCK_FROM_HP + 8;
static constexpr int TIME_WIDTH = 4 * 8;
static constexpr int CARD_FROM_HP = TIME_FROM_HP + TIME_WIDTH + 8;
static constexpr int SUITS_FROM_HP = CARD_FROM_HP + 8;
static constexpr int SUIT_CLUB_FROM_HP = SUITS_FROM_HP;
static constexpr int SUIT_DIAMOND_FROM_HP = SUIT_CLUB_FROM_HP + 8;
static constexpr int SUIT_HEART_FROM_HP = SUIT_DIAMOND_FROM_HP + 8;
static constexpr int SUIT_SPADE_FROM_HP = SUIT_HEART_FROM_HP + 8;
static constexpr int SUITS_WIDTH = NUMBER_OF_CARD_SUITS * 8;
static constexpr int MISC_FROM_HP = SUITS_FROM_HP + SUITS_WIDTH + 8;
static constexpr int OXYGEN_RIGHT = 390;
static constexpr int COUNT_WIDTH = 3 * 8;
static constexpr int KEY_WIDTH = 8;
static constexpr int ON_OFF_WIDTH = 3 * 8;

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

InventoryLevelGraphics::InventoryLevelGraphics( int max_hp, bool live_update, int bg_color )
:
	flashing_timer_ ( 0 ),
	flashing_time_shade_ ( 0 ),
	count_index_ ( -1 ),
	misc_end_x_ ( 0 ),
	text_color_ ( WTextCharacter::Color::BLACK ),
	main_texture_ ( Unit::WINDOW_WIDTH_PIXELS, HEIGHT, 0, Y ),
	health_gfx_ ( HP_X, ( ( live_update ) ? Y : 0 ) + 10, max_hp, bg_color ),
	ticker_ ( BOTTOM_ROW_Y ),
	oxygen_meter_ ( OXYGEN_RIGHT, TOP_ROW_Y ),
	bg_frame_ ( 0, ( live_update ) ? Y : 0, Unit::WINDOW_WIDTH_PIXELS, HEIGHT, bg_color ),
	live_ ( live_update ),
	y_ ( ( live_update ) ? Y : 0 ),
	bg_color_ ( bg_color )
{
	misc_end_x_ = MISC_FROM_HP + getXFromHP();
};

InventoryLevelGraphics::~InventoryLevelGraphics()
{
	main_texture_.destroy();
};

void InventoryLevelGraphics::update( const EventSystem& events, const Health& health, const InventoryLevel& inventory, bool funds_changed, bool time_changed )
{
	oxygen_meter_.update( health );
	if ( funds_changed )
	{
		updatePtsGraphics( inventory );
	}
	if ( time_changed )
	{
		updateTimerGraphics( inventory );
	}
	if ( health_gfx_.update( health ) )
	{
		updateHealthGraphics();
	}
	ticker_.update();
	if ( inventory.clock().lowOnTime() )
	{
		++flashing_timer_;
		if ( flashing_timer_ >= inventory.clock().timeRemaining() )
		{
			flashing_timer_ = 0;
			++flashing_time_shade_;
			if ( flashing_time_shade_ == FLASHING_TIMER_SHADES_NUM )
			{
				flashing_time_shade_ = 0;
			}
			updateTimerGraphics( inventory );
		}
	}

	for ( auto& extra_counter : extra_counters_ )
	{
		switch ( extra_counter.type )
		{
			case ( InventoryCounterType::KEY ):
			{
				if ( !extra_counter.number && events.hasKey() )
				{
					extra_counter.number = 1;
					updateKeyGraphics( extra_counter );
				}
			}
			break;
			case ( InventoryCounterType::ON_OFF ):
			{
				if ( extra_counter.number != events.isSwitchOn() )
				{
					extra_counter.number = events.isSwitchOn();
					updateSwitchGraphics( extra_counter );
				}
			}
			break;
		}
	}
};

void InventoryLevelGraphics::init( const Level& level, const InventoryLevel& inventory )
{
	const Map& lvmap = level.currentMap();

	if ( lvmap.show_key_ )
	{
		extra_counters_.push_back
		(
			InventoryCounter
			{
				InventoryCounterType::KEY,
				0,
				{ misc_end_x_, y_ + TOP_ROW_Y_RELATIVE, KEY_WIDTH, 8 }
			}
		);
		misc_end_x_ += KEY_WIDTH + 8;
	}

	if ( lvmap.show_on_off_ )
	{
		extra_counters_.push_back
		(
			InventoryCounter
			{
				InventoryCounterType::ON_OFF,
				0,
				{ misc_end_x_, y_ + TOP_ROW_Y_RELATIVE, ON_OFF_WIDTH, 8 }
			}
		);
		misc_end_x_ += ON_OFF_WIDTH + 8;
	}

	main_texture_.init();
	forceRerender( inventory );
};

void InventoryLevelGraphics::render( const EventSystem& events, const Sprite& hero, const Camera& camera, const InventoryLevel& inventory )
{
	if ( live_ )
	{
		rerender( inventory );
	}
	else
	{
		main_texture_.render();
	}

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
};


void InventoryLevelGraphics::renderBops( const Sprite& hero, const Camera& camera, const InventoryLevel& inventory ) const
{
	// BOPS
	if ( inventory.bopsMultiplier() )
	{
		WTextObj text( mezun::intToChar32String( inventory.howManyBops() ), camera.relativeX( hero.centerXPixels() - 4 ), camera.relativeY( hero.yPixels() - 12 ), WTextCharacter::Color::DARK_GRAY );
		text.render();
	}
	else if ( inventory.multipleGhostKills() )
	{
		WTextObj text( mezun::intToChar32String( inventory.howManyGhostKills() ), camera.relativeX( hero.centerXPixels() - 4 ), camera.relativeY( hero.yPixels() - 12 ), WTextCharacter::Color::DARK_GRAY );
		text.render();
	}
};

void InventoryLevelGraphics::setShowCounter( Icon icon, int start_count )
{
	count_index_ = extra_counters_.size();
	extra_counters_.push_back
	(
		InventoryCounter
		{
			InventoryCounterType::COUNT,
			start_count,
			{ misc_end_x_, y_ + TOP_ROW_Y_RELATIVE, COUNT_WIDTH, 8 },
			icon
		}
	);
	misc_end_x_ += COUNT_WIDTH + 8;
};

void InventoryLevelGraphics::changeCounter( int count )
{
	extra_counters_[ count_index_ ].number = count;
	updateCountGraphics( extra_counters_[ count_index_ ] );
};

void InventoryLevelGraphics::updateHealthGraphics()
{
	main_texture_.startDrawing();
	health_gfx_.render();
	main_texture_.endDrawing();
};

void InventoryLevelGraphics::forceRerender( const InventoryLevel& inventory )
{
	main_texture_.startDrawing();
	Render::clearScreenTransparency();
	rerender( inventory );
	main_texture_.endDrawing();
	ticker_.forceRedraw();
};

void InventoryLevelGraphics::rerender( const InventoryLevel& inventory )
{
	// Render BG Frame.
	bg_frame_.render();

	// Render ₧
	WTextObj pts_icon{ mezun::charToChar32String( "₧" ), PTS_X, y_ + TOP_ROW_Y_RELATIVE, text_color_ };
	pts_icon.render();
	renderPtsGraphics( inventory );

	// Render HP.
	health_gfx_.render();

	// Render time.
	Render::renderObject( "bg/level-select-characters.png", { colorX(), 8, 8, 8 }, { CLOCK_FROM_HP + getXFromHP(), y_ + TOP_ROW_Y_RELATIVE, 8, 8 } );
	renderTimerGraphics( inventory );

	// Render card.
	if ( Inventory::haveDiamond() )
	{
		renderDiamond();
	}
	else
	{
		Render::renderObject( "bg/level-select-characters.png", { colorX(), 24, 8, 8 }, { CARD_FROM_HP + getXFromHP(), y_ + TOP_ROW_Y_RELATIVE, 8, 8 } );
	}

	// Render suits.
	if ( inventory.haveSuit( CardSuit::CLUB ) )
	{
		renderClubSuit();
	}
	else
	{
		Render::renderObject( "bg/level-select-characters.png", { 0, 208, 8, 8 }, { SUIT_CLUB_FROM_HP + getXFromHP(), y_ + TOP_ROW_Y_RELATIVE, 8, 8 } );
	}
	if ( inventory.haveSuit( CardSuit::DIAMOND ) )
	{
		renderDiamondSuit();
	}
	else
	{
		Render::renderObject( "bg/level-select-characters.png", { 0, 216, 8, 8 }, { SUIT_DIAMOND_FROM_HP + getXFromHP(), y_ + TOP_ROW_Y_RELATIVE, 8, 8 } );
	}
	if ( inventory.haveSuit( CardSuit::HEART ) )
	{
		renderHeartSuit();
	}
	else
	{
		Render::renderObject( "bg/level-select-characters.png", { 0, 224, 8, 8 }, { SUIT_HEART_FROM_HP + getXFromHP(), y_ + TOP_ROW_Y_RELATIVE, 8, 8 } );
	}
	if ( inventory.haveSuit( CardSuit::SPADE ) )
	{
		renderSpadeSuit();
	}
	else
	{
		Render::renderObject( "bg/level-select-characters.png", { 0, 232, 8, 8 }, { SUIT_SPADE_FROM_HP + getXFromHP(), y_ + TOP_ROW_Y_RELATIVE, 8, 8 } );
	}

	for ( auto& extra_counter : extra_counters_ )
	{
		switch ( extra_counter.type )
		{
			case ( InventoryCounterType::KEY ):
			{
				updateKeyGraphics( extra_counter );
			}
			break;
			case ( InventoryCounterType::ON_OFF ):
			{
				updateSwitchGraphics( extra_counter );
			}
			break;
			case ( InventoryCounterType::COUNT ):
			{
				updateCountGraphics( extra_counter );
			}
			break;
		}
	}
};

void InventoryLevelGraphics::updatePtsGraphics( const InventoryLevel& inventory )
{
	main_texture_.startDrawing();
	Render::renderRect( { PTS_COUNTER_X, y_ + TOP_ROW_Y_RELATIVE, PTS_COUNTER_WIDTH, 8 }, bg_color_ );
	renderPtsGraphics( inventory );
	main_texture_.endDrawing();
};

void InventoryLevelGraphics::renderPtsGraphics( const InventoryLevel& inventory )
{
	WTextObj text{ inventory.fundsString(), PTS_COUNTER_X, y_ + TOP_ROW_Y_RELATIVE, text_color_ };
	text.render();
};

void InventoryLevelGraphics::updateTimerGraphics( const InventoryLevel& inventory )
{
	main_texture_.startDrawing();
	Render::renderRect( { timeX(), y_ + TOP_ROW_Y_RELATIVE, TIME_WIDTH, 8 }, bg_color_ );
	renderTimerGraphics( inventory );
	main_texture_.endDrawing();
};

void InventoryLevelGraphics::renderTimerGraphics( const InventoryLevel& inventory )
{
	WTextObj text{ inventory.clock().getTimeString(), timeX(), y_ + TOP_ROW_Y_RELATIVE, FLASHING_TIMER_SHADES[ flashing_time_shade_ ] };
	text.render();
};

void InventoryLevelGraphics::updateCountGraphics( const InventoryCounter& counter )
{
	main_texture_.startDrawing();
	Render::renderRect( counter.position, bg_color_ );
	renderCountGraphics( counter );
	main_texture_.endDrawing();
};

void InventoryLevelGraphics::renderCountGraphics( const InventoryCounter& counter )
{
	Render::renderObject( "bg/level-select-characters.png", { ( int )( counter.icon ) * 8, 184, 8, 8 }, { counter.position.x, y_ + TOP_ROW_Y_RELATIVE, 8, 8 } );
	WTextObj text{ mezun::intToChar32StringWithPadding( counter.number, 2 ), counter.position.x + 8, y_ + TOP_ROW_Y_RELATIVE, text_color_ };
	text.render();
};

void InventoryLevelGraphics::updateKeyGraphics( const InventoryCounter& counter )
{
	main_texture_.startDrawing();
	Render::renderRect( counter.position, bg_color_ );
	renderKeyGraphics( counter );
	main_texture_.endDrawing();
};

void InventoryLevelGraphics::renderKeyGraphics( const InventoryCounter& counter )
{
	int src_x = ( counter.number ) ? colorX() : 4 * 8;
	Render::renderObject( "bg/level-select-characters.png", { src_x, 200, 8, 8 }, counter.position );
};

void InventoryLevelGraphics::updateSwitchGraphics( const InventoryCounter& counter )
{
	main_texture_.startDrawing();
	Render::renderRect( counter.position, bg_color_ );
	renderSwitchGraphics( counter );
	main_texture_.endDrawing();
};

void InventoryLevelGraphics::renderSwitchGraphics( const InventoryCounter& counter )
{
	const std::u32string string = ( counter.number ) ? U"ON" : U"OFF";
	WTextObj text( string, counter.position.x, y_ + TOP_ROW_Y_RELATIVE, text_color_ );
	text.render();
};

void InventoryLevelGraphics::reRenderDiamond()
{
	main_texture_.startDrawing();
	renderDiamond();
	main_texture_.endDrawing();
};

void InventoryLevelGraphics::renderDiamond()
{
	Render::renderObject( "bg/level-select-characters.png", { 16, 32, 8, 8 }, { CARD_FROM_HP + getXFromHP(), y_ + TOP_ROW_Y_RELATIVE, 8, 8 } );
};

int InventoryLevelGraphics::getXFromHP() const
{
	return HP_X + health_gfx_.width() + 8;
};

int InventoryLevelGraphics::colorX() const
{
	return 8 * ( int )( text_color_ );
};

int InventoryLevelGraphics::timeX() const
{
	return TIME_FROM_HP + getXFromHP();
};

void InventoryLevelGraphics::setSuitGotten( CardSuit suit )
{
	main_texture_.startDrawing();
	switch ( suit )
	{
		case ( CardSuit::CLUB ):
		{
			Render::renderRect( { SUIT_CLUB_FROM_HP + getXFromHP(), y_ + TOP_ROW_Y_RELATIVE, 8, 8 }, bg_color_ );
			renderClubSuit();
		}
		break;
		case ( CardSuit::DIAMOND ):
		{
			Render::renderRect( { SUIT_DIAMOND_FROM_HP + getXFromHP(), y_ + TOP_ROW_Y_RELATIVE, 8, 8 }, bg_color_ );
			renderDiamondSuit();
		}
		break;
		case ( CardSuit::HEART ):
		{
			Render::renderRect( { SUIT_HEART_FROM_HP + getXFromHP(), y_ + TOP_ROW_Y_RELATIVE, 8, 8 }, bg_color_ );
			renderHeartSuit();
		}
		break;
		case ( CardSuit::SPADE ):
		{
			Render::renderRect( { SUIT_SPADE_FROM_HP + getXFromHP(), y_ + TOP_ROW_Y_RELATIVE, 8, 8 }, bg_color_ );
			renderSpadeSuit();
		}
		break;
	}
	main_texture_.endDrawing();
};

void InventoryLevelGraphics::renderClubSuit()
{
	Render::renderObject( "bg/level-select-characters.png", { 8, 208, 8, 8 }, { SUIT_CLUB_FROM_HP + getXFromHP(), y_ + TOP_ROW_Y_RELATIVE, 8, 8 } );
};

void InventoryLevelGraphics::renderDiamondSuit()
{
	Render::renderObject( "bg/level-select-characters.png", { 8, 216, 8, 8 }, { SUIT_DIAMOND_FROM_HP + getXFromHP(), y_ + TOP_ROW_Y_RELATIVE, 8, 8 } );
};

void InventoryLevelGraphics::renderHeartSuit()
{
	Render::renderObject( "bg/level-select-characters.png", { 8, 224, 8, 8 }, { SUIT_HEART_FROM_HP + getXFromHP(), y_ + TOP_ROW_Y_RELATIVE, 8, 8 } );
};

void InventoryLevelGraphics::renderSpadeSuit()
{
	Render::renderObject( "bg/level-select-characters.png", { 8, 232, 8, 8 }, { SUIT_SPADE_FROM_HP + getXFromHP(), y_ + TOP_ROW_Y_RELATIVE, 8, 8 } );
};